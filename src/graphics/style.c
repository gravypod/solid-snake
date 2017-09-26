//
// Created by gravypod on 9/20/17.
//

#include "style.h"
#include "lib/linmath/linmath.h"
#include "src/util/llist.h"
#include "quadmesh.h"
#include "texture.h"
#include "shader.h"

llist *styles = NULL;


struct style {
    quadmesh m;
    shader_program program;
    size_t num_textures;
    char **texture_names;
};

void make_style(const char const *name,
                float width, float height,
                const char const *vertex_name, const char const *fragment_name,
                const size_t num_textures, const char **texture_names, const char **texture_files)
{
    if (llist_has(&styles, name))
        return;

    struct style s;
    s.program = make_shader(vertex_name, fragment_name);

    s.num_textures = num_textures;
    s.texture_names = NULL;

    if (s.num_textures) {
        // TODO: Create a destructor for llist to delete this pointer
        s.texture_names = malloc(sizeof(char*) * num_textures);
        for (size_t i = 0; i < num_textures; i++) {
            s.texture_names[i] = strdup(texture_names[i]);
            // TODO: Handle failed texture load
            load_texture(texture_names[i], texture_files[i]);
        }
    }

    make_quadmesh(&s.m, width, height);

    llist_add(&styles, name, &s, sizeof(struct style));
}

void draw_style(const char const *name, float x, float y, float rotation)
{
    static bool has_initialized = false;
    static mat4x4 translation, projection;

    const struct style *s = llist_get(&styles, name);
    const quadmesh m = s->m;
    const shader_program  program = s->program;

    if (!has_initialized) {
        mat4x4_ortho(projection, 0, 1, 0, 1, 0, 1);
        has_initialized = true;
    }

    // Move to location in screen space
    mat4x4_translate(translation, x, y, 0);

    if (rotation != 0) {
        // Find center location of the style
        const float center_x = m.width / 2;
        const float center_y = m.height / 2;;

        // Move to the center of the style
        mat4x4_translate_in_place(translation,  center_x,  center_y, 0);

        // Rotate from center of the style
        mat4x4_rotate_Z(translation, translation, rotation);

        // Move back into 0,0 of our style
        mat4x4_translate_in_place(translation, -center_x, -center_y, 0);
    }

    USE_SHADER(program, {
        const GLint translation_matrix_location = glGetUniformLocation(program, "translation");
        const GLint  projection_matrix_location = glGetUniformLocation(program,  "projection");

        glUniformMatrix4fv(translation_matrix_location, 1, GL_FALSE, (GLfloat*) translation);
        glUniformMatrix4fv( projection_matrix_location, 1, GL_FALSE, (GLfloat*) projection);

        // Bind all of our textures to our shader program
        for (size_t i = 0; i < s->num_textures; i++)
        {
            const char    *texture_name = s->texture_names[i];
            const GLint texture_uniform = glGetUniformLocation(s->program, texture_name);
            const GLuint     texture_id = bind_texture(texture_name);
            glUniform1i(texture_uniform, texture_id);
        }

        // TODO: Cache this
        const GLint width  = glGetUniformLocation(s->program, "quad_width");
        const GLint height = glGetUniformLocation(s->program, "quad_height");
        if (width >= 1 && height >= 1) {
            glUniform1f(width, m.width);
            glUniform1f(height, m.height);
        }

        draw_quadmesh(&m);

    });


    // Cleanup our textures
    for (size_t i = 0; i < s->num_textures; i++)
        unbind_texture(s->texture_names[i]);
}