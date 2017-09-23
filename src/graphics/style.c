//
// Created by gravypod on 9/20/17.
//

#include "style.h"
#include "lib/linmath/linmath.h"
#include "src/util/llist.h"
#include "quadmesh.h"
#include "shader.h"

llist *styles = NULL;

struct style {
    quadmesh m;
    shader_program program;
};

void make_style(const char const *name,
                float width, float height,
                const char const *vertex_name, const char const *fragment_name)
{
    if (llist_has(&styles, name))
        return;

    struct style s;
    s.program = make_shader(vertex_name, fragment_name);
    make_quadmesh(&s.m, width, height);

    llist_add(&styles, name, &s, sizeof(struct style));
}

void draw_style(const char const *name, float x, float y)
{
    static mat4x4 translation;
    static mat4x4 move, ortho;
    //static mat4x4 perspetcive;
    //mat4x4_identity(translation);
    mat4x4_ortho(ortho, 0, 1, 0, 1, 0, 1);
    mat4x4_translate(move, x, y, 0);
    mat4x4_mul(translation, ortho, move);
    //mat4x4_perspective()

    struct style *s = llist_get(&styles, name);
    USE_SHADER(s->program, {
        GLint translation_matrix_location = glGetUniformLocation(s->program, "translation");
        glUniformMatrix4fv(translation_matrix_location, 1, GL_FALSE, (GLfloat*) translation);
        // TODO: Texture
       draw_quadmesh(&s->m);
    });
}