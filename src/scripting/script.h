//
// Created by gravypod on 9/15/17.
//

#ifndef ENGINE_SCRIPT_H
#define ENGINE_SCRIPT_H

#include "interface.h"

// Call script game.update()
static void s_update(const double delta)
{
    duk_get_global_string(ctx, "game");
    duk_push_string(ctx, "update");
    duk_push_number(ctx, delta);
    duk_pcall_prop(ctx, -3, 1 /*nargs*/);
    duk_pop_2(ctx);

}

// Call script game.keypress()
static void s_keypress(const unsigned char key, const bool pressed)
{

    duk_get_global_string(ctx, "game");
    duk_push_string(ctx, "keypress");
    duk_push_number(ctx, key);
    duk_push_boolean(ctx, pressed);
    duk_pcall_prop(ctx, -4, 2 /*nargs*/);
    duk_pop_2(ctx);
}

// Call script game.render();
static void s_draw()
{
    duk_get_global_string(ctx, "game");
    duk_push_string(ctx, "render");
    duk_pcall_prop(ctx, -2, 0 /*nargs*/);
    duk_pop_2(ctx);
}

// Call script game.init()
static void s_init()
{
    duk_get_global_string(ctx, "game");
    duk_push_string(ctx, "init");
    duk_pcall_prop(ctx, -2, 0 /*nargs*/);
    duk_pop_2(ctx);
}

#endif //ENGINE_SCRIPT_H
