#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "vulkan.h"

glfw_state glfw;
vulkan_state vulkan;

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

/**
 * Check to make sure all of our desired validation layers can be provided by vulkan
 * @return
 */
bool has_all_wanted_validation_layers()
{
    for (uint32_t wanted_layer_index = 0; wanted_layer_index < NUM_WANTED_VALIDATION_LAYERS; wanted_layer_index++) {
        bool found = false;

        for (uint32_t existing_layer_index = 0; !found && existing_layer_index < vulkan.num_layer_properties; existing_layer_index++) {
            found = strcmp(wanted_validation_layers[wanted_layer_index], vulkan.layer_properties[existing_layer_index].layerName) == 0;
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

    return vkCreateInstance(&creation_request, NULL, &vulkan.instance);
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

    if (create_vulkan_application_interface() != VK_SUCCESS) {
        printf("Failed to initialize vulkan instance\n");
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
    vkDestroyInstance(vulkan.instance, NULL);
}
