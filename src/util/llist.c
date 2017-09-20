//
// Created by gravypod on 9/20/17.
//

#include "llist.h"

#include <string.h>
#include <malloc.h>

void *llist_get(llist **head, const char const *name)
{
    llist const * tmp = *head;
    while (tmp)
    {
        if (strcmp(name, tmp->name) == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}

bool llist_has(llist **head, const char const *name)
{
    return llist_get(head, name) != NULL;
}

void llist_add(llist **head, const char const *name, const void *value, size_t size)
{
    // Allocate
    llist *next = malloc(sizeof(llist));

    // Set variables
    next->value = malloc(size);
    next->name = strdup(name);
    next->next = *head;

    // Copy data into container
    memcpy(next->value, value, size);

    *head = next;
}