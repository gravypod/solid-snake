#ifndef ENGINE_CPOOL_H
#define ENGINE_CPOOL_H

#include "vulkan.h"

VkFence vulkan_command_fence_get(size_t pool_index);

void vulkan_command_pool_render(vulkan *v);

bool vulkan_command_pool_init(vulkan *v);

VkCommandBuffer *vulkan_command_pool_get(size_t pool_index);

#endif
