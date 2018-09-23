#include <src/engine/util/bits.h>
#include "memory.h"

vulkan_device_memory_info info;

static inline bool vulkan_memory_properties_has_all_desired_features(
		const VkMemoryPropertyFlags available,
		const VkMemoryPropertyFlags desired
)
{
	return IS_EVERY_BIT_SET(available, desired);
}

uint32_t vulkan_memory_type_find(uint32_t memory_type_mask, VkMemoryPropertyFlags properties)
{
	for (uint32_t i = 0; i < info.properties.memoryTypeCount; i++) {
		const VkMemoryPropertyFlags memory_type_properties = info.properties.memoryTypes[i].propertyFlags;

		if (!IS_BIT_SET(memory_type_mask, i)) {
			continue;
		}

		if (!vulkan_memory_properties_has_all_desired_features(memory_type_properties, properties)) {
			continue;
		}

		return i;
	}

	return UINT32_MAX;
}

// TODO: Implement memory mapping, flushing mapped memory, and invalidating mapped memory caches.
// TODO: Move memory mapping out of vbuffer.c/h

bool vulkan_memory_allocate(
		const vulkan *const v,
		const uint32_t memory_type_mask,
		const VkMemoryPropertyFlags properties,
		const uint32_t size,
		VkDeviceMemory *const memory
)
{

	// If you don't store the memory address we refuse to allocate anything
	if (!memory) {
		return false;
	}

	// Memory index that supports our variable filter flags
	uint32_t memory_type_index = vulkan_memory_type_find(memory_type_mask, properties);

	if (memory_type_index == UINT32_MAX) {
		return false;
	}

	VkMemoryAllocateInfo memory_allocation_info = {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = size,
			.memoryTypeIndex = memory_type_index
	};

	return vkAllocateMemory(v->devices.logical_device, &memory_allocation_info, NULL, memory) == VK_SUCCESS;
}

void vukan_memory_free(
		const vulkan *const v,
		VkDeviceMemory memory
)
{
	vkFreeMemory(v->devices.logical_device, memory, NULL);
}

bool vulkan_memory_init(vulkan *v)
{
	vkGetPhysicalDeviceMemoryProperties(v->devices.selected_device, &info.properties);
	return true;
}
