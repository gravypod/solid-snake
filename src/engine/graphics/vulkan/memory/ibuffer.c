#include "ibuffer.h"

bool vulkan_memory_ibuffer_allocate(
		vulkan *v,
		buffer_t *buffer,
		uint32_t element_size,
		uint32_t num_elements,
		bool map_memory_region
)
{
	// Memory is going to be used to store vertex data
	const VkBufferUsageFlags usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	const VkSharingMode sharing = VK_SHARING_MODE_EXCLUSIVE;
	return vulkan_memory_buffer_allocate(v, buffer, 0, usage, sharing, element_size, num_elements, map_memory_region);
}
