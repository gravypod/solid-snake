#ifndef ENGINE_TRIANGLE_H
#define ENGINE_TRIANGLE_H


#include <src/engine/entity/entity.h>
#include <vulkan/vulkan.h>
#include <src/engine/graphics/vulkan/vulkan.h>

void entity_triangle_init(entity_t *entity, vulkan *v);

void entity_triangle_update(entity_t *entity, void *metadata);

void entity_triangle_free(entity_t *entity, void *metadata);

void entity_triangle_draw(entity_t *e, VkCommandBuffer buffer);

#endif
