//
// Created by gravypod on 9/20/17.
//

#include "style.h"
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

void draw_style(const char const *name)
{
    struct style *s = llist_get(&styles, name);
    USE_SHADER(s->program, {
       // TODO: Texture and Location
       draw_quadmesh(&s->m);
    });
}