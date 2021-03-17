#pragma once

#include "grid.h"
#include "scene.hpp"
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

/*
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

*/

history history_init();


struct game : scene {
    history m_history = history_init();

    float state_t = 0;

    float title_state_t = 0;
    
    level m_level;

    int buffered_move_dx = 0;
    int buffered_move_dy = 0;
    
    enum game_state {GS_NORMAL, GS_FADE_OUT, GS_FADE_IN, GS_ANIMATE};
    game_state state = GS_NORMAL;
    void set_state(game_state gs);

    enum title_state {TS_NO_SHOW, TS_FADE_IN, TS_SHOW, TS_FADE_OUT};
    title_state m_title_state = TS_NO_SHOW;
    void set_title_state(title_state ts);
    void title_sm_update(double dt);
    
    void draw(shared_data *app_d, double dt);
    void update(shared_data *app_d, double dt);
    void on_focus(shared_data *app_d);
    void handle_input(shared_data *app_d, SDL_Event e);
};


void history_append_record(history *h, history_record r);

void game_append_history(game *g, float time);
bool game_undo(game *g, shared_data *shared_data);

void game_load_level_from_str(game *g, const char *level_str, shared_data *shared_data);