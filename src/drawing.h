//
// Created by gravypod on 9/15/17.
//

#ifndef ENGINE_DRAWING_H
#define ENGINE_DRAWING_H

#include "src/scripting/duktape.h"

void cube(float x, float y, float width, float height, int food);

// Interface layer between javascript and C cube
static duk_ret_t s_cube(duk_context *ctx)
{
    float      x = (float) duk_require_number(ctx, 0);
    float      y = (float) duk_require_number(ctx, 1);
    float  width = (float) duk_require_number(ctx, 2);
    float height = (float) duk_require_number(ctx, 3);
    int     food = (int)  duk_require_boolean(ctx, 4);
    //printf("Cube called\n");
    cube(x, y, width, height, food);
    return 1;
}

#endif //ENGINE_DRAWING_H
