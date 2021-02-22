#pragma once

#include "grid.h"
#include "scene.h"
#include "audio.h"

typedef enum {
    TT_WALL = 1,
    TT_SNOW = 2,
    TT_BOX = 4,
    TT_TARGET = 8,
    TT_ICE = 16,
    TT_PLAYER = 32,
    TT_CRATE = 64,
    TT_SENTINEL = 128,
    TT_HOLE = 256,
} tile_type;

typedef struct {
    tile_type t;
    int x;
    int y;
} history_record;

typedef struct {
    history_record *records;
    int length;
    int backing_size;
} history;

typedef struct {
    scene_interface s;
    grid current_level;
    int current_level_num;
    char *current_level_name;
    history history;
    audio *audio;
} game;

history history_init();

void history_append_record(history *h, history_record r);

void game_append_history(game *g);
void game_undo(game *g);

void game_load_level_from_str(game *g, const char *level_str);

game game_init(audio *audio, shared_data *shared_data);