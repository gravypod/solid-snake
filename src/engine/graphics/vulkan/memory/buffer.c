#include <src/engine/graphics/vulkan/memory/buffer.h>
#include <src/engine/graphics/vulkan/memory/memory.h>
#include <string.h>


bool vulkan_memory_buffer_allocate(
		vulkan *v,
		buffer_t *buffer,
		VkMemoryPropertyFlags buffer_memory_property_flags,
		VkBufferUsageFlags buffer_usage_flags, VkSharingMode buffer_sharing_mode,
		uint32_t element_size, uint32_t num_elements,
		bool map_memory_region
)
{

	if (map_memory_region) {
		// If we want to memory map this region it must be visible to the host!!!
		buffer_memory_property_flags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		buffer_usage_flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	} else {
		// If we dont mmap this region it must be possible to copy into this buffer
		buffer_usage_flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	}

	const VkDeviceSize size = element_size * num_elements;

	// Clear out old buffer
	memset(buffer, 0, sizeof(buffer_t));

	// Engine metadata
	buffer->element_size = element_size;
	buffer->num_elements = num_elements;

	// Set up buffer dimension info
	buffer->info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer->info.size = size;
	buffer->info.usage = buffer_usage_flags;
	buffer->info.sharingMode = buffer_sharing_mode;

	if (vkCreateBuffer(v->devices.logical_device, &buffer->info, NULL, &buffer->buffer) != VK_SUCCESS) {
		return false;
	}

	vkGetBufferMemoryRequirements(v->devices.logical_device, buffer->buffer, &buffer->required_memory);

	if (!vulkan_memory_allocate(v, buffer->required_memory.memoryTypeBits, buffer_memory_property_flags, buffer->required_memory.size, &buffer->memory)) {
		return false;
	}

	// Bind the memory to the new buffer
	if (vkBindBufferMemory(v->devices.logical_device, buffer->buffer, buffer->memory, 0) != VK_SUCCESS) {
		return false;
	}

	if (map_memory_region) {
		// Map memory location into trapped page (I guess?)
		if (!vulkan_memory_map(v, buffer->memory, 0, buffer->info.size, &buffer->mapped_memory)) {
			return false;
		}
	}

	return true;
}


void vulkan_memory_buffer_free(vulkan *v, buffer_t *buffer)
{
	if (!buffer || !buffer->memory) {
		return;
	}

	// If we have mapped memory, we need to unmap it first.
	if (buffer->mapped_memory) {
		vulkan_memory_unmap(v, buffer->memory, &buffer->mapped_memory);
	}

	vulkan_memory_free(v, buffer->memory);
	// Clear out old buffer
	memset(buffer, 0, sizeof(buffer_t));
}

void vulkan_memory_buffer_copy(
		buffer_t *from, buffer_t *to,
		VkCommandBuffer buffer
)
{
	VkBufferCopy copy_request = {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = from->info.size,
	};
	vkCmdCopyBuffer(buffer, from->buffer, to->buffer, 1, &copy_request);
}
