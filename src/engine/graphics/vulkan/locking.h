#ifndef ENGINE_LOCKING_H
#define ENGINE_LOCKING_H

#include "vulkan.h"


bool vulkan_locking_init(vulkan *v);
VkSemaphore vulkan_locking_semphore_get_frame_buffer_image_available();
VkSemaphore vulkan_locking_semphore_get_rendering_finished();

#endif

