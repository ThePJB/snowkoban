#include "scene.h"
#include "game.h"
#include "util.h"
#include "coolmath.h"
#include "snowflakes.h"
#include "level.h"


//#define DEBUG_HISTORY

void history_erase(history *h, float t);

const float step_time = 0.1;
const float wipe_time = 1.2;

void game_set_state(game *g, game_state state) {
    g->state_t = 0;
    g->state = state;
}

void title_set_state(game *g, title_state state) {
    g->title_state_t = 0;
    g->m_title_state = state;
}

void title_sm_update(game *g, float dt) {
    g->title_state_t += dt;
    if (g->m_title_state == TS_FADE_IN && g->title_state_t > 1) {
        title_set_state(g, TS_SHOW);
    } else if (g->m_title_state == TS_SHOW && g->title_state_t > 2) {
        title_set_state(g, TS_FADE_OUT);
    } else if (g->m_title_state == TS_FADE_OUT && g->title_state_t > 1) {
        title_set_state(g, TS_NO_SHOW);
    }
}

void game_move_player(game *g, int dx, int dy, float time, audio *a) {
    if (!dx && !dy) {
        return;
    }

    if (dx > 0) {
        g->m_level.player_faces_left = false;
    } else if (dx < 0) {
        g->m_level.player_faces_left = true;
    }

    game_append_history(g, time);

    for (int i = 0; i < g->m_level.entities.num_entities; i++) {
        if (g->m_level.entities.entities[i].et == ET_PLAYER) {
            if (level_move_entity(&g->m_level, i, dx, dy, a)) {
                g->state = GS_ANIMATE;
                g->state_t = 0;
            }
        }
    }

    level_step(&g->m_level);
}

// basically just goes through state machine
void game_update(shared_data *shared_data, void *scene_data, double dt) {
    game *g = (game *)scene_data;

    title_sm_update(g, dt);

    g->state_t += dt;

    if (g->state == GS_VICTORY_FADE_OUT && g->state_t > wipe_time) {
        // FADE OUT -> FADE IN

        game_set_state(g, GS_FADE_IN);
        level_set *w = &shared_data->worlds[shared_data->world_idx];

        // tally progress
        if (! w->completed[shared_data->level_idx]) {
            w->completed[shared_data->level_idx] = true;
            int n_presents = 0;
            for (int i = 0; i < g->m_level.entities.num_entities; i++) {
                if (g->m_level.entities.entities[i].et == ET_PRESENT) {
                    w->collected_presents++;
                }
            }
        }

        shared_data->snow_offset_base += shared_data->snow_offset_current;
        shared_data->snow_offset_current = 0;
        
        if (shared_data->level_idx >= w->num_levels - 1) {
            // kick back to the main menu
            shared_data->current_scene = SCENE_LEVEL_MENU;
            if (shared_data->world_idx < shared_data->num_worlds - 1) {
                shared_data->level_idx = 0;
                shared_data->world_idx++;
            } else {
                // you finished da game
            }
        } else {
            // next level
            shared_data->level_idx++;
            g->s.on_focus(shared_data, g);    
        }

    } else if (g->state == GS_FADE_IN && g->state_t > wipe_time) {
        // FADE IN -> NORMAL
        
        game_set_state(g, GS_NORMAL);

    } else if (g->state == GS_ANIMATE && g->state_t > step_time) {
        if (level_do_ice(&g->m_level, &shared_data->a)) {
            // ANIMATE -> MORE ANIMATE

            game_set_state(g, GS_ANIMATE);
            level_step(&g->m_level);
        } else if(level_check_victory(&g->m_level)) {
            // ANIMATE -> VICTORY FADE OUT

            audio_play(&shared_data->a, CS_WIN);
            game_set_state(g, GS_VICTORY_FADE_OUT);
        } else if (g->buffered_move_dx != 0 || g->buffered_move_dy != 0) {
            // ANIMATE -> MORE ANIMATE
            
            game_move_player(g, g->buffered_move_dx, g->buffered_move_dy, shared_data->time, &shared_data->a);
            g->buffered_move_dx = 0;
            g->buffered_move_dy = 0;
            
            // gets set to animate in game_move_player...
        } else {
            // ANIMATE -> NORMAL

            game_set_state(g, GS_NORMAL);
        }
    }
}

