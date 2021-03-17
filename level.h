#pragma once

#include "grid.h"
#include "SDL.h"
#include "gef.h"
#include "entity.h"
#include "audio.h"
#include "scene.hpp"

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
    channel_sound walk_sound;
} tile_prototype;


typedef struct {
    char *title;
    grid tiles;
    entity_vla entities;
    text_handle title_handle;
    bool player_faces_left;
} level;

void level_init(level *l, const char *level_str, gef_context *gc, font_handle font, shared_data *shared_data);
void level_destroy(level *l);
void level_draw(level *l, gef_context *gc, int xo, int yo, int pxsize, float t, float time);
bool level_move_entity(level *l, int entity_idx, int dx, int dy, audio *a);
void level_set_tile(level *l, int x, int y, tile_type t);
tile_type level_get_tile(level *l, int x, int y);
bool level_check_victory(level *l);
bool level_do_ice(level *l, audio *a);

void level_step(level *l);