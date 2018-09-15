#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include "query.h"
#include "vulkan.h"
#include "config.h"
#include "debug.h"
#include "queues.h"
#include "window.h"
#include "surface.h"
#include "swapchain.h"
#include "shader.h"
#include "pipeline.h"
#include "framebuffer.h"
#include "commandpool.h"
#include "locking.h"

vulkan v = {
        .definition =  {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = "Solid Snake",
                .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
                .pEngineName = "Solid Engine",
                .engineVersion = VK_MAKE_VERSION(0, 0, 1),
                .apiVersion = VK_API_VERSION_1_0
        },
        .devices.selected_device = VK_NULL_HANDLE
};


VkResult vulkan_create_instance() {
    VkInstanceCreateInfo creation_request = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &v.definition
    };

    creation_request.ppEnabledLayerNames = v.required_configuration.layers;
    creation_request.enabledLayerCount = v.required_configuration.num_layers;

    creation_request.ppEnabledExtensionNames = v.required_configuration.extensions;
    creation_request.enabledExtensionCount = v.required_configuration.num_extensions;

    return vkCreateInstance(&creation_request, NULL, &v.instance);
}

void vulkan_info_print() {
    printf("Vulkan Initialized\n");

    printf("Loaded Vulkan Extensions: %u\n", v.extensions.num_properties);
    for (uint32_t i = 0; i < v.extensions.num_properties; i++) {
        printf("\t%d - %s (%d)\n", i, v.extensions.properties[i].extensionName, v.extensions.properties[i].specVersion);
    }

    printf("Loaded Vulkan Instance Layers: %u\n", v.layers.num_properties);
    for (uint32_t i = 0; i < v.layers.num_properties; i++) {
        printf("\t%d - %s (%d)\n", i, v.layers.properties[i].layerName, v.layers.properties[i].specVersion);
    }

    printf("Using %u as the main graphics queue\n", v.queues.main_rendering_queue_id);
}


bool vulkan_init() {

    if (!vulkan_window_init(&v)) {
        printf("Failed to create window!\n");
        return false;
    }

    // Find hardware info
    if (!vulkan_hardware_query(&v)) {
        printf("Failed to query vulkan hardware!\n");
        return false;
    }

    // Choose extensions and layers to use
    vulkan_config_init(&v);

    // Create vulkan instance
    if (vulkan_create_instance() != VK_SUCCESS) {
        printf("Failed to create vulkan instance\n");
        return false;
    }

    // Start the debugger
    vulkan_debug_init(&v);

    // Select a device
    if ((v.devices.selected_device = vulkan_config_pick_physical_device(&v)) == VK_NULL_HANDLE) {
        printf("Failed to find suitable device!\n");
        return false;
    }

    // Make drawable surface
    if (!vulkan_surface_init(&v)) {
        printf("Failed to create drawing surface\n");
        return false;
    }

    // Find a queue with graphics pipeline support
    if (!vulkan_queues_init(&v)) {
        printf("Could not find graphics bit in chosen device!\n");
        return false;
    }

    if (!vulkan_swapchain_init(&v)) {
        return false;
    }

    if (!vulkan_shader_init(&v)) {
        return false;
    }


    if (!vulkan_locking_init(&v)) {
        printf("Could not create locks\n");
        return false;
    }

    if (!vulkan_pipeline_init(&v)) {
        return false;
    }

    if (!vulkan_framebuffer_init(&v)) {
        return false;
    }

    if (!vulkan_command_pool_init(&v)) {
        return false;
    }

    // Info to prove we have loaded everything
    vulkan_info_print();

    return true;
}

void vulkan_render() {
    uint32_t image_index;
    vkAcquireNextImageKHR(
            v.devices.logical_device,
            v.swapchain.swapchain,
            UINT64_MAX,
            vulkan_locking_semphore_get_frame_buffer_image_available(),
            VK_NULL_HANDLE,
            &image_index
    );

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
    if (vkQueueSubmit(v.queues.rendering, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS) {
        printf("Failed to render!\n");
    }

    // Submitting presenting job
    VkSwapchainKHR subscribing_swapchains[] = {
            v.swapchain.swapchain
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

    vkQueuePresentKHR(v.queues.presenting, &present_info);
}

void vulkan_update() {
    vulkan_window_update();
}

void vulkan_cleanup() {
    vulkan_window_cleanup();
}
