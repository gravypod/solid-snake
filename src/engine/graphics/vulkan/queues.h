#ifndef ENGINE_QUEUES_H
#define ENGINE_QUEUES_H

#include "vulkan.h"

void vulkan_queues_cleanup(vulkan *v);
bool vulkan_queues_init(vulkan *v);

#endif
