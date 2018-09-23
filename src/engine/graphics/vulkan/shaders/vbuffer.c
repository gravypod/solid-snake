#include <string.h>
#include <src/engine/graphics/vulkan/memory/memory.h>
#include "vbuffer.h"

const VkMemoryPropertyFlags vbuffer_memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

bool vulkan_vbuffer_allocate(
		vulkan *v,
		buffer_t *buffer,
		uint32_t element_size,
		uint32_t num_elements,
		bool map_memory_region
)
{
	return vulkan_buffer_allocate(
			v,
			buffer,
			vbuffer_memory_properties,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE,
			element_size, num_elements,
			map_memory_region
	);
}
