#include <lib/linmath/linmath.h>
#include <src/engine/graphics/vulkan/memory/buffer.h>
#include <src/engine/graphics/vulkan/window.h>
#include <memory.h>
#include <src/engine/graphics/vulkan/memory/vbuffer.h>
#include <stdio.h>
#include <src/engine/graphics/vulkan/commands/pool.h>
#include <src/engine/graphics/vulkan/commands/buffer.h>
#include <src/engine/graphics/vulkan/memory/ibuffer.h>
#include "triangle.h"

#define NUM_TRIANGLE_MODEL_POSITIONS 4
vec2 triangle_model_positions[NUM_TRIANGLE_MODEL_POSITIONS] = {
		{-0.5f, -0.5f},
		{0.5f, -0.5f},
		{0.5f, 0.5f},
		{-0.5f, 0.5f}
};

#define NUM_TRIANGLE_MODEL_IDXS 6
const uint16_t indices[NUM_TRIANGLE_MODEL_IDXS] = {
		0, 1, 2, 2, 3, 0
};

buffer_t triangle_model_index_buffer, triangle_model_index_staging_buffer;
buffer_t triangle_model_buffer, triangle_model_staging_buffer;

void entity_triangle_model_indexes_upload()
{
	memcpy(triangle_model_index_staging_buffer.mapped_memory, indices, sizeof(uint16_t) * NUM_TRIANGLE_MODEL_IDXS);

	uint32_t cbuffer_id = vulkan_commands_cpool_cbuffer_id_take(cpool);
	vulkan_commands_buffer_begin(cpool, cbuffer_id);
	{
		vulkan_memory_buffer_copy(&triangle_model_index_staging_buffer, &triangle_model_index_buffer, vulkan_commands_cpool_cbuffer_get(cpool, cbuffer_id));
	}
	vulkan_commands_buffer_end(
			cpool, cbuffer_id,
			0, NULL, NULL,
			0, NULL
	);
}

void entity_triangle_model_upload()
{
	memcpy(triangle_model_staging_buffer.mapped_memory, triangle_model_positions, sizeof(vec2) * NUM_TRIANGLE_MODEL_POSITIONS);

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
	vkCmdBindIndexBuffer(buffer, triangle_model_index_buffer.buffer, 0, VK_INDEX_TYPE_UINT16);

	vkCmdDrawIndexed(buffer, NUM_TRIANGLE_MODEL_IDXS, 1, 0, 0, 0);
}


void entity_triangle_init(entity_t *entity, vulkan *v)
{
	// TODO: Make move this into the entity definition
	// TODO: Configurable "agumon"?
	entity->material = vulkan_material_load(v->pipelines.render_pass, v->devices.logical_device, "vt");

	if (!vulkan_memory_ibuffer_allocate(v, &triangle_model_index_buffer, sizeof(uint16_t), NUM_TRIANGLE_MODEL_IDXS, false)) {
		printf("Failed allocate index buffer\n");
	}

	if (!vulkan_memory_ibuffer_allocate(v, &triangle_model_index_staging_buffer, sizeof(uint16_t), NUM_TRIANGLE_MODEL_IDXS, true)) {
		printf("Failed allocate index staging buffer\n");
	}

	// TODO: Do not allocate this here. This is game state logic
	if (!vulkan_memory_vbuffer_allocate(v, &triangle_model_buffer, sizeof(vec2), NUM_TRIANGLE_MODEL_POSITIONS, false)) {
		printf("Failed allocate buffer\n");
	}

	if (!vulkan_memory_vbuffer_allocate(v, &triangle_model_staging_buffer, sizeof(vec2), NUM_TRIANGLE_MODEL_POSITIONS, true)) {
		printf("Failed allocate buffer\n");
	}

	entity->update = (void (*)(struct entity_struct *)) entity_triangle_update;
	entity->draw = (void (*)(struct entity_struct *, VkCommandBuffer)) (void (*)(struct entity_struct *)) entity_triangle_draw;
	entity->free = (void (*)(struct entity_struct *)) entity_triangle_free;

	// Send model to GPU
	entity_triangle_model_indexes_upload();
	entity_triangle_model_upload();
}
