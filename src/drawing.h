//
// Created by gravypod on 9/15/17.
//

#ifndef ENGINE_DRAWING_H
#define ENGINE_DRAWING_H

#include "src/scripting/duktape.h"

void cube(float x, float y, int food);

// Interface layer between javascript and C cube
static duk_ret_t s_cube(duk_context *ctx)
{
    float    x = (float) duk_require_number(ctx, 0);
    float    y = (float) duk_require_number(ctx, 1);
    int   food = (int)  duk_require_boolean(ctx, 2);
    //printf("Cube called\n");
    cube(x, y, food);
    return 1;
}

#endif //ENGINE_DRAWING_H
