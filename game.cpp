#include "scene.hpp"
#include "game.hpp"
#include "util.h"
#include "coolmath.h"
#include "snowflakes.h"
#include "level.h"



//#define DEBUG_HISTORY

void history_erase(history *h, float t);

const float step_time = 0.1;
const float wipe_time = 1.2;

void game::set_state(game_state gs) {
    state_t = 0;
    state = gs;
}

void game::set_title_state(title_state ts) {
    title_state_t = 0;
    m_title_state = ts;
}

void game::title_sm_update(double dt) {
    title_state_t += dt;
    if (m_title_state == TS_FADE_IN && title_state_t > 1) {
        set_title_state(TS_SHOW);
    } else if (m_title_state == TS_SHOW && title_state_t > 2) {
        set_title_state(TS_FADE_OUT);
    } else if (m_title_state == TS_FADE_OUT && title_state_t > 1) {
        set_title_state(TS_NO_SHOW);
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
                g->state = game::GS_ANIMATE;
                g->state_t = 0;
            }
        }
    }

    level_step(&g->m_level);
}

// basically just goes through state machine
void game_update(shared_data *shared_data, void *scene_data, double dt) {
    game *g = (game *)scene_data;

    
}

void game_handle_input(shared_data *shared_data, void *scene_data, SDL_Event e) {
    game *g = (game *)scene_data;
    


}

void game::draw(shared_data *app_d, double dt) {
    gef_context *gc = &app_d->gc;

    const int t_start_size = 16;
    
    // determine the scale factor
    int scale_factor = 1;
    while ((scale_factor + 1) * t_start_size * m_level.tiles.w < gc->xres &&
           (scale_factor + 1) * t_start_size * m_level.tiles.h < gc->yres && scale_factor < app_d->max_scale) {
               scale_factor++;
    }

    const int tsize = t_start_size * scale_factor;

    const int level_w_px = tsize * m_level.tiles.w;
    const int level_h_px = tsize * m_level.tiles.h;

    int xo = gc->xres / 2 - level_w_px/2;
    int yo = gc->yres / 2 - level_h_px/2;

    xo = xo / tsize * tsize;
    yo = yo / tsize * tsize;

    int64_t t_start = get_us();

    float wipe_t = state_t / wipe_time;
    if (state == GS_FADE_OUT) {
        wipe_t = cm_slow_start2(wipe_t);
        xo += wipe_t * gc->xres;

        app_d->snow_offset_current = -wipe_t * gc->xres;

    } else if (state == GS_FADE_IN) {
        wipe_t = cm_slow_start2(1 - wipe_t);
        xo += -1 * wipe_t * gc->xres;

        app_d->snow_offset_current = wipe_t * gc->xres;

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

    if (state == GS_ANIMATE) {
        level_draw(&m_level, gc, xo, yo, scale_factor, state_t / step_time, app_d->time);
    } else {
        level_draw(&m_level, gc, xo, yo, scale_factor, 1, app_d->time);
    }

    int64_t t_level = get_us();
    

    if (app_d->draw_snow) {
        snowflakes_draw(gc, gc->xres, gc->yres, app_d->time, app_d->snow_offset_base + app_d->snow_offset_current);
    }

    int64_t t_snow = get_us();

    int x = gc->xres / 2 - m_level.title_handle.w / 2;

    if (m_title_state == TS_FADE_IN) {
        float downness = cm_slow_stop2(title_state_t);
        int y = downness * 100 - 50;
        gef_draw_text(gc, m_level.title_handle, x, y);
    } else if (m_title_state == TS_FADE_OUT) {
        float upness = cm_slow_start2(title_state_t);
        int y = (1 - upness) * 100 - 50;
        gef_draw_text(gc, m_level.title_handle, x, y);
    } else if (m_title_state == TS_SHOW) {
        int y = 50;
        gef_draw_text(gc, m_level.title_handle, x, y);
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

void game::update(shared_data *app_d, double dt) {
    title_sm_update(dt);

    state_t += dt;

    if (state == GS_FADE_OUT && state_t > wipe_time) {
        // FADE OUT -> FADE IN

        set_state(GS_FADE_IN);
        level_set *w = &app_d->worlds[app_d->world_idx];

        // tally progress
        if (! w->completed[app_d->level_idx]) {
            w->completed[app_d->level_idx] = true;
            int n_presents = 0;
            for (int i = 0; i < m_level.entities.num_entities; i++) {
                if (m_level.entities.entities[i].et == ET_PRESENT) {
                    w->collected_presents++;
                }
            }
        }

        app_d->snow_offset_base += app_d->snow_offset_current;
        app_d->snow_offset_current = 0;
        
        if (app_d->level_idx >= w->num_levels - 1) {
            // kick back to the main menu
            app_d->current_scene = SCENE_LEVEL_MENU;
            if (app_d->world_idx < app_d->num_worlds - 1) {
                app_d->level_idx = 0;
                app_d->world_idx++;
            } else {
                // you finished da game
            }
        } else {
            // next level
            app_d->level_idx++;
            on_focus(app_d);    
        }

    } else if (state == GS_FADE_IN && state_t > wipe_time) {
        // FADE IN -> NORMAL
        
        set_state(GS_NORMAL);

    } else if (state == GS_ANIMATE && state_t > step_time) {
        if (level_do_ice(&m_level, &app_d->a)) {
            // ANIMATE -> MORE ANIMATE

            set_state(GS_ANIMATE);
            level_step(&m_level);
        } else if(level_check_victory(&m_level)) {
            // ANIMATE -> VICTORY FADE OUT

            audio_play(&app_d->a, CS_WIN);
            set_state(GS_FADE_OUT);
        } else if (buffered_move_dx != 0 || buffered_move_dy != 0) {
            // ANIMATE -> MORE ANIMATE
            
            game_move_player(this, buffered_move_dx, buffered_move_dy, app_d->time, &app_d->a);
            buffered_move_dx = 0;
            buffered_move_dy = 0;
            
            // gets set to animate in game_move_player...
        } else {
            // ANIMATE -> NORMAL

            set_state(GS_NORMAL);
        }
    }
}

void game::on_focus(shared_data *app_d) {
    history_erase(&m_history, app_d->time);
    level_destroy(&m_level);

    const char *level_str = app_d->worlds[app_d->world_idx].levels[app_d->level_idx];

    level_init(&m_level, level_str, &app_d->gc, app_d->title_font, app_d);
    set_state(GS_FADE_IN);
    set_title_state(TS_FADE_IN);
}

void game::handle_input(shared_data *app_d, SDL_Event e) {
    if (state == GS_FADE_OUT) return;

    if (e.type  == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        if (sym == SDLK_ESCAPE) {
            app_d->current_scene = SCENE_LEVEL_MENU;
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

            if (state == GS_ANIMATE) {
                buffered_move_dx = dx;
                buffered_move_dy = dy;
            } else {
                game_move_player(this, dx, dy, app_d->time, &app_d->a);
            }

        } else if (reset) {
            on_focus(app_d);
            audio_play(&app_d->a, CS_LOSE);
        } else if (sym == SDLK_q) {
            if (app_d->selected_level == 0) {
                return;
            }
            app_d->selected_level--;
            on_focus(app_d);
        } else if (sym == SDLK_e) {
            if (app_d->selected_level == app_d->num_levels - 1) {
                return;
            }
            app_d->selected_level++;
            on_focus(app_d);
        } else if (sym == SDLK_u) {
            if (game_undo(this, app_d)) {
                audio_play(&app_d->a, CS_UNDO);
            }
        }
    }
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