#ifndef ENGINE_HQUERY_H
#define ENGINE_HQUERY_H

#include <src/engine/graphics/vulkan/vulkan.h>
#include <stdbool.h>

/**
 * Query all hardware from vulkan system
 * @param v
 * @return
 */
bool vulkan_hardware_query(vulkan *v);

#endif
