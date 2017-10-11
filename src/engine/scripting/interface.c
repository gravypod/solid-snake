//
// Created by gravypod on 9/15/17.
//

#include "interface.h"
#include "src/engine/util/llist.h"
#include "src/engine/util/files.h"
#include "src/engine/graphics/style.h"

llist *imported_scripts = NULL;

const char *stdlib_assets =
    "function stdlib_assets_load_style_file(file_name) {\n"
        "var s = JSON.parse(read_file(file_name));\n"
        "var tex = s.textures || [];"
        "make_style(s.name, s.width, s.height, s.shader.vertex, s.shader.fragment, tex.length, tex);\n"
    "}\n"
    "function draw(name, x, y, rotation) {\n"
        "draw_style(name, x, y, rotation);\n"
    "}\n"
    "list_files(\"assets/styles/\").forEach(function (e, i, a) {\n"
        "if (e.endsWith(\".json\")) {\n"
            "print(\"ASSET-STYLE: Loading\", e);\n"
            "stdlib_assets_load_style_file(e);\n"
        "}\n"
    "});\n"
"\0";


inline static bool eval_string(const char *buffer)
{
    return duk_peval_lstring_noresult(ctx, buffer, (duk_size_t) strlen(buffer)) != 0;
}

bool include_script(const char *filename)
{
    if (llist_has(&imported_scripts, filename))
        return true;

    char *buffer = read_file(filename);

    if (!buffer)
        return false;

    // Load game into memory
    if (eval_string(buffer))
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
    REGISTER_SCRIPT_INTERFACE("list_files", native_list_files,           1);
    REGISTER_SCRIPT_INTERFACE("make_style", native_make_style,           7); // Graphics natives
    REGISTER_SCRIPT_INTERFACE("draw_style", native_draw_style,           4);


    /* STANDARD LIBRARY FUNCTIONS */

    if (eval_string(stdlib_assets)) // Asset loading
        return false;

    return true;
}


