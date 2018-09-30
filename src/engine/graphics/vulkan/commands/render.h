#ifndef ENGINE_GRAPHICS_VULKAN_COMMANDS_RENDER_H
#define ENGINE_GRAPHICS_VULKAN_COMMANDS_RENDER_H

#include <stdint.h>
#include <src/engine/graphics/vulkan/vulkan.h>
#include <src/engine/graphics/vulkan/commands/pool.h>


/**
 * Begin a swapchain command
 *
 * @param v
 * @param pool
 * @param cbuffer_id
 * @param swapchain_image_id
 */
void vulkan_commands_render_begin(vulkan *v, cbuffer_pool_t *pool, uint32_t cbuffer_id, uint32_t swapchain_image_id);

/**
 * End a swapchain command
 *
 * @param pool
 * @param cbuffer_id
 */
void vulkan_commands_render_end(cbuffer_pool_t *pool, uint32_t cbuffer_id);

#endif
