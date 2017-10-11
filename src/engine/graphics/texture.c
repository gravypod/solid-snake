//
// Created by gravypod on 9/25/17.
//

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb/stb_image.h"

#include "src/engine/util/llist.h"
#include "texture.h"

struct texture_unit {
    GLuint unit_id;
    GLuint bound_id;
    bool being_used;
};



#define MAKE_UNIT(name) {.unit_id = (name), .bound_id = 0, .being_used = false}
struct texture_unit units[] = {
        MAKE_UNIT( GL_TEXTURE0), MAKE_UNIT( GL_TEXTURE1), MAKE_UNIT( GL_TEXTURE2), MAKE_UNIT( GL_TEXTURE3),
        MAKE_UNIT( GL_TEXTURE4), MAKE_UNIT( GL_TEXTURE5), MAKE_UNIT( GL_TEXTURE6), MAKE_UNIT( GL_TEXTURE7),
        MAKE_UNIT( GL_TEXTURE8), MAKE_UNIT( GL_TEXTURE9), MAKE_UNIT(GL_TEXTURE10), MAKE_UNIT(GL_TEXTURE11),
        MAKE_UNIT(GL_TEXTURE12), MAKE_UNIT(GL_TEXTURE13), MAKE_UNIT(GL_TEXTURE14), MAKE_UNIT(GL_TEXTURE15),
        MAKE_UNIT(GL_TEXTURE16), MAKE_UNIT(GL_TEXTURE17), MAKE_UNIT(GL_TEXTURE18), MAKE_UNIT(GL_TEXTURE19),
        MAKE_UNIT(GL_TEXTURE20), MAKE_UNIT(GL_TEXTURE21), MAKE_UNIT(GL_TEXTURE22), MAKE_UNIT(GL_TEXTURE23),
        MAKE_UNIT(GL_TEXTURE24), MAKE_UNIT(GL_TEXTURE25), MAKE_UNIT(GL_TEXTURE26), MAKE_UNIT(GL_TEXTURE27),
        MAKE_UNIT(GL_TEXTURE28), MAKE_UNIT(GL_TEXTURE29), MAKE_UNIT(GL_TEXTURE30), MAKE_UNIT(GL_TEXTURE31)
};
const int NUM_UNITS = (sizeof(units) / sizeof(struct texture_unit));

llist *textures = NULL;

int get_texture_unit_id(const char *name)
{
    GLuint texture_id = *((GLuint *) llist_get(&textures, name));
    for (size_t i = 0; i < (size_t) NUM_UNITS; i++)
        if (units[i].bound_id == texture_id)
            return (int) i;
    return -1;
}

// TODO: Test this logic
int load_texture_unit(const char *name)
{
    GLuint texture_id = *((GLuint *) llist_get(&textures, name));


    int first_unused_texture = -1;
    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[i].being_used)
            continue;
        first_unused_texture = i;
        break;
    }

    glActiveTexture(units[first_unused_texture].unit_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    units[first_unused_texture].bound_id = texture_id;

    return first_unused_texture;
}

GLuint bind_texture(const char *name)
{
    int bound_unit_idx = get_texture_unit_id(name);

    // Swap an unused texture out if the texture we want is not loaded
    if (bound_unit_idx == -1)
        bound_unit_idx = load_texture_unit(name);

    // Mark our texture as in use
    units[bound_unit_idx].being_used = true;

    // Return our unit id
    return units[bound_unit_idx].unit_id;
}

void unbind_texture(const char *name)
{
    int bound_unit_idx = get_texture_unit_id(name);
    units[bound_unit_idx].being_used = false;
}

bool load_texture(const char *name, const char *file)
{
    int width, height, num_channels;
    unsigned char *data = stbi_load(file, &width, &height, &num_channels, 0);

    if (data)
    {

        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);

        llist_add(&textures, name, &texture_id, sizeof(texture_id));
    }

    stbi_image_free(data);

    return data != 0;
}