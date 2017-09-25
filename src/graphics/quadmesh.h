//
// Created by gravypod on 9/20/17.
//

#ifndef ENGINE_QUADMESH_H
#define ENGINE_QUADMESH_H

#include "lib/glad/glad.h"

typedef struct {
    GLuint VAO, VBO, EBO;
    float width, height;
} quadmesh;

void make_quadmesh(quadmesh *m, float width, float height);
void draw_quadmesh(const quadmesh const *m);

#endif //ENGINE_QUADMESH_H
