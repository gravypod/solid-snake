#include "render.h"

void vulkan_commands_render_begin(vulkan *v, cbuffer_pool_t *pool, uint32_t cbuffer_id, uint32_t swapchain_image_id)
{
	VkClearValue clear_color = {
			.color.float32 = {
					0, 0, 0, 0
			}
	};
	VkRenderPassBeginInfo render_pass_info = {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = v->pipelines.render_pass,
			.framebuffer = v->swapchain.frame_buffers[swapchain_image_id],
			.renderArea = {
					.offset = {0, 0},
					.extent = v->swapchain.extent
			},
			.clearValueCount = 1,
			.pClearValues = &clear_color,
	};


	vkCmdBeginRenderPass(pool->buffers[cbuffer_id], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
}


void vulkan_commands_render_end(cbuffer_pool_t *pool, uint32_t cbuffer_id)
{
	vkCmdEndRenderPass(pool->buffers[cbuffer_id]);
}
