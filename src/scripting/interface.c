//
// Created by gravypod on 9/15/17.
//

#include "interface.h"
#include <sys/stat.h>

/* Native functions */

off_t fsize(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}


bool include_script(const char *filename)
{
    const size_t file_size = (size_t) fsize(filename);
    FILE *f;

    if (file_size == -1 || !(f = fopen(filename, "rb"))) {
        duk_push_undefined(ctx);
        return false;
    }

    size_t data_left = file_size;
    char *buffer = malloc(file_size + 1);
    char *tmp    = buffer;

    while (data_left > 0)
    {
        const size_t len = fread((void *) tmp, sizeof(char), sizeof(buffer), f);
        data_left -= len;
        tmp       += len;
    }
    buffer[file_size] = 0;

    fclose(f);

    // Load script into memory
    if (duk_peval_lstring_noresult(ctx, (const char *) buffer, (duk_size_t) file_size) != 0) {
        return false;
    }

    return true;
}

static duk_ret_t native_include(duk_context *ctx)
{
    const char *file_name = duk_require_string(ctx, 0);
    return include_script(file_name) ? 0 : 1;
}

static duk_ret_t native_print(duk_context *ctx)
{
    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    printf("%s\n", duk_safe_to_string(ctx, -1));
    return 0;
}



bool init_interface()
{
    ctx = duk_create_heap_default();

    if (!ctx)
        return false;

    REGISTER_SCRIPT_INTERFACE(  "print",   native_print, DUK_VARARGS);
    REGISTER_SCRIPT_INTERFACE("include", native_include,           1);

    return true;
}


