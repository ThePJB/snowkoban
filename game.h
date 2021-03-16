#pragma once

#include "grid.h"
#include "scene.h"
#include "audio.h"
#include "entity.h"
#include "level.h"

typedef struct {
    entity_vla v;
    float time;
} history_record;

typedef struct {
    history_record *records;
    int length;
    int backing_size;
} history;

typedef enum {
    GS_NORMAL,
    GS_VICTORY_FADE_OUT,
    GS_FADE_IN,
    GS_ANIMATE,
    NUM_GS,
} game_state;

typedef enum {
    TS_NO_SHOW,
    TS_FADE_IN,
    TS_SHOW,
    TS_FADE_OUT,
} title_state;

typedef struct {
    scene_interface s;
    history m_history;

    float state_t;
    game_state state;

    float title_state_t;
    title_state m_title_state;
    
    level m_level;

    int buffered_move_dx;
    int buffered_move_dy;
} game;

history history_init();

void history_append_record(history *h, history_record r);

void game_append_history(game *g, float time);
bool game_undo(game *g, shared_data *shared_data);

void game_load_level_from_str(game *g, const char *level_str, shared_data *shared_data);

game game_init(shared_data *shared_data);