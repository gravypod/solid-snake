#include <stdio.h>
#include <src/engine/graphics/vulkan/vulkan.h>
#include <src/engine/graphics/vulkan/commands/pool.h>
#include <malloc.h>

/**
 * Allocate a VkCommandPool for a given device and queue
 *
 * @param device
 * @param queue_family_index
 * @return
 */
VkCommandPool vulkan_commands_cpool_commandpool_allocate(VkDevice device, uint32_t queue_family_index)
{
	VkCommandPool result;
	VkCommandPoolCreateInfo command_pool_create_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = queue_family_index
	};


	if (vkCreateCommandPool(device, &command_pool_create_info, NULL, &result) != VK_SUCCESS) {
		printf("Failed to initialize command pool\n");
		return VK_NULL_HANDLE;
	}

	return result;
}

bool vulkan_commands_cpool_fences_allocate(VkDevice device, VkFence *fences, uint32_t size)
{
	const VkFenceCreateInfo fence_create_info = {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = 0,
			.pNext = NULL
	};

	for (uint32_t i = 0; i < size; i++) {
		if (vkCreateFence(device, &fence_create_info, NULL, &fences[i]) != VK_SUCCESS) {
			printf("Failed to allocate fence for command buffer!\n");
			return false;
		}
	}
	return true;
}

bool vulkan_commands_cpool_buffers_allocate(VkDevice device, VkCommandPool pool, VkCommandBuffer *buffers, uint32_t size)
{
	const VkCommandBufferAllocateInfo allocation_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = pool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = size
	};

	if (vkAllocateCommandBuffers(device, &allocation_info, buffers) != VK_SUCCESS) {
		printf("Failed to allocate command buffer for pool\n");
		return false;
	}

	return true;
}

cbuffer_pool_t *vulkan_commands_cpool_allocate(vulkan *v, uint32_t size)
{
	cbuffer_pool_t *pool = calloc(sizeof(cbuffer_pool_t), 1);

	pool->owners.device = v->devices.logical_device;
	pool->owners.pool = vulkan_commands_cpool_commandpool_allocate(v->devices.logical_device, v->queues.main_rendering_queue_id);
	pool->owners.queue = v->queues.rendering;
	pool->size = size;

	pool->fences = calloc(sizeof(VkFence), size);
	pool->already_submitted = calloc(sizeof(bool), size); // this all defaults to 0 which is the correct behavior
	pool->buffers = calloc(sizeof(VkCommandBuffer), size);

	// Create resources on device
	vulkan_commands_cpool_fences_allocate(v->devices.logical_device, pool->fences, size);
	vulkan_commands_cpool_buffers_allocate(v->devices.logical_device, pool->owners.pool, pool->buffers, size);

	// We haven't use anything yet
	pool->last_used = 0;

	return pool;
}


uint32_t vulkan_commands_cpool_cbuffer_id_take(cbuffer_pool_t *pool)
{
	VkDevice device = pool->owners.device;
	// Wait for and locate an unused fence
	while (true) {
		uint32_t peaking = CBUFFER_POOL_NEXT_INDEX(pool->last_used, pool->size);

		// Check to see if we've come full circle around out command buffer
		while (peaking != pool->last_used) {
			VkFence fence = vulkan_commands_cpool_fence_get(pool, peaking);
			VkCommandBuffer buffer = vulkan_commands_cpool_cbuffer_get(pool, peaking);


			// We have found a good fence if it's never been submitted before OR if the fence has been signaled by the GPU
			if (!pool->already_submitted[peaking] || vkGetFenceStatus(device, fence) == VK_SUCCESS) {

				// If this was already submitted we need to clean up the Fence and CommandBuffer object
				if (pool->already_submitted[peaking]) {
					vkResetFences(device, 1, &fence);
					vkResetCommandBuffer(buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
				}

				return peaking;
			}

			peaking = CBUFFER_POOL_NEXT_INDEX(peaking, pool->size);
		}

		// Block until we have a fence available
		if (vkWaitForFences(device, pool->size, pool->fences, VK_FALSE, UINT64_MAX) != VK_SUCCESS) {
			printf("Failed to block on fences");
		}
	}
}

VkCommandBuffer vulkan_commands_cpool_cbuffer_get(cbuffer_pool_t *pool, uint32_t cbuffer_id)
{
	// TODO: Locking & bounds checking
	return pool->buffers[cbuffer_id];
}

VkFence vulkan_commands_cpool_fence_get(cbuffer_pool_t *pool, uint32_t cbuffer_id)
{
	// TODO: Locking & bounds checking
	return pool->fences[cbuffer_id];
}

void vulkan_commands_cpool_cbuffer_id_mark(cbuffer_pool_t *pool, uint32_t cbuffer_id)
{
	pool->last_used = cbuffer_id;
	pool->already_submitted[cbuffer_id] = true;
}
