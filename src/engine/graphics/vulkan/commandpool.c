#include <stdio.h>
#include <malloc.h>
#include <src/engine/graphics/vulkan/shaders/vbuffer.h>
#include <src/engine/entity/entity.h>
#include <src/engine/entity/manager.h>
#include "commandpool.h"
#include "locking.h"

bool *command_buffer_submitted;
VkFence *fences;
VkCommandBuffer *command_buffer;
size_t current_command_buffer = 0;

VkCommandBuffer *vulkan_command_pool_get(size_t pool_index)
{
	// TODO: Bounds checking
	return &command_buffer[pool_index];
}


VkFence vulkan_command_fence_get(size_t pool_index)
{
	// TODO: Bounds checking
	return fences[pool_index];
}


bool vulkan_command_pool_buffer_init(vulkan *v)
{
	fences = calloc(sizeof(VkFence), v->swapchain.num_images);
	command_buffer = calloc(sizeof(VkCommandBuffer), v->swapchain.num_images);
	command_buffer_submitted = calloc(sizeof(bool), v->swapchain.num_images);
	VkCommandBufferAllocateInfo allocation_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = v->command_pool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = (uint32_t) v->swapchain.num_images
	};

	for (size_t i = 0; i < v->swapchain.num_images; i++) {
		VkFenceCreateInfo fence_create_info = {
				.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				.flags = 0,
				.pNext = NULL
		};
		if (vkCreateFence(v->devices.logical_device, &fence_create_info, NULL, &fences[i]) != VK_SUCCESS) {
			printf("Failed to allocate fence for command buffer!\n");
			return false;
		}
	}

	if (vkAllocateCommandBuffers(v->devices.logical_device, &allocation_info, command_buffer) != VK_SUCCESS) {
		printf("Failed to allocate command buffer for pool\n");
		return false;
	}

	return true;
}

bool vulkan_command_pool_render_pass_begin(vulkan *v, uint32_t image_id)
{
	VkClearValue clear_color = {
			.color.float32 = {
					0, 0, 0, 0
			}
	};
	VkRenderPassBeginInfo render_pass_info = {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = v->pipelines.render_pass,
			.framebuffer = v->swapchain.frame_buffers[image_id],
			.renderArea = {
					.offset = {0, 0},
					.extent = v->swapchain.extent
			},
			.clearValueCount = 1,
			.pClearValues = &clear_color,
	};

	VkCommandBufferBeginInfo command_buffer_begin_info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
			.pInheritanceInfo = NULL,
	};


	vkResetCommandBuffer(command_buffer[image_id], VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

	if (vkBeginCommandBuffer(command_buffer[current_command_buffer], &command_buffer_begin_info) != VK_SUCCESS) {
		printf("Failed to start recording on command buffer %u\n", image_id);
		return false;
	}

	{
		vkCmdBeginRenderPass(command_buffer[current_command_buffer], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
		{
			// TODO: Bind pipeline based on material
			vkCmdBindPipeline(command_buffer[current_command_buffer], VK_PIPELINE_BIND_POINT_GRAPHICS, v->pipelines.graphics);

			// TODO: Maybe put this in a cleaner place?
			entity_manager_draw(command_buffer[current_command_buffer]);

		}
		vkCmdEndRenderPass(command_buffer[current_command_buffer]);
	}

	if (vkEndCommandBuffer(command_buffer[current_command_buffer]) != VK_SUCCESS) {
		printf("Failed to end command buffer sequence when sending render job to %u\n", image_id);
		return false;
	}


	return true;
}

void vulkan_command_pool_render(vulkan *v)
{
	if (command_buffer_submitted[current_command_buffer]) {
		if (vkWaitForFences(v->devices.logical_device, 1, &fences[current_command_buffer], VK_TRUE, UINT64_MAX) != VK_SUCCESS) {
			printf("Failed to wait on rendering fence\n");
			return;
		}
	}

	if (vkResetFences(v->devices.logical_device, 1, &fences[current_command_buffer]) != VK_SUCCESS) {
		printf("Failed to resent rendering fence\n");
	}

	uint32_t image_index;
	vkAcquireNextImageKHR(
			v->devices.logical_device,
			v->swapchain.swapchain,
			UINT64_MAX,
			vulkan_locking_semphore_get_frame_buffer_image_available(),
			VK_NULL_HANDLE,
			&image_index
	);

	if (!vulkan_command_pool_render_pass_begin(v, image_index)) {
		printf("Failed to render\n");
	}

	// Render starting configuration
	VkSemaphore waiting_semaphores[] = {
			vulkan_locking_semphore_get_frame_buffer_image_available()
	};
	VkPipelineStageFlags stages[] = {
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
	};

	// Render ending configuration
	VkSemaphore signaling_semaphores[] = {
			vulkan_locking_semphore_get_rendering_finished()
	};


	VkSubmitInfo submit_info = {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = waiting_semaphores,
			.pWaitDstStageMask = stages,


			// Link the command buffer to the render stage
			.commandBufferCount = 1,
			.pCommandBuffers = vulkan_command_pool_get(image_index),


			// After rendering signal these semphores
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signaling_semaphores
	};

	// Submitting render job
	if (vkQueueSubmit(v->queues.rendering, 1, &submit_info, vulkan_command_fence_get(current_command_buffer)) != VK_SUCCESS) {
		printf("Failed to render!\n");
	}

	// Submitting presenting job
	VkSwapchainKHR subscribing_swapchains[] = {
			v->swapchain.swapchain
	};

	VkPresentInfoKHR present_info = {
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			// Subscribe
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signaling_semaphores,

			.swapchainCount = 1,
			.pSwapchains = subscribing_swapchains,
			.pImageIndices = &image_index,

			.pResults = NULL
	};

	vkQueuePresentKHR(v->queues.presenting, &present_info);

	// mark this command buffer as dirty
	command_buffer_submitted[current_command_buffer] = true;

	// Increment command buffer
	current_command_buffer = (current_command_buffer + 1) % v->swapchain.num_images;
}

bool vulkan_command_pool_init(vulkan *v)
{
	VkCommandPoolCreateInfo poolInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = v->queues.main_rendering_queue_id
	};


	if (vkCreateCommandPool(v->devices.logical_device, &poolInfo, NULL, &v->command_pool) != VK_SUCCESS) {
		printf("Failed to initialize command pool\n");
		return false;
	}

	if (!vulkan_command_pool_buffer_init(v)) {
		return false;
	}

	return true;
}
