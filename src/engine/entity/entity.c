#include "entity.h"

void entity_init_empty(entity_t *e, void *metadata)
{
	(void) metadata;
	if (!e) {
		return;
	}

	e->allocated = false;

	// Null out behavior
	e->draw = NULL;
	e->free = NULL;
	e->update = NULL;

	if (!e->paramaters) {
		// This entity needs a dictionary allocated for them.
		e->paramaters = dict_init(16);
	} else {
		// This entity already had a dict allocated. Just reuse this by emptying it
		dict_clear(e->paramaters);
	}
}

void entity_update(entity_t *entity, void *metadata)
{
	(void) metadata;
	if (entity->update) {
		entity->update(entity);
	}
}

void entity_free(entity_t *entity, void *metadata)
{
	(void) metadata;
	if (entity->free) {
		entity->free(entity);
	}
}

void entity_draw(entity_t *e, VkCommandBuffer metadata)
{
	if (e->draw) {
		e->draw(e, metadata);
	}
}
