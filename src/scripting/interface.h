//
// Created by gravypod on 9/15/17.
//

#ifndef ENGINE_INTERFACE_H
#define ENGINE_INTERFACE_H

#include <stdbool.h>
#include "lib/duktape/duktape.h"

/**
 * Register a native function into the duk context.
 */
#define REGISTER_SCRIPT_INTERFACE(name, f, nargs) { \
    (void) f;                                       \
    duk_push_c_function(ctx, f, nargs);             \
    duk_put_global_string(ctx, name);               \
}

/**
 * Shared duk context.
 */
duk_context *ctx;

/**
 * Init the script interface
 * @return  - True if init completed correctly.
 */
bool init_interface();

/**
 * Include a script into the duk runtime
 * @param filename - The file path to import
 * @return - True if the file was read and was eval'd correctly.
 */
bool include_script(const char *filename);

/**
 * Native interface to include_script function
 */
static duk_ret_t native_include(duk_context *ctx)
{
    const char *file_name = duk_require_string(ctx, 0);

    if (include_script(file_name))
        duk_push_boolean(ctx, true);
    else
        duk_push_boolean(ctx, false);

    return 1;
}

static duk_ret_t native_print(duk_context *ctx)
{
    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    printf("%s\n", duk_safe_to_string(ctx, -1));
    return 0;
}



#endif //ENGINE_INTERFACE_H
