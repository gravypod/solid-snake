#ifndef ENGINE_ENTITY_ENTITY_H
#define ENGINE_ENTITY_ENTITY_H

#include <stddef.h>
#include <stdbool.h>
#include <src/engine/util/dict.h>
#include <vulkan/vulkan.h>

/**
 * Abstract definition of an entity
 */
typedef struct entity_struct
{
	/**
	 * Unique ID for every entity
	 */
	size_t id;

	/**
	 * Misc paramaters that can't be accounted for
	 */
	dict *paramaters;

	/**
	 * If this entity is already being used by something
	 */
	bool allocated;

	void (*free)(struct entity_struct *);

	void (*update)(struct entity_struct *);

	void (*draw)(struct entity_struct *, VkCommandBuffer buffer);
} entity_t;

/**
 * Abstract definition of initialization of an entity
 */
typedef void (*entity_initializer_t)(entity_t *, void *);

/**
 * Consume and process an entity
 */
typedef void (*entity_consumer_t)(entity_t *, void *);


/**
 * Initialize an empty entity
 * @param e
 */
void entity_init_empty(entity_t *e, void *metadata);


void entity_update(entity_t *entity, void *metadata);

void entity_free(entity_t *entity, void *metadata);

/**
 * Draw an entity to a command buffer
 *
 * @param e
 * @param buffer
 */
void entity_draw(entity_t *e, VkCommandBuffer buffer);

#endif
