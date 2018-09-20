#include <stdio.h>
#include <malloc.h>
#include <src/engine/graphics/vulkan/shaders/vbuffer.h>
#include "commandpool.h"

VkCommandBuffer *command_buffer;

VkCommandBuffer *vulkan_command_pool_get(size_t pool_index) {
    // TODO: Bounds checking
    return &command_buffer[pool_index];
}

bool vulkan_command_pool_buffer_init(vulkan *v) {
    command_buffer = malloc(sizeof(VkCommandBuffer) * v->swapchain.num_images);
    VkCommandBufferAllocateInfo allocation_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = v->command_pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = (uint32_t) v->swapchain.num_images
    };


    if (vkAllocateCommandBuffers(v->devices.logical_device, &allocation_info, command_buffer) != VK_SUCCESS) {
        printf("Failed to allocate command buffer for pool\n");
        return false;
    }

    return true;
}

bool vulkan_command_pool_buffer_recording_start(vulkan *v) {
    for (size_t i = 0; i < v->swapchain.num_images; i++) {
        VkCommandBufferBeginInfo command_buffer_begin_info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
                .pInheritanceInfo = NULL
        };

        if (vkBeginCommandBuffer(command_buffer[i], &command_buffer_begin_info) != VK_SUCCESS) {
            printf("Failed to start recording on command buffer %zu\n", i);
            return false;
        }
    }

    return true;
}


bool vulkan_command_pool_render_pass_begin(vulkan *v, vbuffer *buffer) {
    for (size_t i = 0; i < v->swapchain.num_images; i++) {
        VkClearValue clear_color = {
                .color.float32 = {
                        0, 0, 0, 0
                }
        };
        VkRenderPassBeginInfo render_pass_info = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .renderPass = v->pipelines.render_pass,
                .framebuffer = v->swapchain.frame_buffers[i],
                .renderArea = {
                        .offset = {0, 0},
                        .extent = v->swapchain.extent
                },
                .clearValueCount = 1,
                .pClearValues = &clear_color,
        };

        {
            vkCmdBeginRenderPass(command_buffer[i], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
            {
                vkCmdBindPipeline(command_buffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, v->pipelines.graphics);

                // TODO: How the fuck to I get this buffer data into here?!
                VkBuffer vertexBuffers[] = {buffer->buffer};
                VkDeviceSize offsets[] = {0};
                vkCmdBindVertexBuffers(command_buffer[i], 0, 1, vertexBuffers, offsets);

                vkCmdDraw(command_buffer[i], (uint32_t) buffer->num_elements, 1, 0, 0);

            }
            vkCmdEndRenderPass(command_buffer[i]);


            if (vkEndCommandBuffer(command_buffer[i]) != VK_SUCCESS) {
                printf("Failed to end command buffer sequence when sending render job to %zu\n", i);
                return false;
            }
        }
    }

    return true;
}

bool vulkan_command_pool_init(vulkan *v, vbuffer *buffer) {
    VkCommandPoolCreateInfo poolInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = 0,
            .queueFamilyIndex = v->queues.main_rendering_queue_id
    };


    if (vkCreateCommandPool(v->devices.logical_device, &poolInfo, NULL, &v->command_pool) != VK_SUCCESS) {
        printf("Failed to initialize command pool\n");
        return false;
    }

    if (!vulkan_command_pool_buffer_init(v)) {
        return false;
    }

    if (!vulkan_command_pool_buffer_recording_start(v)) {
        return false;
    }

    if (!vulkan_command_pool_render_pass_begin(v, buffer)) {
        return false;
    }

    return true;
}
