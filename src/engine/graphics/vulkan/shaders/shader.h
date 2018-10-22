#ifndef ENGINE_VULKAN_SHADERS_SHADER_H
#define ENGINE_VULKAN_SHADERS_SHADER_H

#include <shaderc/shaderc.h>
#include "src/engine/graphics/vulkan/vulkan.h"

#define VULKAN_SHADER_FRAGMENT_TEST "assets/shaders/vt.frag"
#define VULKAN_SHADER_VERTEX_TEST "assets/shaders/vt.vert"
#define VULKAN_SHADER_GEOMETRY_TEST "assets/shaders/vt.geom"

typedef struct
{
	/**
	 * Name of the file this shadr source came from
	 */
	char *file_name;

	/**
	 * Source code of the shader
	 */
	char *source;

	/**
	 * SPIR-V opcodes
	 */
	const uint32_t *binary;

	/**
	 * Device this shader was compiled onto
	 */
	VkDevice device;

	/**
	 * Shader stage configuration for pipelines
	 */
	VkPipelineShaderStageCreateInfo stage_info;
	VkShaderModule module;

	/**
	 * Output of the shaderc compiler
	 */
	shaderc_compilation_result_t result;
} shader_t;

typedef struct
{
	VkPipelineInputAssemblyStateCreateInfo input_assembly_state;

	// Vertex Attribute Descriptions
	uint32_t num_vertex_input_attribute_descriptions;
	VkVertexInputAttributeDescription *vertex_input_attribute_descriptions;

	// Vertex Binding Descriptions
	uint32_t num_vertex_input_binding_descriptions;
	VkVertexInputBindingDescription *vertex_input_binding_descriptions;

	uint32_t num_shaders;
	shader_t **shaders;

	VkPipelineShaderStageCreateInfo *stages;

	VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info;
} shader_group_t;

/**
 * Allocate space for all fields in shader_group_t based on the size fields of the struct.
 * @param group
 */
void vulkan_shader_group_allocate_space(shader_group_t *group);

/**
 * Build final structures from configurable shader_group_t fields
 * @param group
 */
void vulkan_shader_group_finalize(shader_group_t *group);

/**
 * Compile a shader.
 *
 * @param device - Device to compile onto
 * @param file_name - The file name of the shader
 * @return
 */
shader_t *vulkan_shader_compile(VkDevice device, char *file_name);

bool vulkan_shader_init();

#endif
