//
// Created by gravypod on 9/20/17.
//

#ifndef ENGINE_LLIST_H
#define ENGINE_LLIST_H

#include <stdbool.h>
#include <stddef.h>

struct llist_t {
    struct llist_t *last;
    struct llist_t *next;
    char *name;
    void *value;
    size_t length;
};
typedef struct llist_t llist;

void *llist_get(llist **head, const char *const name);

bool llist_retrive(llist **head, const char *const name, void **value, size_t *length);

bool llist_has(llist **head, const char *const name);

void llist_add(llist **head, const char *const name, const void *const value, size_t size);

void llist_remove(llist **head, const char *const name);

bool llist_empty(llist **head);

void llist_free(llist **head);

#endif //ENGINE_LLIST_H
