#ifndef ENGINE_MEMORY_H
#define ENGINE_MEMORY_H

#include <stdbool.h>
#include "vulkan.h"

typedef struct {
    VkPhysicalDeviceMemoryProperties properties;
} vulkan_device_memory_info;

bool vulkan_memory_allocate(
        const vulkan *v,
        uint32_t memory_type_mask,
        VkMemoryPropertyFlags properties,
        uint32_t size,
        VkDeviceMemory *memory
);

void vukan_memory_free(
        const vulkan *v,
        VkDeviceMemory memory
);

bool vulkan_memory_init(vulkan *v);

#endif
