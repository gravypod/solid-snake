#include <lib/linmath/linmath.h>
#include <src/engine/graphics/vulkan/memory/buffer.h>
#include <src/engine/graphics/vulkan/window.h>
#include <memory.h>
#include <src/engine/graphics/vulkan/shaders/vbuffer.h>
#include <stdio.h>
#include <src/engine/graphics/vulkan/commandpool.h>
#include "triangle.h"


vec2 triangle_center_position;
buffer_t triangle_position_buffer;


void entity_triangle_update(entity_t *entity, void *metadata)
{
	(void) entity;
	(void) metadata;
	/**
	 * set triangle position
	 */
	triangle_center_position[0] = (cursor_x - (vulkan_window_width_get() / 2.0f)) / vulkan_window_width_get();
	triangle_center_position[1] = (cursor_y - (vulkan_window_height_get() / 2.0f)) / vulkan_window_height_get();

	// send to gpu via memory mapped region
	memcpy(triangle_position_buffer.mapped_memory, &triangle_center_position, sizeof(vec2) * 1);
}

void entity_triangle_free(entity_t *entity, void *metadata)
{
	(void) entity;
	(void) metadata;
}

void entity_triangle_draw(entity_t *e, VkCommandBuffer buffer)
{
	(void) e;

	VkBuffer vertexBuffers[] = {triangle_position_buffer.buffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);

	vkCmdDraw(buffer, (uint32_t) triangle_position_buffer.num_elements, 1, 0, 0);
}


void entity_triangle_init(entity_t *entity, vulkan *v)
{
	// TODO: Do not allocate this here. This is game state logic
	if (!vulkan_vbuffer_allocate(v, &triangle_position_buffer, sizeof(vec2), 1, true)) {
		printf("Failed allocate buffer\n");
	}

	entity->update = (void (*)(struct entity_struct *)) entity_triangle_update;
	entity->draw = (void (*)(struct entity_struct *, VkCommandBuffer)) (void (*)(struct entity_struct *)) entity_triangle_draw;
	entity->free = (void (*)(struct entity_struct *)) entity_triangle_free;
}
