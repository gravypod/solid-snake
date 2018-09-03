#ifndef ENGINE_VULKAN_DEBUGGING_H
#define ENGINE_VULKAN_DEBUGGING_H

#include <vulkan/vulkan.h>

/**
 * Wrapper for vkCreateDebugUtilsMessengerEXT extension function
 * @param instance
 * @param pCreateInfo
 * @param pAllocator
 * @param pCallback
 * @return
 */
static inline VkResult wrap_vulkan_create_debug_utils_messenger_ext(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT *pCallback
) {
    const PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
            instance,
            "vkCreateDebugUtilsMessengerEXT"
    );
    if (func != NULL) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

/**
 * Wrapper around vkDestroyDebugUtilsMessengerEXT
 * @param instance
 * @param callback
 * @param pAllocator
 */
static inline void wrap_vulkan_destroy_debug_utils_messenger_ext(
        VkInstance instance,
        VkDebugUtilsMessengerEXT callback,
        const VkAllocationCallbacks *pAllocator
) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
            instance,
            "vkDestroyDebugUtilsMessengerEXT"
    );
    if (func != NULL) {
        func(instance, callback, pAllocator);
    }
}


#endif
