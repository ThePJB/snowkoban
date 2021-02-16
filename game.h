#pragma once

#include "grid.h"
#include "scene.h"

typedef enum {
    TT_WALL = 1,
    TT_SNOW = 2,
    TT_BOX = 4,
    TT_TARGET = 8,
    TT_ICE = 16,
    TT_PLAYER = 32,
} tile_type;

typedef struct {
    scene_interface s;
    grid current_level;
    int current_level_num;
    int player_x;
    int player_y;
} game;

void game_load_level_from_str(game *g, const char *level_str);

game game_init();