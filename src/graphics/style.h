//
// Created by gravypod on 9/20/17.
//

#ifndef ENGINE_STYLE_H
#define ENGINE_STYLE_H

#include "lib/duktape/duktape.h"

void make_style(const char const *name,
                float width, float height,
                const char const *vertex_name, const char const *fragment_name,
                const size_t num_textures, const char **texture_names, const char **texture_files);
void draw_style(const char const *name, float x, float y);

__attribute__((unused)) static duk_ret_t native_make_style(duk_context *ctx)
{
    // Can't have more then 32 textures in a single style
    static char *(names[32]), *(files[32]);

    const char const          *name = duk_require_string(ctx, 0);
    const float               width = (float) duk_require_number(ctx, 1);
    const float              height = (float) duk_require_number(ctx, 2);
    const char const   *vertex_name = duk_require_string(ctx, 3);
    const char const *fragment_name = duk_require_string(ctx, 4);
    const size_t       num_textures = (size_t) duk_require_number(ctx, 5);

    if (num_textures)
    {
        for (size_t i = 0; i < num_textures; i++)
        {
            duk_get_prop_index(ctx, 6, (duk_uarridx_t) i); // TODO: Handle missing props
            duk_get_prop_string(ctx, -1, "name");
            names[i] = strdup(duk_to_string(ctx, -1));
            duk_pop_2(ctx);

            duk_get_prop_index(ctx, 6, (duk_uarridx_t) i); // TODO: Handle missing props
            duk_get_prop_string(ctx, -1, "file");
            files[i] = strdup(duk_to_string(ctx, -1));
            duk_pop_2(ctx);

            printf("ASSETS-Textures: %s -> %s\n", names[i], files[i]);
        }
    }

    make_style(name,
               width, height,
               vertex_name, fragment_name,
               num_textures, (const char **) names, (const char **) files);

    for (size_t i = 0; i < num_textures; i++)
    {
        free(names[i]);
        free(files[i]);
    }

    return 0;
}

__attribute__((unused)) static duk_ret_t native_draw_style(duk_context *ctx)
{
    const char const          *name = duk_require_string(ctx, 0);
    const float                   x = (float) duk_require_number(ctx, 1);
    const float                   y = (float) duk_require_number(ctx, 2);
    draw_style(name, x, y);
    return 0;
}

#endif //ENGINE_STYLE_H
