/*
* Copyright 2018-2019 Redis Labs Ltd. and Contributors
*
* This file is available under the Apache License, Version 2.0,
* modified with the Commons Clause restriction.
*/

#include "graph_entity.h"
#include <stdio.h>
#include "../../util/rmalloc.h"

SIValue *PROPERTY_NOTFOUND = &(SIValue){.intval = 0, .type = T_NULL};

/* Expecting e to be either *Node or *Edge */
void GraphEntity_Add_Properties(GraphEntity *e, int prop_count, char **keys, SIValue *values) {
	if(e->entity->properties == NULL) {
		e->entity->properties = rm_malloc(sizeof(EntityProperty) * prop_count);
	} else {
		e->entity->properties = rm_realloc(e->entity->properties, sizeof(EntityProperty) * (e->entity->prop_count + prop_count));
	}
	
	for(int i = 0; i < prop_count; i++) {
		e->entity->properties[e->entity->prop_count + i].name = rm_strdup(keys[i]);
		e->entity->properties[e->entity->prop_count + i].value = values[i];
	}

	e->entity->prop_count += prop_count;
}

SIValue* GraphEntity_Get_Property(const GraphEntity *e, const char* key) {
	for(int i = 0; i < e->entity->prop_count; i++) {
		if(strcmp(key, e->entity->properties[i].name) == 0) {
			// Note, this is a bit unsafe as entity properties can get reallocated.
			return &(e->entity->properties[i].value);
		}
	}
	return PROPERTY_NOTFOUND;
}

void FreeEntity(Entity *e) {
	if(e->properties != NULL) {
		for(int i = 0; i < e->prop_count; i++) {
			rm_free(e->properties[i].name);
			SIValue_Free(&e->properties[i].value);
		}
		rm_free(e->properties);
		e->properties = NULL;
	}
}
