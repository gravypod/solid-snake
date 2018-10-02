#ifndef ENGINE_SWAPCHAIN_H
#define ENGINE_SWAPCHAIN_H

#include "vulkan.h"

bool vulkan_swapchain_init(vulkan *v);

uint32_t vulkan_swapchain_aquire(vulkan *v);

#endif