void game_handle_input(shared_data *shared_data, void *scene_data, SDL_Event e) {
    game *g = (game *)scene_data;
    

    if (g->state == GS_VICTORY_FADE_OUT) return;
    //if (g->state == GS_ANIMATE) return; // lets see what breaks if you reset or undo while animating

    if (e.type  == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        if (sym == SDLK_ESCAPE) {
            shared_data->current_scene = SCENE_LEVEL_MENU;
            return;
        }

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool left = sym == SDLK_LEFT || sym == SDLK_a || sym == SDLK_h;
        bool right = sym == SDLK_RIGHT || sym == SDLK_d || sym == SDLK_l;
        bool reset = sym == SDLK_r;

        bool movement = up || down || left || right;

        if (movement) {

            int dx = 0;
            int dy = 0;

            if (up) {
                dy = -1;
            } else if (down) {
                dy = 1;
            } else if (left) {
                dx = -1;
            } else if (right) {
                dx = 1;
            }

            if (g->state == GS_ANIMATE) {
                g->buffered_move_dx = dx;
                g->buffered_move_dy = dy;
            } else {
                game_move_player(g, dx, dy, shared_data->time, &shared_data->a);
            }

        } else if (reset) {
            g->s.on_focus(shared_data, g);
            audio_play(&shared_data->a, CS_LOSE);
        } else if (sym == SDLK_q) {
            if (shared_data->selected_level == 0) {
                return;
            }
            shared_data->selected_level--;
            g->s.on_focus(shared_data, g);
        } else if (sym == SDLK_e) {
            if (shared_data->selected_level == shared_data->num_levels - 1) {
                return;
            }
            shared_data->selected_level++;
            g->s.on_focus(shared_data, g);
        } else if (sym == SDLK_u) {
            if (game_undo(g, shared_data)) {
                audio_play(&shared_data->a, CS_UNDO);
            }
        }
    }
}

void game_draw(shared_data *shared_data, void *scene_data, gef_context *gc, double dt) {
    game *g = (game *)scene_data;

    const int t_start_size = 16;
    
    // determine the scale factor
    int scale_factor = 1;
    while ((scale_factor + 1) * t_start_size * g->m_level.tiles.w < gc->xres &&
           (scale_factor + 1) * t_start_size * g->m_level.tiles.h < gc->yres && scale_factor < shared_data->max_scale) {
               scale_factor++;
    }

    const int tsize = t_start_size * scale_factor;

    const int level_w_px = tsize * g->m_level.tiles.w;
    const int level_h_px = tsize * g->m_level.tiles.h;

    int xo = gc->xres / 2 - level_w_px/2;
    int yo = gc->yres / 2 - level_h_px/2;

    xo = xo / tsize * tsize;
    yo = yo / tsize * tsize;

    int64_t t_start = get_us();

    float wipe_t = g->state_t / wipe_time;
    if (g->state == GS_VICTORY_FADE_OUT) {
        wipe_t = cm_slow_start2(wipe_t);
        xo += wipe_t * gc->xres;

        shared_data->snow_offset_current = -wipe_t * gc->xres;

    } else if (g->state == GS_FADE_IN) {
        wipe_t = cm_slow_start2(1 - wipe_t);
        xo += -1 * wipe_t * gc->xres;

        shared_data->snow_offset_current = wipe_t * gc->xres;

    }

    int64_t t_fade = get_us();

    SDL_Rect clip_wall = {0, 0, 16, 16};
  
    int start_wall_px = (xo % tsize) - tsize;
    int max_wall_px = gc->xres + tsize;
    for (int i = start_wall_px; i < max_wall_px; i += tsize) {
        for (int j = 0; j < (gc->yres + tsize); j += tsize) {
            SDL_Rect to_rect = (SDL_Rect) {i, j, tsize, tsize};
            gef_draw_sprite(gc, clip_wall, to_rect);
        }
    }

    int64_t t_fill = get_us();

    if (g->state == GS_ANIMATE) {
        level_draw(&g->m_level, gc, xo, yo, scale_factor, g->state_t / step_time, shared_data->time);
    } else {
        level_draw(&g->m_level, gc, xo, yo, scale_factor, 1, shared_data->time);
    }

    int64_t t_level = get_us();
    

    if (shared_data->draw_snow) {
        snowflakes_draw(gc, gc->xres, gc->yres, shared_data->time, shared_data->snow_offset_base + shared_data->snow_offset_current);
    }

    int64_t t_snow = get_us();

    int x = gc->xres / 2 - g->m_level.title_handle.w / 2;

    if (g->m_title_state == TS_FADE_IN) {
        float downness = cm_slow_stop2(g->title_state_t);
        int y = downness * 100 - 50;
        gef_draw_text(gc, g->m_level.title_handle, x, y);
    } else if (g->m_title_state == TS_FADE_OUT) {
        float upness = cm_slow_start2(g->title_state_t);
        int y = (1 - upness) * 100 - 50;
        gef_draw_text(gc, g->m_level.title_handle, x, y);
    } else if (g->m_title_state == TS_SHOW) {
        int y = 50;
        gef_draw_text(gc, g->m_level.title_handle, x, y);
    }

    int64_t t_text = get_us();


    #ifdef PROFILE
    printf("t_fade = %f, t_fill = %f, t_level = %f, t_snow = %f, t_text = %f\n",
        (t_fade - t_start) / 1000.0,
        (t_fill - t_fade) / 1000.0,
        (t_level - t_fill) / 1000.0,
        (t_snow - t_level) / 1000.0,
        (t_text - t_snow) / 1000.0
    );
    #endif
}

