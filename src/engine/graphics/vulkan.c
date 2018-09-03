#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "vulkan.h"
#include "wrappers/vulkan_debugging.h"

glfw_state glfw;
vulkan_state vulkan = {
        .physicalDevice = VK_NULL_HANDLE
};

/*
 * Provide vulkan with some context for our application
 */
VkApplicationInfo definition = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Solid Snake",
        .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName = "Solid Engine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_0
};

#define NUM_WANTED_VALIDATION_LAYERS ((uint32_t) 2)
const char *wanted_validation_layers[NUM_WANTED_VALIDATION_LAYERS] = {
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_LUNARG_standard_validation"

};


static VKAPI_ATTR VkBool32 VKAPI_CALL on_vulkan_validation_failure(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT *data,
        void *user_date
) {
    // Ignore lack of usage
    ((void) severity);
    ((void) type);
    ((void) data);
    ((void) user_date);

    printf("validation layer: %s\n", data->pMessage);

    return VK_FALSE;
}

void init_debug_callback() {
    VkDebugUtilsMessengerCreateInfoEXT creation_request = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = on_vulkan_validation_failure,
            .pUserData = NULL // Optional
    };

    wrap_vulkan_create_debug_utils_messenger_ext(
            vulkan.instance,
            &creation_request,
            NULL,
            &vulkan.debug_utils_messenger_callback
    );
}


/**
 * Check to make sure all of our desired validation layers can be provided by vulkan
 * @return
 */
bool has_all_wanted_validation_layers() {
    for (uint32_t wanted_layer_index = 0; wanted_layer_index < NUM_WANTED_VALIDATION_LAYERS; wanted_layer_index++) {
        bool found = false;

        // Search array of available layers
        for (uint32_t i = 0; !found && i < vulkan.num_layer_properties; i++) {
            found = strcmp(wanted_validation_layers[wanted_layer_index], vulkan.layer_properties[i].layerName) == 0;
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

VkResult create_vulkan_application_interface() {
    VkInstanceCreateInfo creation_request = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &definition
    };
    creation_request.enabledExtensionCount = glfw.num_extensions;
    creation_request.ppEnabledExtensionNames = glfw.extensions;

    creation_request.ppEnabledLayerNames = wanted_validation_layers;
    creation_request.enabledLayerCount = NUM_WANTED_VALIDATION_LAYERS;

    creation_request.ppEnabledExtensionNames = vulkan.required_extensions;
    creation_request.enabledExtensionCount = vulkan.num_required_extensions;
    return vkCreateInstance(&creation_request, NULL, &vulkan.instance);
}


void init_list_of_required_extensions() {
    vulkan.num_required_extensions = glfw.num_extensions + 1;
    vulkan.required_extensions = malloc(sizeof(char *) * vulkan.num_required_extensions);
    for (size_t i = 0; i < vulkan.num_required_extensions - 1; i++) {
        vulkan.required_extensions[i] = glfw.extensions[i];
    }
    vulkan.required_extensions[vulkan.num_required_extensions - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
}


bool is_suitable_physical_device(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &features);

    return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader;
}

bool pick_physical_device() {
    uint32_t num_devices = 0;
    if (vkEnumeratePhysicalDevices(vulkan.instance, &num_devices, NULL) != VK_SUCCESS) {
        return false;
    }

    VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * num_devices);
    if (vkEnumeratePhysicalDevices(vulkan.instance, &num_devices, devices) != VK_SUCCESS) {
        free(devices);
        return false;
    }

    bool found = false;
    for (uint32_t i = 0; i < num_devices; i++) {
        if (is_suitable_physical_device(devices[i])) {
            vulkan.physicalDevice = devices[i];
            found = true;
            break;
        }
    }

    free(devices);

    return found;
}

void init_glfw() {
    glfw.extensions = glfwGetRequiredInstanceExtensions(&glfw.num_extensions);
}

bool init_vulkan() {
    init_glfw();

    if (vkEnumerateInstanceExtensionProperties(NULL, &vulkan.num_properties, vulkan.properties) != VK_SUCCESS) {
        printf("Failed to get extension properties\n");
        return false;
    }

    vkEnumerateInstanceLayerProperties(&vulkan.num_layer_properties, NULL);
    vulkan.layer_properties = malloc(sizeof(VkLayerProperties) * vulkan.num_layer_properties);
    if (vkEnumerateInstanceLayerProperties(&vulkan.num_layer_properties, vulkan.layer_properties) != VK_SUCCESS) {
        printf("Failed to get extension properties\n");
        return false;
    }

    if (!has_all_wanted_validation_layers()) {
        printf("Vulkan does not provide all requested validation layers!\n");
        return false;
    }

    init_list_of_required_extensions();

    if (create_vulkan_application_interface() != VK_SUCCESS) {
        printf("Failed to initialize vulkan instance\n");
        return false;
    }

    init_debug_callback();

    if (!pick_physical_device()) {
        printf("Failed to pick a suitable graphics device\n");
        return false;
    }

    printf("Vulkan Initialized\n");
    printf("Loaded Vulkan Extensions: %u\n", vulkan.num_properties);

    printf("Loaded Vulkan Instance Layers: %u\n", vulkan.num_layer_properties);
    for (uint32_t i = 0; i < vulkan.num_layer_properties; i++) {
        printf("\t%d - %s (%d)\n", i, vulkan.layer_properties[i].layerName, vulkan.layer_properties[i].specVersion);
    }
    return true;
}

void cleanup_vulkan() {
    wrap_vulkan_destroy_debug_utils_messenger_ext(vulkan.instance, vulkan.debug_utils_messenger_callback, NULL);
    vkDestroyInstance(vulkan.instance, NULL);
}
