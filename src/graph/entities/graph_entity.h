/*
* Copyright 2018-2019 Redis Labs Ltd. and Contributors
*
* This file is available under the Apache License, Version 2.0,
* modified with the Commons Clause restriction.
*/

#ifndef GRAPH_ENTITY_H_
#define GRAPH_ENTITY_H_

#include "../../value.h"
#include "../../../deps/GraphBLAS/Include/GraphBLAS.h"

#define ENTITY_ID_ISLT(a,b) ((*a)<(*b))
#define INVALID_ENTITY_ID -1l

#define ENTITY_GET_ID(graphEntity) ((graphEntity)->entity ? (graphEntity)->entity->id : INVALID_ENTITY_ID)
#define ENTITY_PROP_COUNT(graphEntity) ((graphEntity)->entity->prop_count)
#define ENTITY_PROPS(graphEntity) ((graphEntity)->entity->properties)

// Defined in graph_entity.c
extern SIValue *PROPERTY_NOTFOUND;
typedef GrB_Index EntityID;
typedef GrB_Index NodeID;
typedef GrB_Index EdgeID;

typedef struct {
    char *name;
    SIValue value;
} EntityProperty;

// Essence of a graph entity.
// TODO: see if pragma pack 0 will cause memory access violation on ARM.
typedef struct {
    EntityID id;                    // Unique id
    int prop_count;                 // Number of properties.
    EntityProperty *properties;     // Key value pair of attributes.
} Entity;

// Common denominator between nodes and edges.
typedef struct {
    Entity *entity;
} GraphEntity;

/* Adds a properties to entity
 * prop_count - number of new properties to add 
 * keys - array of properties keys 
 * values - array of properties values */
void GraphEntity_Add_Properties(GraphEntity *ge, int prop_count, char **keys, SIValue *values);

/* Retrieves entity's property
 * NOTE: If the key does not exist, we return the special
 * constant value PROPERTY_NOTFOUND. */
SIValue* GraphEntity_Get_Property(const GraphEntity *e, const char* key);

/* Release all memory allocated by entity */
void FreeEntity(Entity *e);

#endif