#define HISTORY_STARTING_SIZE 100

history history_init() {
    history h = (history) {
        .records = (history_record *)malloc(HISTORY_STARTING_SIZE * sizeof(history_record)),
        .length = 0,
        .backing_size = HISTORY_STARTING_SIZE,
    };

    return h;
}

void history_erase(history *h, float t) {
    for (int i = 0; i < h->length; i++) {
        entity_vla_destroy(&h->records[i].v);
    }
    h->length = 0;
}

void history_append_record(history *h, history_record r) {
    if (h->length == h->backing_size) {
        h->backing_size *= 2;
        h->records = (history_record *)realloc(h->records, sizeof(history_record) * h->backing_size);
    }
    h->records[h->length++] = r;
}

void game_append_history(game *g, float time) {
    #ifdef DEBUG_HISTORY
    printf("appending to history, current state:\n...\n");
    for (int i = max(g->m_history.length-2, 0); i < g->m_history.length; i++) {
        printf("t: %f\n", g->m_history.records[i].time);
        entity_vla_print(&g->m_history.records[i].v);
    }
    #endif
    if (g->m_history.length >= g->m_history.backing_size - 2) {
        g->m_history.backing_size *= 2;
        g->m_history.records = (history_record *)realloc(g->m_history.records, sizeof(history_record) * g->m_history.backing_size);        
    }

    history_record r = (history_record) {
        .v = entity_vla_clone(&g->m_level.entities),
        .time = time,
    };

    #ifdef DEBUG_HISTORY
    printf("\nnew record: t %f, v:\n", r.time);
    entity_vla_print(&r.v);
    #endif

    g->m_history.records[g->m_history.length++] = r;
}

bool game_undo(game *g, shared_data *shared_data) {
    #ifdef DEBUG_HISTORY
    printf("undoing, current state:\n...\n");
    for (int i = max(g->m_history.length-2, 0); i < g->m_history.length; i++) {
        printf("t: %f\n", g->m_history.records[i].time);
        entity_vla_print(&g->m_history.records[i].v);
    }
    #endif

    // already at the start
    if (g->m_history.length == 0) {
        return false;
    }

    entity_vla_destroy(&g->m_level.entities);
    g->m_history.length--;
    shared_data->time = g->m_history.records[g->m_history.length].time;
    g->m_level.entities = g->m_history.records[g->m_history.length].v;

    Mix_RewindMusic();
    Mix_SetMusicPosition(shared_data->time);

    #ifdef DEBUG_HISTORY
    printf("undoing, new state:\n...\n");
    for (int i = max(g->m_history.length-2, 0); i < g->m_history.length; i++) {
        printf("t: %f\n", g->m_history.records[i].time);
        entity_vla_print(&g->m_history.records[i].v);
    }
    #endif

    return true;
}

void game_on_focus(shared_data *shared_data, void *scene_data) {
    game *g = (game *)scene_data;
    
    history_erase(&g->m_history, shared_data->time);
    level_destroy(&g->m_level);

    const char *level_str = shared_data->worlds[shared_data->world_idx].levels[shared_data->level_idx];

    level_init(&g->m_level, level_str, &shared_data->gc, shared_data->title_font, shared_data);
    game_set_state(g, GS_FADE_IN);
    title_set_state(g, TS_FADE_IN);
}

game game_init(shared_data *shared_data) {
    game g = {0};
    g.m_history = history_init();

    g.s = (scene_interface) {
        .draw = game_draw,
        .handle_input = game_handle_input,
        .on_focus = game_on_focus,
        .update = game_update,
    };

    g.state = GS_NORMAL;
    g.state_t = 0;

    g.m_title_state = TS_NO_SHOW;
    g.title_state_t = 0;

    return g;
}


 