#include <memory.h>
#include <src/engine/util/files.h>
#include "shader.h"

shaderc_compiler_t compiler;

void vulkan_shader_compiler_init() {
    compiler = shaderc_compiler_initialize();
}



void vulkan_shader_init() {
    vulkan_shader_compiler_init();

    char * source = read_file("assets/shaders/apple.frag");
    printf("%s\n", source);
    shaderc_compilation_result_t result = shaderc_compile_into_spv(
            compiler,
            source, strlen(source),
            shaderc_fragment_shader,
            "apple.frag",
            "main",
            NULL
    );
    shaderc_compilation_status status = shaderc_result_get_compilation_status(result);
    printf("Shader compiling status: %d\n", status);
    if (status != shaderc_compilation_status_success) {
        printf("Failed to compile shader: %s\n", shaderc_result_get_error_message(result));
    }
    shaderc_result_release(result);
}
