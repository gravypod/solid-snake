//
// Created by gravypod on 9/25/17.
//

#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

#include "lib/glad/glad.h"

GLuint bind_texture(const char *name);
void unbind_texture(const char *name);
bool load_texture(const char *name, const char *file);

#endif //ENGINE_TEXTURE_H
