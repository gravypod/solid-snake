#include <malloc.h>
#include "framebuffer.h"

bool vulkan_framebuffer_init(vulkan *v) {
    v->swapchain.frame_buffers = malloc(sizeof(VkFramebuffer) * v->swapchain.num_images);

    VkFramebufferCreateInfo framebuffer_create_info = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = v->pipelines.render_pass,
            .attachmentCount = 1,
            .pAttachments = NULL, // configured later
            .width = v->swapchain.extent.width,
            .height = v->swapchain.extent.height,
            .layers = 1
    };

    for (size_t i = 0; i < v->swapchain.num_images; i++) {
        framebuffer_create_info.pAttachments = &v->swapchain.image_views[i];
        VkResult creation_result = vkCreateFramebuffer(
                v->devices.logical_device,
                &framebuffer_create_info,
                NULL,
                &v->swapchain.frame_buffers[i]
        );

        if (creation_result != VK_SUCCESS) {
            printf("Failed to create vulkan framebuffer for swapchain image %zu\n", i);
            return false;
        }
    }

    return true;
}
