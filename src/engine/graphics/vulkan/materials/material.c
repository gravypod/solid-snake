#include <src/engine/util/dict.h>
#include <malloc.h>
#include <src/engine/graphics/vulkan/pipeline.h>
#include <lib/linmath/linmath.h>
#include <memory.h>
#include "material.h"

dict *materials = NULL;

void vulkan_material_group_configure(VkDevice logical_device, shader_group_t *group)
{
	group->num_shaders = 3;
	group->num_vertex_input_binding_descriptions = 1;
	group->num_vertex_input_attribute_descriptions = 1;

	vulkan_shader_group_allocate_space(group);
	{
		// Load all shaders for this group
		group->shaders[0] = vulkan_shader_compile(logical_device, VULKAN_SHADER_VERTEX_TEST);
		group->shaders[1] = vulkan_shader_compile(logical_device, VULKAN_SHADER_GEOMETRY_TEST);
		group->shaders[2] = vulkan_shader_compile(logical_device, VULKAN_SHADER_FRAGMENT_TEST);

		VkPipelineInputAssemblyStateCreateInfo input_assembly_state;
		memset(&input_assembly_state, 0, sizeof(input_assembly_state));
		{
			input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			input_assembly_state.pNext = NULL;
			input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			input_assembly_state.primitiveRestartEnable = VK_FALSE;
			input_assembly_state.flags = 0;
		}

		VkVertexInputAttributeDescription triangle_center_pos_attribute_description;
		memset(&triangle_center_pos_attribute_description, 0, sizeof(triangle_center_pos_attribute_description));
		{
			triangle_center_pos_attribute_description.binding = 0;
			triangle_center_pos_attribute_description.offset = 0;
			triangle_center_pos_attribute_description.format = VK_FORMAT_R32G32_SFLOAT;
			triangle_center_pos_attribute_description.location = 0;
		}

		VkVertexInputBindingDescription triangle_center_pos_binding_description;
		memset(&triangle_center_pos_binding_description, 0, sizeof(triangle_center_pos_binding_description));
		{
			triangle_center_pos_binding_description.binding = 0;
			triangle_center_pos_binding_description.stride = sizeof(vec2);
			triangle_center_pos_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}

		group->input_assembly_state = input_assembly_state;
		group->vertex_input_attribute_descriptions[0] = triangle_center_pos_attribute_description;
		group->vertex_input_binding_descriptions[0] = triangle_center_pos_binding_description;
	}
	vulkan_shader_group_finalize(group);
}

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
		vulkan_material_group_configure(logical_device, &m->group);
		m->pipeline = vulkan_pipeline_graphics_create(
				logical_device,
				vulkan_pipeline_layout_create(logical_device),
				render_pass,
				&m->group
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

