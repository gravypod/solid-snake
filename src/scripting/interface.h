//
// Created by gravypod on 9/15/17.
//

#ifndef ENGINE_INTERFACE_H
#define ENGINE_INTERFACE_H

#include <stdbool.h>
#include "duktape.h"

#define REGISTER_SCRIPT_INTERFACE(name, f, nargs) { \
    duk_push_c_function(ctx, f, nargs);             \
    duk_put_global_string(ctx, name);               \
}

duk_context *ctx;

bool init_interface();
bool include_script(const char *filename);

#endif //ENGINE_INTERFACE_H
