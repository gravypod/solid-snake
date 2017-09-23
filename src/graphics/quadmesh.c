//
// Created by gravypod on 9/20/17.
//

#include "quadmesh.h"



void make_quadmesh(quadmesh *m, float width, float height)
{
    GLuint VAO, VBO, EBO;
    GLfloat verts[] = {
            width, height, 0.0f,
            width,   0.0f, 0.0f,
             0.0f,   0.0f, 0.0f,
             0.0f, height, 0.0f
    };
    GLuint idxs[] = {
            0, 1, 3,
            1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxs), idxs, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m->VAO = VAO;
    m->VBO = VBO;
    m->EBO = EBO;
}

void draw_quadmesh(const quadmesh const *m)
{
    glBindVertexArray(m->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}