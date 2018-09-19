#include <stdint.h>
#include <malloc.h>
#include <memory.h>
#include "queues.h"
#include "vulkan.h"

typedef bool (*vulkan_queues_selection_criteria_t)(vulkan *v, VkPhysicalDevice device, uint32_t i,
                                                   VkQueueFamilyProperties *properties);


bool
vulkan_queues_selection_filter_is_presentable(vulkan *v, VkPhysicalDevice device, uint32_t i,
                                              VkQueueFamilyProperties *properties) {
    ((void) properties);
    VkBool32 presentable = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, v->surface, &presentable);
    return (bool) presentable;
}

bool
vulkan_queues_selection_filter_is_graphics_queue(vulkan *v, VkPhysicalDevice device, uint32_t i,
                                                 VkQueueFamilyProperties *properties) {

    ((void) v);
    ((void) device);
    ((void) i);
    return (bool) properties->queueFlags & VK_QUEUE_GRAPHICS_BIT;
}

VkResult vulkan_queues_init_queue_family(vulkan *v, VkPhysicalDevice device, float priority) {


    v->queues.num_queue_families = 2;

    // We don't need to double create the same queue.
    // TODO: Hack. Make this handle a dynamic number of queues
    if (v->queues.main_presentation_queue_id == v->queues.main_rendering_queue_id) {
        v->queues.num_queue_families = 1;
        v->queues.queue_families[0] = v->queues.main_presentation_queue_id;
    } else {
        v->queues.num_queue_families = 2;
        v->queues.queue_families[0] = v->queues.main_presentation_queue_id;
        v->queues.queue_families[1] = v->queues.main_rendering_queue_id;
    }

    VkDeviceQueueCreateInfo queue_creations[] = {
            {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .queueFamilyIndex = v->queues.main_rendering_queue_id,
                    .queueCount = 1,
                    .pQueuePriorities = &priority
            },
            {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .queueFamilyIndex = v->queues.main_presentation_queue_id,
                    .queueCount = 1,
                    .pQueuePriorities = &priority
            },
    };

    VkPhysicalDeviceFeatures device_features;
    memset(&device_features, 0, sizeof(VkPhysicalDeviceFeatures));

    // Geometry shaders enabled. TODO: Refactor
    device_features.geometryShader = VK_TRUE;

    VkDeviceCreateInfo request = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pQueueCreateInfos = queue_creations,
            .queueCreateInfoCount = v->queues.num_queue_families,
            .pEnabledFeatures = &device_features,

            // Device extensions
            .enabledExtensionCount = v->required_configuration.num_logical_extensions,
            .ppEnabledExtensionNames = v->required_configuration.logical_extensions,

            // Validation Layers
            .enabledLayerCount = v->required_configuration.num_layers,
            .ppEnabledLayerNames = v->required_configuration.layers
    };


    return vkCreateDevice(device, &request, NULL, &v->devices.logical_device);
}


uint32_t vulkan_queues_find_appropriate_queue(vulkan *v, vulkan_queues_selection_criteria_t filter) {
    for (uint32_t i = 0; i < v->queues.num_properties; i++) {
        VkQueueFamilyProperties *properties = &v->queues.properties[i];

        if (filter(v, v->devices.selected_device, i, properties)) {
            return i;
        }
    }
    return UINT32_MAX;
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


    v->queues.main_rendering_queue_id = vulkan_queues_find_appropriate_queue(v,
                                                                             vulkan_queues_selection_filter_is_graphics_queue);
    v->queues.main_presentation_queue_id = vulkan_queues_find_appropriate_queue(v,
                                                                                vulkan_queues_selection_filter_is_presentable);

    if (v->queues.main_rendering_queue_id == UINT32_MAX || v->queues.main_presentation_queue_id == UINT32_MAX) {
        return false;
    }


    if (vulkan_queues_init_queue_family(v, v->devices.selected_device, 1.0f) != VK_SUCCESS) {
        printf("Failed to initialize queue family\n");
        return false;
    }


    // Load instance into vulkan structure
    vkGetDeviceQueue(v->devices.logical_device, v->queues.main_rendering_queue_id, 0, &v->queues.rendering);
    vkGetDeviceQueue(v->devices.logical_device, v->queues.main_presentation_queue_id, 0, &v->queues.presenting);

    return true;
}
