
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
} vulkan_state;


/**
 * Initialize Vulkan
 * @return
 */
bool init_vulkan();

void cleanup_vulkan();

#endif
