#ifndef ENGINE_GRAPHICS_VULKAN_COMMANDS_BUFFER_H
#define ENGINE_GRAPHICS_VULKAN_COMMANDS_BUFFER_H

#include <src/engine/graphics/vulkan/commands/pool.h>


/**
 * Begin a command buffer
 *
 * @param pool
 * @param cbuffer_id
 * @return
 */
bool vulkan_commands_buffer_begin(cbuffer_pool_t *pool, uint32_t cbuffer_id);

/**
 * Submit a command buffer to the GPU on a given queue
 *
 * @param pool
 * @param cbuffer_id
 * @param num_waiting_semaphores
 * @param waiting_semaphores
 * @param waiting_semaphores_signaled_at
 * @param num_signaling_semaphores
 * @param signaling_semaphores
 */
void vulkan_commands_buffer_end(
		cbuffer_pool_t *pool, uint32_t cbuffer_id,
		uint32_t num_waiting_semaphores, VkSemaphore *waiting_semaphores, const VkPipelineStageFlags *waiting_semaphores_signaled_at,
		uint32_t num_signaling_semaphores, VkSemaphore *signaling_semaphores
);


#endif
