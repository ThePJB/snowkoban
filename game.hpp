#pragma once

#include "grid.hpp"
#include "scene.hpp"
#include "audio.hpp"
#include "entity.hpp"
#include "level.hpp"

typedef struct {
    vla<entity> v;
    float time;
} history_record;

struct game : scene {
    float state_t = 0;

    float title_state_t = 0;
    
    level m_level;

    int buffered_move_dx = 0;
    int buffered_move_dy = 0;

    game() {};

    vla<history_record> history = vla<history_record>();
    void clear_history();
    void append_current_state_to_history(float time);
    void undo(shared_data *shared_data);
    
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


void game_load_level_from_str(game *g, const char *level_str, shared_data *shared_data);