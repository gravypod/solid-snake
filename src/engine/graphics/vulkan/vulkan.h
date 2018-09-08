#ifndef ENGINE_VGMODULE_H
#define ENGINE_VGMODULE_H

#define GLFW_INCLUDE_VULKAN 1

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <vulkan/vulkan.h>

typedef struct {
    uint32_t num_extensions;
    const char **extensions;
    GLFWwindow *window;
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
    VkDevice  logical_device;
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
    uint32_t num_properties;
    VkQueueFamilyProperties *properties;

    uint32_t  num_queue_families;
    uint32_t queue_families[2];

    uint32_t main_rendering_queue_id;
    uint32_t main_presentation_queue_id;
    VkQueue rendering;
    VkQueue presenting;
} vulkan_queues;

typedef struct {
    uint32_t num_extensions;
    const char **extensions;
    uint32_t num_logical_extensions;
    const char **logical_extensions;
    uint32_t num_layers;
    const char **layers;
} vulkan_required_configuration;

typedef struct {
    VkSwapchainKHR swapchain;
    VkSurfaceCapabilitiesKHR capabilities;

    VkSurfaceFormatKHR *formats;
    uint32_t num_formats;

    VkPresentModeKHR  *modes;
    uint32_t num_modes;
} vulkan_swapchain;

typedef struct {
    VkApplicationInfo definition;

    glfw g;
    VkInstance instance;
    VkSurfaceKHR surface;

    vulkan_layers layers;
    vulkan_extensions extensions;
    vulkan_debugging debugging;
    vulkan_devices devices;
    vulkan_queues queues;
    vulkan_swapchain swapchain;

    vulkan_required_configuration required_configuration;
} vulkan;


bool vulkan_init();
void vulkan_update();
void vulkan_cleanup();

#endif
