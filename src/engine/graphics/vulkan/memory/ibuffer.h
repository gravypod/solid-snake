#ifndef ENGINE_GRAPHICS_VULKAN_MEMORY_IBUFFER_H
#define ENGINE_GRAPHICS_VULKAN_MEMORY_IBUFFER_H


#include <vulkan/vulkan.h>
#include <stdbool.h>
#include <src/engine/graphics/vulkan/vulkan.h>
#include <src/engine/graphics/vulkan/memory/buffer.h>


/**
 * Allocate a buffer for usage in a index buffer
 *
 * @param v
 * @param buffer
 * @param element_size
 * @param num_elements
 * @param map_memory_region
 * @return
 */
bool vulkan_memory_ibuffer_allocate(
		vulkan *v,
		buffer_t *buffer,
		uint32_t element_size,
		uint32_t num_elements,
		bool map_memory_region
);

#endif
