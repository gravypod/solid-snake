//
// Created by gravypod on 9/20/17.
//

#ifndef ENGINE_FILES_H
#define ENGINE_FILES_H

#include "lib/duktape/duktape.h"

/**
 * Size of the file in bytes.
 * @param filename - Path to the file to measure
 * @return - Size of file
 */
long int fsize(const char *filename);

/**
 * Read a file into a char array.
 * @param filename - filename to read
 * @return - NULL if the file read failed, otherwise a pointer that you need to free().
 */
char* read_file(const char* filename);

/**
 * Native wrapper to read_file
 */
static duk_ret_t native_read(duk_context *ctx)
{
    const char *file_name = duk_require_string(ctx, 0);
    const char *data      = read_file(file_name);

    if (data)
        duk_push_lstring(ctx, data, strlen(data));
    else
        duk_push_null(ctx);

    return 1;
}

#endif //ENGINE_FILES_H
