//
// Created by gravypod on 9/20/17.
//

#ifndef ENGINE_SHADER_H
#define ENGINE_SHADER_H

#include "lib/glad/glad.h"

typedef GLuint shader_program;

shader_program make_shader(const char const *vertex_name, const char const *fragment_name);

#endif //ENGINE_SHADER_H
