//
// Created by gravypod on 9/18/17.
//

#include "importhistory.h"

#include <stdlib.h>
#include <string.h>

struct included_scripts_t {
    struct included_scripts_t* next;
    const char *filename;
};

static struct included_scripts_t *last_script = NULL;

bool should_import(const char* filename)
{
    struct included_scripts_t *tmp = last_script;

    while (tmp)
    {
        if (strcmp(tmp->filename, filename) == 0)
            return false;
        tmp = tmp->next;
    }

    return true;
}

void mark_imported(const char* filename)
{
    struct included_scripts_t *imported = malloc(sizeof(struct included_scripts_t));
    imported->next = last_script;
    imported->filename = strdup(filename);
    last_script = imported;
}
