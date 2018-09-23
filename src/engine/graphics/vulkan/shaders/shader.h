#ifndef ENGINE_VULKAN_SHADERS_SHADER_H
#define ENGINE_VULKAN_SHADERS_SHADER_H

#include <shaderc/shaderc.h>
#include "src/engine/graphics/vulkan/vulkan.h"

#define VULKAN_SHADER_FRAGMENT_TEST "assets/shaders/vt.frag"
#define VULKAN_SHADER_VERTEX_TEST "assets/shaders/vt.vert"
#define VULKAN_SHADER_GEOMETRY_TEST "assets/shaders/vt.geom"

typedef struct {
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
