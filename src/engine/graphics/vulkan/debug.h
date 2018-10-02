#ifndef ENGINE_HDEBUG_H
#define ENGINE_HDEBUG_H

#include <stdbool.h>
#include "vulkan.h"

/**
 * Check if all validation layers can be provided by vulkan on this system
 * @param v
 * @return
 */
bool vulkan_debug_has_all_wanted_validation_layers(vulkan *v);

/**
 * Set debugger callbacks into vulkan instance
 * @param v
 */
void vulkan_debug_init(vulkan *v);

uint32_t vulkan_debug_num_validation_layers();

const char **vulkan_debug_validation_layers();

#endif
