#include <stdio.h>
#include "buffer.h"
#include "pool.h"


bool vulkan_commands_buffer_begin(cbuffer_pool_t *pool, uint32_t cbuffer_id)
{
	VkCommandBuffer buffer = vulkan_commands_cpool_cbuffer_get(pool, cbuffer_id);

	VkCommandBufferBeginInfo command_buffer_begin_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			.pInheritanceInfo = NULL,
	};


	vkResetCommandBuffer(buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

	if (vkBeginCommandBuffer(buffer, &command_buffer_begin_info) != VK_SUCCESS) {
		printf("Failed to start recording on command buffer %u\n", cbuffer_id);
		return false;
	}

	return true;
}


void vulkan_commands_buffer_end(
		cbuffer_pool_t *pool, uint32_t cbuffer_id,
		uint32_t num_waiting_semaphores, VkSemaphore *waiting_semaphores, const VkPipelineStageFlags *waiting_semaphores_signaled_at,
		uint32_t num_signaling_semaphores, VkSemaphore *signaling_semaphores
)
{
	VkQueue queue = pool->owners.queue;
	VkCommandBuffer buffer = vulkan_commands_cpool_cbuffer_get(pool, cbuffer_id);
	VkFence fence = vulkan_commands_cpool_fence_get(pool, cbuffer_id);

	VkSubmitInfo submit_info = {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount = num_waiting_semaphores,
			.pWaitSemaphores = waiting_semaphores,
			.pWaitDstStageMask = waiting_semaphores_signaled_at,


			// Link the command buffer to the render stage
			.commandBufferCount = 1,
			.pCommandBuffers = &buffer,

			// After rendering signal these semphores
			.signalSemaphoreCount = num_signaling_semaphores,
			.pSignalSemaphores = signaling_semaphores
	};

	// Mark that we've started using this command buffer
	vulkan_commands_cpool_cbuffer_id_mark(pool, cbuffer_id);

	vkEndCommandBuffer(buffer);

	// Submitting render job
	if (vkQueueSubmit(queue, 1, &submit_info, fence) != VK_SUCCESS) {
		printf("Failed to render!\n");
	}
}
