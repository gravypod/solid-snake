#ifndef ENGINE_VGMODULE_H
#define ENGINE_VGMODULE_H

#include <stdbool.h>
#include <vulkan/vulkan.h>

typedef struct {
    uint32_t num_extensions;
    const char **extensions;
} glfw;


typedef struct {
    VkDebugUtilsMessengerEXT debug_utils_messenger_callback;
} vulkan_debugging;


typedef struct {

    /**
     * Info about available devices
     */
    VkPhysicalDevice *devices;
    uint32_t num_devices;

    /**
     * The device we are currently rendering to
     */
    VkPhysicalDevice selected_device;
} vulkan_devices;

typedef struct {
    VkExtensionProperties *properties;
    uint32_t num_properties;
} vulkan_extensions;

typedef struct {
    VkLayerProperties *properties;
    uint32_t num_properties;

    const char **layers;
    uint32_t num_layers;
} vulkan_layers;

typedef struct {
    uint32_t num_extensions;
    const char **extensions;
    uint32_t num_layers;
    const char **layers;
} vulkan_required_configuration;

typedef struct {
    VkApplicationInfo definition;

    glfw g;
    VkInstance instance;

    vulkan_layers layers;
    vulkan_extensions extensions;
    vulkan_debugging debugging;
    vulkan_devices devices;

    vulkan_required_configuration required_configuration;
} vulkan;


bool vulkan_init();


#endif
