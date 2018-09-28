#include "dict.h"
#include <stddef.h>
#include <malloc.h>

#include <src/engine/util/hashing/crc.h>
#include <memory.h>

typedef size_t bucket_index_t;

/**
 * Find the bucket index for this hash map
 * @param d - Dictionary object
 * @param name - Name of the pair
 * @param length - Length of the name
 * @return
 */
static inline bucket_index_t dict_bucket_for(dict *d, const char *const name, const size_t length)
{
	const uint_fast32_t hash = hashing_crc32_make(name, length);
	const size_t num_buckets = d->num_buckets;
	return (bucket_index_t) (hash % num_buckets);
}

static inline bool dict_bucket_empty(dict *d, bucket_index_t i)
{
	return llist_empty(&d->buckets[i]);
}


dict *dict_init(size_t num_buckets)
{
	const size_t size_of_buckets = sizeof(llist *) * num_buckets;
	dict *d = malloc(sizeof(dict) + size_of_buckets);
	d->num_buckets = num_buckets;
	memset(d->buckets, 0, size_of_buckets);
	return d;
}

void dict_clear(dict *d)
{
	if (d) {
		for (bucket_index_t i = 0; i < d->num_buckets; i++) {
			llist_free(&d->buckets[i]);
		}
	}
}

bool dict_get(dict *d, const char *const name, void *value, size_t *length)
{
	const size_t len_name = strlen(name);
	bucket_index_t index = dict_bucket_for(d, name, len_name);

	if (dict_bucket_empty(d, index)) {
		return false;
	}

	return llist_retrive(&d->buckets[index], name, value, length);
}

bool dict_get_string(dict *d, const char *const name, char **value)
{
	return dict_get(d, name, (void *) value, NULL);
}

bool dict_has(dict *d, const char *const name)
{
	return dict_get(d, name, NULL, NULL);
}

void dict_set(dict *d, const char *const name, void *value, size_t length)
{
	const size_t len_name = strlen(name);
	bucket_index_t index = dict_bucket_for(d, name, len_name);
	llist_add(&d->buckets[index], name, value, length);
}

void dict_set_string(dict *d, const char *const name, char *value)
{
	dict_set(d, name, value, strlen(value));
}

