#ifndef ENGINE_MEMORY_H
#define ENGINE_MEMORY_H

#include <stdbool.h>
#include "src/engine/graphics/vulkan/vulkan.h"

typedef struct
{
	VkPhysicalDeviceMemoryProperties properties;
} vulkan_device_memory_info;

bool vulkan_memory_allocate(
		const vulkan *v,
		uint32_t memory_type_mask,
		VkMemoryPropertyFlags properties,
		VkDeviceSize size,
		VkDeviceMemory *memory
);

void vulkan_memory_free(
		const vulkan *v,
		VkDeviceMemory memory
);

bool vulkan_memory_map(
		const vulkan *v,
		VkDeviceMemory memory,
		VkDeviceSize offset, VkDeviceSize size,
		void **ptr
);

void vulkan_memory_unmap(
		const vulkan *v,
		VkDeviceMemory memory,
		void **ptr
);

bool vulkan_memory_init(vulkan *v);

#endif
