#include "scene.h"
#include "game.h"
#include "util.h"
#include "coolmath.h"
#include "snowflakes.h"
#include "level.h"

//#define DEBUG_HISTORY

void history_erase(history *h, float t);

const float step_time = 0.1;

// action of the state machine
// is this retarded yes
void game_set_state(game *g, game_state state) {
    g->state_t = 0;
    g->state = state;
}

void game_move_player(game *g, int dx, int dy, float time) {
    if (!dx && !dy) {
        return;
    }

    if (dx > 0) {
        g->level.player_faces_left = false;
    } else if (dx < 0) {
        g->level.player_faces_left = true;
    }

    game_append_history(g, time);

    for (int i = 0; i < g->level.entities.num_entities; i++) {
        if (g->level.entities.entities[i].et == ET_PLAYER) {
            if (level_move_entity(&g->level, i, dx, dy)) {
                g->state = GS_ANIMATE;
                g->state_t = 0;
            }
        }
    }

    level_step(&g->level);
}

// basically just goes through state machine
void game_update(shared_data *shared_data, void *scene_data, double dt) {
    game *g = (game *)scene_data;

    g->state_t += dt;

    if (g->state == GS_VICTORY_FADE_OUT && g->state_t > 1) {
        // FADE OUT -> FADE IN

        game_set_state(g, GS_FADE_IN);
        shared_data->completed[shared_data->selected_level] = true;
        shared_data->selected_level++;
        g->s.on_focus(shared_data, g);
        shared_data->snow_offset_base += shared_data->snow_offset_current;
        shared_data->snow_offset_current = 0;

    } else if (g->state == GS_FADE_IN && g->state_t > 1) {
        // FADE IN -> NORMAL
        
        game_set_state(g, GS_NORMAL);

    } else if (g->state == GS_ANIMATE && g->state_t > step_time) {
        if (level_do_ice(&g->level)) {
            // ANIMATE -> MORE ANIMATE

            game_set_state(g, GS_ANIMATE);
            level_step(&g->level);
        } else if(level_check_victory(&g->level)) {
            // ANIMATE -> VICTORY FADE OUT

            Mix_PlayChannel(CS_WIN, g->audio->win, 0);
            game_set_state(g, GS_VICTORY_FADE_OUT);
        } else if (g->buffered_move_dx != 0 || g->buffered_move_dy != 0) {
            // ANIMATE -> MORE ANIMATE
            
            game_move_player(g, g->buffered_move_dx, g->buffered_move_dy, shared_data->time);
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
                game_move_player(g, dx, dy, shared_data->time);
            }

        } else if (reset) {
            g->s.on_focus(shared_data, g);
            Mix_PlayChannel(CS_LOSE, g->audio->lose, 0);
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
                Mix_PlayChannel(CS_UNDO, g->audio->undo, 0);
            }
        }
    }
}

void game_draw(shared_data *shared_data, void *scene_data, gef_context *gc, double dt) {
    game *g = (game *)scene_data;

    const int tsize = 64;

    const int level_w_px = tsize * g->level.tiles.w;
    const int level_h_px = tsize * g->level.tiles.h;

    int xo = gc->xres / 2 - level_w_px/2;
    int yo = gc->yres / 2 - level_h_px/2;

    xo = xo / tsize * tsize;
    yo = yo / tsize * tsize;

    int64_t t_start = get_us();

    float wipe_t = g->state_t;
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
  
    for (int i = (xo % tsize) - tsize; i < (gc->xres + tsize); i += tsize) {
        for (int j = 0; j < (gc->yres + tsize); j += tsize) {
            SDL_Rect to_rect = (SDL_Rect) {i, j, tsize, tsize};
            gef_draw_sprite(gc, clip_wall, to_rect);
        }
    }

    int64_t t_fill = get_us();

    if (g->state == GS_ANIMATE) {
        level_draw(&g->level, gc, xo, yo, g->state_t / step_time, shared_data->time);
    } else {
        level_draw(&g->level, gc, xo, yo, 1, shared_data->time);
    }

    int64_t t_level = get_us();
    

    if (shared_data->draw_snow) {
        snowflakes_draw(gc, gc->xres, gc->yres, shared_data->interp_time, shared_data->snow_offset_base + shared_data->snow_offset_current);
    }

    int64_t t_snow = get_us();

    if (g->state == GS_FADE_IN) {
        float downness = cm_slow_stop2(min(g->state_t * 2, 1));
        int y = downness * 100 - 50;
        int x = gc->xres / 2 - g->level.title_handle.w / 2;
        gef_draw_text(gc, g->level.title_handle, x, y);
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
        .backing_size = HISTORY_STARTING_SIZE,
        .length = 0,
        .records = malloc(HISTORY_STARTING_SIZE * sizeof(history_record)),
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
        h->records = realloc(h->records, sizeof(history_record) * h->backing_size);
    }
    h->records[h->length++] = r;
}

void game_append_history(game *g, float time) {
    #ifdef DEBUG_HISTORY
    printf("appending to history, current state:\n...\n");
    for (int i = max(g->history.length-2, 0); i < g->history.length; i++) {
        printf("t: %f\n", g->history.records[i].time);
        entity_vla_print(&g->history.records[i].v);
    }
    #endif
    if (g->history.length >= g->history.backing_size - 2) {
        g->history.backing_size *= 2;
        g->history.records = realloc(g->history.records, sizeof(history_record) * g->history.backing_size);        
    }

    history_record r = (history_record) {
        .time = time,
        .v = entity_vla_clone(&g->level.entities),
    };

    #ifdef DEBUG_HISTORY
    printf("\nnew record: t %f, v:\n", r.time);
    entity_vla_print(&r.v);
    #endif

    g->history.records[g->history.length++] = r;
}

bool game_undo(game *g, shared_data *shared_data) {
    #ifdef DEBUG_HISTORY
    printf("undoing, current state:\n...\n");
    for (int i = max(g->history.length-2, 0); i < g->history.length; i++) {
        printf("t: %f\n", g->history.records[i].time);
        entity_vla_print(&g->history.records[i].v);
    }
    #endif

    // already at the start
    if (g->history.length == 0) {
        return false;
    }

    entity_vla_destroy(&g->level.entities);
    g->history.length--;
    shared_data->time = g->history.records[g->history.length].time;
    g->level.entities = g->history.records[g->history.length].v;
    

    Mix_RewindMusic();
    Mix_SetMusicPosition(shared_data->time);

    #ifdef DEBUG_HISTORY
    printf("undoing, new state:\n...\n");
    for (int i = max(g->history.length-2, 0); i < g->history.length; i++) {
        printf("t: %f\n", g->history.records[i].time);
        entity_vla_print(&g->history.records[i].v);
    }
    #endif

    return true;
}

void game_on_focus(shared_data *shared_data, void *scene_data) {
    game *g = (game *)scene_data;
    
    history_erase(&g->history, shared_data->time);
    level_destroy(&g->level);

    font_handle title_font = gef_load_font("assets/Hack-Regular.ttf", 36); // leak
    level_init(&g->level, shared_data->levels[shared_data->selected_level], &shared_data->gc, title_font, shared_data->selected_level);
    game_set_state(g, GS_FADE_IN);

}

game game_init(audio *audio, shared_data *shared_data) {
    game g = {0};
    g.history = history_init();

    g.s = (scene_interface) {
        .draw = game_draw,
        .handle_input = game_handle_input,
        .on_focus = game_on_focus,
        .update = game_update,
    };

    //g.player_faces_left = true;
    g.audio = audio;
    g.state = GS_NORMAL;
    g.state_t = 0;


    return g;
}


 