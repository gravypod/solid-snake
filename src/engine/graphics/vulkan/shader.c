#include "shader.h"

shaderc_compiler_t compiler;

void vulkan_shader_compiler_init()
{
    compiler = shaderc_compiler_initialize();
}

void vulkan_shader_init()
{
    vulkan_shader_compiler_init();
}
