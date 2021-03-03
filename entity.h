#pragma once

#include "SDL.h"

typedef enum {
    ET_NONE,
    ET_PLAYER,
    ET_CRATE,
    ET_PRESENT,
    ET_TARGET,
    NUM_ET,
} entity_type;

typedef struct {
    const char *name;
    const char *symbols;
    SDL_Rect clip;
    bool moveable;
} entity_prototype;

typedef struct {
    entity_type et;
    int x;
    int y;
    int dx;
    int dy;
} entity;

typedef struct {
    entity *entities;
    int backing_array_size;
    int num_entities;
} entity_vla;

void entity_vla_append(entity_vla *v, entity e);
entity_vla entity_vla_clone(entity_vla *v);
void entity_vla_init(entity_vla *v);
void entity_vla_destroy(entity_vla *v);
entity_prototype entity_prototype_get(entity_type et);
void entity_vla_print(entity_vla *v);