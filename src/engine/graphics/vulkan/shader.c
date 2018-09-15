#include <memory.h>
#include <src/engine/util/files.h>
#include "shader.h"

typedef struct {
    char *file_name;
    char *source;
    const uint32_t *binary;

    VkPipelineShaderStageCreateInfo stage_info;
    VkShaderModule module;
    shaderc_compilation_result_t result;
} shader_data;


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

bool vulkan_shader_compile(vulkan *v, char *file_name, shaderc_shader_kind kind) {
    shader_data *resource = malloc(sizeof(shader_data));
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
        return false;
    }

    const char *binary = shaderc_result_get_bytes(resource->result);
    resource->binary = (const uint32_t *) binary;

    VkShaderModuleCreateInfo request = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = shaderc_result_get_length(resource->result),
            .pCode = resource->binary,
            .pNext = NULL
    };

    if (vkCreateShaderModule(v->devices.logical_device, &request, NULL, &resource->module) != VK_SUCCESS) {
        printf("Could not allocate shader on GPU: %s\n", file_name);
        return false;
    }


    // Create pipeline stage info for vulkan configuration down the line
    resource->stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    resource->stage_info.pName = "main";
    resource->stage_info.module = resource->module;
    resource->stage_info.pNext = NULL;
    resource->stage_info.pSpecializationInfo = NULL;
    resource->stage_info.flags = 0;

    if (!vulkan_shader_shaderc_kind_to_stage_bit(kind, &resource->stage_info.stage)) {
        return false;
    }

    // Track shader
    llist_add(&compiled, file_name, resource, sizeof(shader_data));

    return true;
}


bool vulkan_shader_compile_all(vulkan *v) {
    // TODO: Detect and load all shaders
    return vulkan_shader_compile(v, VULKAN_SHADER_FRAGMENT_TEST, shaderc_fragment_shader) &&
           vulkan_shader_compile(v, VULKAN_SHADER_VERTEX_TEST, shaderc_glsl_vertex_shader);
}

bool vulkan_shader_module_get(char *shader_file_name, VkShaderModule *module) {
    shader_data *data = llist_get(&compiled, shader_file_name);
    if (data) {
        *module = data->module;
        return true;
    }
    return NULL;
}

bool vulkan_shader_pipeline_shader_stage_get(char *shader_file_name, VkPipelineShaderStageCreateInfo *request) {
    shader_data *data = llist_get(&compiled, shader_file_name);
    if (data) {
        *request = data->stage_info;
        return true;
    }
    return NULL;
}

bool vulkan_shader_init(vulkan *v) {
    // Init the compiler library
    vulkan_shader_compiler_init();

    // Compile all of the shaders into SPIR-V Bytecode
    if (!vulkan_shader_compile_all(v)) {
        return false;
    }

    return true;
}
