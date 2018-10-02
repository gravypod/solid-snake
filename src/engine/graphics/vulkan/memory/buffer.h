#ifndef ENGINE_GRAPHICS_VULKAN_MEMORY_BUFFER_H
#define ENGINE_GRAPHICS_VULKAN_MEMORY_BUFFER_H

#include <vulkan/vulkan.h>
#include <stdbool.h>
#include <src/engine/graphics/vulkan/vulkan.h>

typedef struct
{
	VkBufferCreateInfo info;
	VkBuffer buffer;
	VkMemoryRequirements required_memory;
	VkDeviceMemory memory;
	size_t element_size;
	size_t num_elements;
	void *mapped_memory;
} buffer_t;


/**
 * Allocate a buffer
 *
 * @param v - Vulkan instance to attach to
 * @param buffer - Buffer to create
 * @param buffer_memory_property_flags - What interface requirements we have. (CPU can see? Coherent? etc)
 * @param buffer_usage_flags - Mask provided by the GPU which describes where we can look for memory
 * @param buffer_sharing_mode - Sharing mode
 * @param element_size - Width of a single element within the buffer
 * @param num_elements - Number of elements in the buffer
 * @param map_memory_region - If we should map a segment of memory for this buffer
 * @return
 */
bool vulkan_memory_buffer_allocate(
		vulkan *v,
		buffer_t *buffer,
		VkMemoryPropertyFlags buffer_memory_property_flags,
		VkBufferUsageFlags buffer_usage_flags, VkSharingMode buffer_sharing_mode,
		uint32_t element_size, uint32_t num_elements,
		bool map_memory_region
);

/**
 * Vulkan free a memory buffer from the GPU
 * @param v
 * @param buffer
 */
void vulkan_memory_buffer_free(vulkan *v, buffer_t *buffer);

/**
 * Schedule a buffer copy into the VkCommandBuffer
 * @param from - Source buffer
 * @param to - Destination buffer
 * @param buffer - Command buffer
 */
void vulkan_memory_buffer_copy(
		buffer_t *from, buffer_t *to,
		VkCommandBuffer buffer
);

#endif
