#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
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


VkResult create_vulkan_application_interface() {
    VkInstanceCreateInfo creation_request = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &definition
    };
    creation_request.enabledExtensionCount = glfw.num_extensions;
    creation_request.ppEnabledExtensionNames = glfw.extensions;
    creation_request.enabledLayerCount = 0;

    return vkCreateInstance(&creation_request, NULL, &vulkan.instance);
}

void init_glfw() {
    glfw.extensions = glfwGetRequiredInstanceExtensions(&glfw.num_extensions);
}

bool init_vulkan() {
    init_glfw();

    // Load properties
    if (vkEnumerateInstanceExtensionProperties(NULL, &vulkan.num_properties, vulkan.properties) != VK_SUCCESS)  {
        printf("Failed to get extension properties\n");
        return false;
    }

    if (create_vulkan_application_interface() != VK_SUCCESS) {
        printf("Failed to initialize vulkan instance\n");
        return false;
    }

    printf("Vulkan Initialized");
    printf("Loaded Vulkan Extensions: %u\n", vulkan.num_properties);

    return true;
}
