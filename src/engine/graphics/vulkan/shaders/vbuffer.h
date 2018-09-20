#ifndef ENGINE_GRAPHICS_VULKAN_SHADERS_VBUFFER_H
#define ENGINE_GRAPHICS_VULKAN_SHADERS_VBUFFER_H


#include <vulkan/vulkan.h>
#include <stdbool.h>
#include <src/engine/graphics/vulkan/vulkan.h>

typedef struct {
    VkBufferCreateInfo info;
    VkBuffer buffer;
    VkMemoryRequirements required_memory;
    VkDeviceMemory memory;
    size_t element_size;
    size_t num_elements;
    void *mapped_memory;
} vbuffer;


bool vulkan_vbuffer_allocate(vulkan *v, vbuffer *buffer, uint32_t element_size, uint32_t num_elements,
                             bool map_memory_region);

#endif
