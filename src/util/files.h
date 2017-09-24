//
// Created by gravypod on 9/20/17.
//

#ifndef ENGINE_FILES_H
#define ENGINE_FILES_H

#include "lib/duktape/duktape.h"
#include "llist.h"

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


llist* list_files(const char* folder_name, const char *ext);

/**
 * Native wrapper to read_file
 */
__attribute__((unused)) static duk_ret_t native_read(duk_context *ctx)
{
    const char *file_name = duk_require_string(ctx, 0);
    const char *data      = read_file(file_name);

    if (data)
        duk_push_lstring(ctx, data, strlen(data));
    else
        duk_push_null(ctx);

    return 1;
}

__attribute__((unused)) static duk_ret_t native_list_files(duk_context *ctx)
{
    // Find folder we want to scan
    const char *folder_name = duk_require_string(ctx, 0);
    llist            *files = list_files(folder_name, NULL);
    llist              *tmp = files;

    // Duktape Javascript array information
    duk_uarridx_t num_ret = 0;
    duk_idx_t     arr_idx;

    // Create an array on the stack
    arr_idx = duk_push_array(ctx);

    while (tmp) {
        // Add element to the javascript array
        duk_push_string(ctx, tmp->name);
        duk_put_prop_index(ctx, arr_idx, num_ret);

        // Add next item and increment index
        tmp = tmp->next;
        num_ret += 1;
    }

    // Free memory from llist
    llist_free(&files);

    return 1;
}

#endif //ENGINE_FILES_H
