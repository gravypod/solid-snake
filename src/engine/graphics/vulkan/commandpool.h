#ifndef ENGINE_CPOOL_H
#define ENGINE_CPOOL_H

#include "vulkan.h"

bool vulkan_command_pool_init(vulkan *v, vbuffer *buffer);

VkCommandBuffer *vulkan_command_pool_get(size_t pool_index);

#endif
