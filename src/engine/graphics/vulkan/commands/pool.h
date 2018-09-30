#ifndef ENGINE_GRAPHICS_VULKAN_COMMANDS_POOL_H
#define ENGINE_GRAPHICS_VULKAN_COMMANDS_POOL_H


#include <src/engine/graphics/vulkan/vulkan.h>
#include <stdbool.h>

#define CBUFFER_POOL_NEXT_INDEX(last_used, size) ((last_used + 1) % size)

typedef struct
{
	struct
	{
		// The device we are allocated against
		VkDevice device;

		// The CommandPool object we allocated
		VkCommandPool pool;

		// The queue we intend to submit to
		VkQueue queue;
	} owners;

	uint32_t last_used;

	uint32_t size;
	bool *already_submitted;
	VkFence *fences;
	VkCommandBuffer *buffers;

} cbuffer_pool_t;

/**
 * Allocate a pool of command buffers
 * @param v
 * @param size
 * @return
 */
cbuffer_pool_t *vulkan_commands_cpool_allocate(vulkan *v, uint32_t size);

/**
 * Find an unused VkCommandBuffer and VkFence pair.
 * @param pool
 * @return
 */
uint32_t vulkan_commands_cpool_cbuffer_id_take(cbuffer_pool_t *pool);

/**
 * Get a VkCommandBuffer by it's cbuffer_id
 *
 * @param pool
 * @param cbuffer_id
 * @return
 */
VkCommandBuffer vulkan_commands_cpool_cbuffer_get(cbuffer_pool_t *pool, uint32_t cbuffer_id);

/**
 * Get a VkFence by it's cbuffer_id
 * @param pool
 * @param cbuffer_id
 * @return
 */
VkFence vulkan_commands_cpool_fence_get(cbuffer_pool_t *pool, uint32_t cbuffer_id);

/**
 * Mark that we've dirtied this CommandBuffer and Fence
 * @param pool
 * @return
 */
void vulkan_commands_cpool_cbuffer_id_mark(cbuffer_pool_t *pool, uint32_t cbuffer_id);

#endif
