//
// Created by gravypod on 9/20/17.
//

#include "llist.h"

#include <string.h>
#include <malloc.h>

llist *llist_get_internal(llist **head, const char *const name) {
    llist *tmp = *head;
    while (tmp) {
        if (strcmp(name, tmp->name) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

void *llist_get(llist **head, const char *const name) {
    llist const *tmp = llist_get_internal(head, name);
    if (tmp) {
        return tmp->value;
    }
    return NULL;
}

bool llist_retrive(llist **head, const char *const name, void **value, size_t *length) {
    llist const *tmp = llist_get_internal(head, name);
    if (tmp) {
        if (length) {
            *length = tmp->length;
        }
        if (value) {
            *value = tmp->value;
        }
        return true;
    }
    return false;
}


bool llist_has(llist **head, const char *const name) {
    return llist_get(head, name) != NULL;
}

void llist_add(llist **head, const char *const name, const void *const value, size_t size) {
    // Allocate
    llist *next = malloc(sizeof(llist));

    // Set variables
    next->value = malloc(size);
    next->name = strdup(name);
    next->next = *head;
    next->length = size;
    next->last = NULL;

    // Set last value
    if (*head) {
        (*head)->last = next;
    }

    // Copy data into container
    memcpy(next->value, value, size);

    *head = next;
}

void llist_remove(llist **head, const char *const name) {
    llist *item = llist_get_internal(head, name);

    if (!item) {
        return;
    }

    if (item->last) {
        item->last->next = item->next;
    }

    if (item->next) {
        item->next->last = item->last;
    }

    free(item->value);
    free(item->name);
    free(item);
}

bool llist_empty(llist **head) {
    return *head == NULL;
}

void llist_free(llist **head) {
    while (*head) {
        llist *curr = *head;
        llist *next = curr->next;
        free(curr->name);
        free(curr->value);
        *head = next;
    }
}
