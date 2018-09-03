#include <GLFW/glfw3.h>
#include <stdio.h>
#include <malloc.h>
#include "query.h"


/**
 * Load hardware and rendering information from GLFW
 */
bool glfw_query(vulkan *v) {
    v->g.extensions = glfwGetRequiredInstanceExtensions(&v->g.num_extensions);
    return true;
}

/**
 * Load available extensions from vulkan
 * @return
 */
bool vulkan_query_extensions(vulkan *v) {
    if (vkEnumerateInstanceExtensionProperties(NULL, &v->extensions.num_properties, NULL) != VK_SUCCESS) {
        return false;
    }

    v->extensions.properties = malloc(sizeof(VkExtensionProperties) * v->extensions.num_properties);

    return vkEnumerateInstanceExtensionProperties(NULL, &v->extensions.num_properties, v->extensions.properties) ==
           VK_SUCCESS;
}

bool vulkan_query_layers(vulkan *v) {
    if (vkEnumerateInstanceLayerProperties(&v->layers.num_properties, NULL) != VK_SUCCESS) {
        return false;
    }

    v->layers.properties = malloc(sizeof(VkLayerProperties) * v->layers.num_properties);

    if (vkEnumerateInstanceLayerProperties(&v->layers.num_properties, v->layers.properties) != VK_SUCCESS) {
        printf("Failed to get extension properties\n");
        return false;
    }

    return true;
}


/**
 * Query all hardware on the system
 * @return
 */
bool vulkan_hardware_query(vulkan *v) {
    return
            glfw_query(v) &&
            vulkan_query_extensions(v) &&
            vulkan_query_layers(v);
}
