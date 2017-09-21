//
// Created by gravypod on 9/20/17.
//

#ifndef ENGINE_STYLE_H
#define ENGINE_STYLE_H

#include <duktape.h>

void make_style(const char const *name,
                float width, float height,
                const char const *vertex_name, const char const *fragment_name);
void draw_style(const char const *name);

static duk_ret_t native_make_style(duk_context *ctx)
{
    const char const          *name = duk_require_string(ctx, 0);
    const float               width = (float) duk_require_number(ctx, 1);
    const float              height = (float) duk_require_number(ctx, 2);
    const char const   *vertex_name = duk_require_string(ctx, 3);
    const char const *fragment_name = duk_require_string(ctx, 4);
    make_style(name, width, height, vertex_name, fragment_name);
    return 0;
}

static duk_ret_t native_draw_style(duk_context *ctx)
{
    const char const          *name = duk_require_string(ctx, 0);
    draw_style(name);
    return 0;
}

#endif //ENGINE_STYLE_H
