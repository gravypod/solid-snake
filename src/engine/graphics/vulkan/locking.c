#include "locking.h"


VkSemaphoreCreateInfo semaphore_create_info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .flags = 0,
        .pNext = NULL
};

VkSemaphore frame_buffer_image_available;
VkSemaphore rendering_finished;

bool vulkan_locking_init(vulkan *v) {
    VkDevice device = v->devices.logical_device;
    return vkCreateSemaphore(device, &semaphore_create_info, NULL, &frame_buffer_image_available) == VK_SUCCESS &&
           vkCreateSemaphore(device, &semaphore_create_info, NULL, &rendering_finished) == VK_SUCCESS;
}

VkSemaphore vulkan_locking_semphore_get_frame_buffer_image_available() {
    return frame_buffer_image_available;
}

VkSemaphore vulkan_locking_semphore_get_rendering_finished() {
    return rendering_finished;
}
