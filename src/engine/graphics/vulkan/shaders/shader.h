#ifndef ENGINE_VULKAN_SHADERS_SHADER_H
#define ENGINE_VULKAN_SHADERS_SHADER_H

#include <shaderc/shaderc.h>
#include "src/engine/graphics/vulkan/vulkan.h"

#define VULKAN_SHADER_FRAGMENT_TEST "assets/shaders/vt.frag"
#define VULKAN_SHADER_VERTEX_TEST "assets/shaders/vt.vert"
#define VULKAN_SHADER_GEOMETRY_TEST "assets/shaders/vt.geom"

bool vulkan_shader_pipeline_shader_stage_get(char *shader_file_name, VkPipelineShaderStageCreateInfo *request);
bool vulkan_shader_init(vulkan *v);

#endif
