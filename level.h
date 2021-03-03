#pragma once

#include "grid.h"
#include "SDL.h"
#include "gef.h"
#include "entity.h"

typedef enum {
    TT_NONE,
    TT_SNOW,
    TT_ICE,
    TT_HOLE,
    TT_WALL,
    NUM_TT,
} tile_type;

typedef struct {
    const char *name;
    const char *symbols;
    SDL_Rect clip;
} tile_prototype;


typedef struct {
    char *title;
    grid tiles;
    entity_vla entities;
} level;

void level_init(level *l, const char *level_str);
void level_destroy(level *l);
void level_draw(level *l, gef_context *gc, int xo, int yo, float t);
bool level_move_entity(level *l, int entity_idx, int dx, int dy);
void level_set_tile(level *l, int x, int y, tile_type t);
tile_type level_get_tile(level *l, int x, int y);
bool level_check_victory(level *l);
bool level_do_ice(level *l);