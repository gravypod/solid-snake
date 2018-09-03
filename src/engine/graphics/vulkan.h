
#ifndef ENGINE_VULKAN_H
#define ENGINE_VULKAN_H

#include <vulkan/vulkan.h>
#include <stdbool.h>

typedef struct {
    uint32_t num_extensions;
    const char **extensions;
} glfw_state;

typedef struct {
    VkExtensionProperties *properties;
    uint32_t num_properties;

    VkLayerProperties *layer_properties;
    uint32_t num_layer_properties;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_utils_messenger_callback;

    const char ** required_extensions;
    uint32_t num_required_extensions;

    VkPhysicalDevice physicalDevice;
} vulkan_state;


/**
 * Initialize Vulkan
 * @return
 */
bool init_vulkan();

void cleanup_vulkan();

#endif
