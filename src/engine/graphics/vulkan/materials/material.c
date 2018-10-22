#include <src/engine/util/dict.h>
#include <malloc.h>
#include <src/engine/graphics/vulkan/pipeline.h>
#include "material.h"

dict *materials = NULL;

/**
 * Load a material to be rendered
 * @param render_pass
 * @param logical_device
 * @param name
 * @return
 */
material_t *vulkan_material_load(VkRenderPass render_pass, VkDevice logical_device, const char *const name)
{
	if (!materials) {
		materials = dict_new();
	}

	material_t *m;

	if (!dict_has(materials, name)) {
		m = calloc(sizeof(material_t), 1);
		// TODO: Load shaders based on name of material
		m->shaders = vulkan_shader_group_create(logical_device);
		m->pipeline = vulkan_pipeline_graphics_create(
				logical_device,
				vulkan_pipeline_layout_create(logical_device),
				render_pass,
				m->shaders
		);
		dict_set(materials, name, &m, sizeof(material_t *));
	} else {
		dict_get(materials, name, &m, NULL);
	}

	return m;
}

void vulkan_material_bind(VkCommandBuffer buffer, const material_t *const m)
{
	// TODO: Bind pipeline in a better way
	vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m->pipeline);
}

