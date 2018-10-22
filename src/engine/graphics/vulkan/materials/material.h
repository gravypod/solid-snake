#ifndef ENGINE_GRAPHICS_VULKAN_MATERIALS_MATERIAL_H
#define ENGINE_GRAPHICS_VULKAN_MATERIALS_MATERIAL_H

#include <vulkan/vulkan.h>
#include <src/engine/graphics/vulkan/memory/buffer.h>
#include <src/engine/graphics/vulkan/shaders/shader.h>

typedef struct material_s
{
	VkPipeline pipeline;
	shader_group_t *shaders;
} material_t;

void vulkan_material_init();

void vulkan_material_bind(VkCommandBuffer buffer, const material_t * m);

material_t *vulkan_material_load(VkRenderPass render_pass, VkDevice logical_device, const char * name);

#endif

