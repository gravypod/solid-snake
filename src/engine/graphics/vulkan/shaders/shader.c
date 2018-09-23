#include <memory.h>
#include <src/engine/util/files.h>
#include <src/engine/util/dict.h>
#include "shader.h"

dict *compiled_shader_map;
llist *compiled;
shaderc_compiler_t compiler;

void vulkan_shader_compiler_init() {
	compiler = shaderc_compiler_initialize();
}

static inline bool vulkan_shader_shaderc_kind_to_stage_bit(shaderc_shader_kind kind, VkShaderStageFlagBits *bits) {
	if (kind == shaderc_glsl_vertex_shader) {
		*bits = VK_SHADER_STAGE_VERTEX_BIT;
		return true;
	}

	if (kind == shaderc_glsl_fragment_shader) {
		*bits = VK_SHADER_STAGE_FRAGMENT_BIT;
		return true;
	}

	if (kind == shaderc_glsl_geometry_shader) {
		*bits = VK_SHADER_STAGE_GEOMETRY_BIT;
		return true;
	}

	printf("Compiled unknown shaderc type\n");
	return false;
}

/**
 * Find the shader kind from the file name
 *
 * @param file_name
 * @return the shader kind or shaderc_glsl_infer_from_source if we could not determine from the
 */
static inline shaderc_shader_kind vulkan_shader_shaderc_detect_kind(char *file_name) {
#define NUM_SHADER_KINDS 3

	static const char *extensions[NUM_SHADER_KINDS] = {
			"vert",
			"frag",
			"geom"
	};
	static const shaderc_shader_kind kinds[NUM_SHADER_KINDS] = {
			shaderc_glsl_vertex_shader,
			shaderc_glsl_fragment_shader,
			shaderc_glsl_geometry_shader
	};

	const char *file_extension = file_extract_extension(file_name);

	for (size_t i = 0; i < NUM_SHADER_KINDS; i++) {
		if (strcmp(extensions[i], file_extension) == 0) {
			return kinds[i];
		}
	}

	return shaderc_glsl_infer_from_source;
}

shader_t *vulkan_shader_compile(VkDevice device, char *file_name) {
	shader_t *resource;

	// Is this shader compiled already?
	if (dict_get(compiled_shader_map, file_name, &resource, NULL)) {
		return resource;
	}

	shaderc_shader_kind kind = vulkan_shader_shaderc_detect_kind(file_name);

	resource = malloc(sizeof(shader_t));
	resource->device = device;
	resource->file_name = strdup(file_name);
	resource->source = read_file(file_name);
	resource->result = shaderc_compile_into_spv(
			compiler,
			resource->source, strlen(resource->source),
			kind,
			file_name,
			"main", // TODO: Configurable shaders?
			NULL // TODO: Add preprocessor directives
	);


	// Was the compile successful?
	if (shaderc_result_get_compilation_status(resource->result) != shaderc_compilation_status_success) {
		printf("Failed to compile shader (%s): %s\n", file_name, shaderc_result_get_error_message(resource->result));
		return NULL;
	}

	const char *binary = shaderc_result_get_bytes(resource->result);
	resource->binary = (const uint32_t *) binary;

	VkShaderModuleCreateInfo request = {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = shaderc_result_get_length(resource->result),
			.pCode = resource->binary,
			.pNext = NULL
	};

	if (vkCreateShaderModule(device, &request, NULL, &resource->module) != VK_SUCCESS) {
		printf("Could not allocate shader on GPU: %s\n", file_name);
		return NULL;
	}


	// Create pipeline stage info for vulkan configuration down the line
	resource->stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	resource->stage_info.pName = "main";
	resource->stage_info.module = resource->module;
	resource->stage_info.pNext = NULL;
	resource->stage_info.pSpecializationInfo = NULL;
	resource->stage_info.flags = 0;

	if (!vulkan_shader_shaderc_kind_to_stage_bit(kind, &resource->stage_info.stage)) {
		return NULL;
	}

	// Track shader
	llist_add(&compiled, file_name, resource, sizeof(shader_t));

	return resource;
}


bool vulkan_shader_init() {
	compiled_shader_map = dict_new();

	// Init the compiler library
	vulkan_shader_compiler_init();

	return true;
}
