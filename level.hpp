#pragma once

#include "grid.hpp"
#include <SDL.h>
#include "gef.hpp"
#include "entity.hpp"
#include "audio.hpp"
#include "scene.hpp"
#include "vla.hpp"

typedef enum {
    T_NONE,
    T_SNOW,
    T_ICE,
    T_HOLE,
    T_WALL,
    NUM_TT,
} tile;

typedef struct {
    const char *name;
    const char *symbols;
    SDL_Rect clip;
    channel_sound walk_sound;
} tile_prototype;


struct level {
    bool initialized = false;
    
    char *title;
    grid<tile> tiles;
    vla<entity> entities;
    bool player_faces_left = false;

    level(){};
    level(const char *level_str, shared_data *app_d);
};

void level_destroy(level *l);
void level_draw(level *l, gef_context *gc, int xo, int yo, int pxsize, float t, float time);
bool level_move_entity(level *l, int entity_idx, int dx, int dy, audio *a);
bool level_check_victory(level *l);
bool level_do_ice(level *l, audio *a);

void level_step(level *l);