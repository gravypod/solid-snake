//
// Created by gravypod on 9/15/17.
//

#include "interface.h"
#include "src/util/llist.h"
#include "src/util/files.h"
#include "src/graphics/style.h"

llist *imported_scripts = NULL;

bool include_script(const char *filename)
{
    if (llist_has(&imported_scripts, filename))
        return true;

    char *buffer = read_file(filename);

    if (!buffer)
        return false;

    // Load script into memory
    if (duk_peval_lstring_noresult(ctx, (const char *) buffer, (duk_size_t) strlen(buffer)) != 0)
        return false;

    free(buffer);

    llist_add(&imported_scripts, filename, NULL, 0);

    return true;
}

bool init_interface()
{
    ctx = duk_create_heap_default();

    if (!ctx)
        return false;

    REGISTER_SCRIPT_INTERFACE(     "print",      native_print, DUK_VARARGS); // Standard natives
    REGISTER_SCRIPT_INTERFACE(   "include",    native_include,           1);
    REGISTER_SCRIPT_INTERFACE( "read_file",       native_read,           1);
    REGISTER_SCRIPT_INTERFACE("make_style", native_make_style,           5); // Graphics natives
    REGISTER_SCRIPT_INTERFACE("draw_style", native_draw_style,           1);


    return true;
}


