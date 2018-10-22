#include <lib/linmath/linmath.h>
#include <src/engine/graphics/vulkan/memory/buffer.h>
#include <src/engine/graphics/vulkan/window.h>
#include <memory.h>
#include <src/engine/graphics/vulkan/memory/vbuffer.h>
#include <stdio.h>
#include <src/engine/graphics/vulkan/commands/pool.h>
#include <src/engine/graphics/vulkan/commands/buffer.h>
#include "triangle.h"

#define NUM_TRIANGLE_MODEL_POSITIONS 3
vec2 triangle_model_positions[NUM_TRIANGLE_MODEL_POSITIONS] = {
		{0.0f,  -0.5f},
		{0.5f,  0.5f},
		{-0.5f, 0.5f},
};

vec2 *triangle_mapped_buffer;
buffer_t triangle_model_buffer, triangle_model_staging_buffer;

void entity_triangle_model_upload() {
	memcpy(triangle_mapped_buffer, triangle_model_positions, sizeof(vec2) * NUM_TRIANGLE_MODEL_POSITIONS);

	uint32_t cbuffer_id = vulkan_commands_cpool_cbuffer_id_take(cpool);
	vulkan_commands_buffer_begin(cpool, cbuffer_id);
	{
		vulkan_memory_buffer_copy(&triangle_model_staging_buffer, &triangle_model_buffer, vulkan_commands_cpool_cbuffer_get(cpool, cbuffer_id));
	}
	vulkan_commands_buffer_end(
			cpool, cbuffer_id,
			0, NULL, NULL,
			0, NULL
	);
}

void entity_triangle_update(entity_t *entity, void *metadata)
{
	(void) entity;
	(void) metadata;
}

void entity_triangle_free(entity_t *entity, void *metadata)
{
	(void) entity;
	(void) metadata;
}

void entity_triangle_draw(entity_t *e, VkCommandBuffer buffer)
{
	(void) e;

	VkBuffer vertexBuffers[] = {triangle_model_buffer.buffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);

	vkCmdDraw(buffer, (uint32_t) triangle_model_buffer.num_elements, 1, 0, 0);
}


void entity_triangle_init(entity_t *entity, vulkan *v)
{
	// TODO: Make move this into the entity definition
	// TODO: Configurable "agumon"?
	entity->material = vulkan_material_load(v->pipelines.render_pass, v->devices.logical_device, "vt");

	// TODO: Do not allocate this here. This is game state logic
	if (!vulkan_memory_vbuffer_allocate(v, &triangle_model_buffer, sizeof(vec2), NUM_TRIANGLE_MODEL_POSITIONS, false)) {
		printf("Failed allocate buffer\n");
	}

	if (!vulkan_memory_vbuffer_allocate(v, &triangle_model_staging_buffer, sizeof(vec2), NUM_TRIANGLE_MODEL_POSITIONS, true)) {
		printf("Failed allocate buffer\n");
	}

	triangle_mapped_buffer = triangle_model_staging_buffer.mapped_memory;

	entity->update = (void (*)(struct entity_struct *)) entity_triangle_update;
	entity->draw = (void (*)(struct entity_struct *, VkCommandBuffer)) (void (*)(struct entity_struct *)) entity_triangle_draw;
	entity->free = (void (*)(struct entity_struct *)) entity_triangle_free;

	entity_triangle_model_upload();
}
