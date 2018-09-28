#ifndef ENGINE_UTIL_DICT_H
#define ENGINE_UTIL_DICT_H

#include <src/engine/util/llist.h>

#define DICT_DEFAULT_NUM_BUCKETS 256

#define dict_new() dict_init(DICT_DEFAULT_NUM_BUCKETS)

typedef struct {
    size_t num_buckets;
    llist *buckets[];
} dict;

dict *dict_init(size_t num_buckets);

void dict_clear(dict *d);

/**
 * Get a value from the dictionary
 *
 * @param d
 * @param name
 * @param value
 * @param length
 * @return
 */
bool dict_get(dict *d, const char *name, void *value, size_t *length);


/**
 * Check to see if a dictionary has a KV pair
 * @param d
 * @param name
 * @return
 */
bool dict_has(dict *d, const char *name);

/**
 * Set a dictionary KV pair
 * @param d
 * @param name
 * @param value
 * @param length
 */
void dict_set(dict *d, const char *name, void *value, size_t length);

/**
 * Set a KV pair of strings
 * @param d
 * @param name
 * @param value
 */
void dict_set_string(dict *d, const char *name, char *value);


/**
 * Get a string from a K
 * @param d
 * @param name
 * @param value
 * @return
 */
bool dict_get_string(dict *d, const char *name, char **value);

#endif
