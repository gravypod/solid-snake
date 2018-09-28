#ifndef ENGINE_GRAPHICS_VULKAN_SHADERS_VBUFFER_H
#define ENGINE_GRAPHICS_VULKAN_SHADERS_VBUFFER_H


#include <vulkan/vulkan.h>
#include <stdbool.h>
#include <src/engine/graphics/vulkan/vulkan.h>
#include <src/engine/graphics/vulkan/memory/buffer.h>


bool vulkan_vbuffer_allocate(vulkan *v, buffer_t *buffer, uint32_t element_size, uint32_t num_elements,
                             bool map_memory_region);

#endif
