#ifndef ENGINE_GRAPHICS_VULKAN_MATERIALS_MATERIAL_H
#define ENGINE_GRAPHICS_VULKAN_MATERIALS_MATERIAL_H

#include <vulkan/vulkan.h>
#include <src/engine/graphics/vulkan/memory/buffer.h>
#include <src/engine/graphics/vulkan/shaders/shader.h>

typedef struct material_s
{
	VkPipeline pipeline;
	buffer_t verticies;
} material_t;

/**
 * Setup material for rendering
 *
 * @param material
 * @param buffer
 */
void vulkan_material_bind(material_t *material, VkCommandBuffer buffer);

#endif
