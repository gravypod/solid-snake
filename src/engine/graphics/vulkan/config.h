#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#include "vulkan.h"
#include <stdbool.h>

VkPhysicalDevice vulkan_config_pick_physical_device(vulkan *v);
bool vulkan_config_init(vulkan *v);

#endif
