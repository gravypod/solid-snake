#include <malloc.h>
#include "config.h"
#include "vulkan.h"
#include "debug.h"

#define NUM_LOGICAL_EXTENSIONS 1
const char *logical_extensions[NUM_LOGICAL_EXTENSIONS] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


bool vulkan_config_physical_device_is_suitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &features);

    return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader;
}

VkPhysicalDevice vulkan_config_pick_physical_device(vulkan *v) {
    if (vkEnumeratePhysicalDevices(v->instance, &v->devices.num_devices, NULL) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    v->devices.devices = malloc(sizeof(VkPhysicalDevice) * v->devices.num_devices);
    if (vkEnumeratePhysicalDevices(v->instance, &v->devices.num_devices, v->devices.devices) != VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    for (uint32_t i = 0; i < v->devices.num_devices; i++) {
        if (vulkan_config_physical_device_is_suitable(v->devices.devices[i])) {
            return v->devices.devices[i];
        }
    }

    return VK_NULL_HANDLE;
}


bool vulkan_config_init(vulkan *v) {
    // Request all GLFW extensions to be loaded up
    v->required_configuration.num_extensions = v->g.num_extensions + 1;
    v->required_configuration.extensions = malloc(sizeof(char *) * v->required_configuration.num_extensions);
    for (size_t i = 0; i < v->g.num_extensions; i++) {
        v->required_configuration.extensions[i] = v->g.extensions[i];
    }

    // Requrest for debugging extensions to be loaded up
    v->required_configuration.extensions[v->g.num_extensions] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

    // request all validation layers
    v->required_configuration.num_layers = vulkan_debug_num_validation_layers();
    v->required_configuration.layers = vulkan_debug_validation_layers();


    v->required_configuration.num_logical_extensions = NUM_LOGICAL_EXTENSIONS;
    v->required_configuration.logical_extensions = logical_extensions;

    return true;
}
