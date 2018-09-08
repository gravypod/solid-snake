#include <stdint.h>
#include <malloc.h>
#include <memory.h>
#include "queues.h"
#include "vulkan.h"

VkResult vulkan_queues_init_queue_family(vulkan *v, VkPhysicalDevice device, uint32_t queue, float priority) {
    VkDeviceQueueCreateInfo queue_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queue,
            .queueCount = 1,
            .pQueuePriorities = &priority
    };

    VkPhysicalDeviceFeatures device_features;
    memset(&device_features, 0, sizeof(VkPhysicalDeviceFeatures));

    VkDeviceCreateInfo request = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pQueueCreateInfos = &queue_create_info,
            .queueCreateInfoCount = 1,
            .pEnabledFeatures = &device_features,

            // No device extensions enabled for now. We will enable swapchain later
            .enabledExtensionCount = 0,

            // Validation Layers
            .enabledLayerCount = v->required_configuration.num_layers,
            .ppEnabledLayerNames = v->required_configuration.layers
    };

    return vkCreateDevice(device, &request, NULL, &v->devices.logical_device);
}

void vulkan_queues_cleanup(vulkan *v) {
    vkDestroyDevice(v->devices.logical_device, NULL);
}

bool vulkan_queues_init(vulkan *v) {
    VkPhysicalDevice device = v->devices.selected_device;

    // Find properties
    vkGetPhysicalDeviceQueueFamilyProperties(device, &v->queues.num_properties, NULL);
    v->queues.properties = malloc(sizeof(VkQueueFamilyProperties) * v->queues.num_properties);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &v->queues.num_properties, v->queues.properties);


    v->queues.main_rendering_queue_id = UINT32_MAX;

    for (uint32_t i = 0; i < v->queues.num_properties; i++) {
        VkQueueFamilyProperties *properties = &v->queues.properties[i];

        // Find graphics queue
        if (properties->queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            v->queues.main_rendering_queue_id = i;
            break;
        }
    }

    if (v->queues.main_rendering_queue_id == UINT32_MAX) {
        return false;
    }

    if (vulkan_queues_init_queue_family(v, v->devices.selected_device, v->queues.main_rendering_queue_id, 1.0f) != VK_SUCCESS) {
        printf("Failed to initialize queue family\n");
        return false;
    }

    // Load instance into vulkan structure
    vkGetDeviceQueue(v->devices.logical_device, v->queues.main_rendering_queue_id, 0, &v->queues.rendering);

    return true;
}
