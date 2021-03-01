#include "scene.h"
#include "game.h"
#include "util.h"
#include "coolmath.h"
#include "snowflakes.h"
#include "level.h"

void history_erase(history *h, float t);

void game_update(shared_data *shared_data, void *scene_data, double dt) {
    game *g = (game *)scene_data;

    g->state_t += dt;

    if (g->state == GS_VICTORY_FADE_OUT) {
        if (g->state_t > 1) {
            g->state = GS_FADE_IN;
            g->state_t = 0;
            shared_data->completed[shared_data->selected_level] = true;
            shared_data->selected_level++;
            g->s.on_focus(shared_data, g);
        }
    } else if (g->state == GS_FADE_IN) {
        if (g->state_t > 1) {
            g->state = GS_NORMAL;
            g->state_t = 0;
        }
    }
}

void game_handle_input(shared_data *shared_data, void *scene_data, SDL_Event e) {
    game *g = (game *)scene_data;
    

    if (g->state == GS_VICTORY_FADE_OUT) return;

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
                //g->player_faces_left = true;
            } else if (right) {
                dx = 1;
                //g->player_faces_left = false;
            }

            game_append_history(g, shared_data->time);

            for (int i = 0; i < g->level.entities.num_entities; i++) {
                if (g->level.entities.entities[i].et == ET_PLAYER) {
                    if (level_move_entity(&g->level, i, dx, dy)) {
                        printf("append history\n");
                    }
                }
            }

            if (level_check_victory(&g->level)) {
                Mix_PlayChannel(CS_WIN, g->audio->win, 0);
                g->state = GS_VICTORY_FADE_OUT;
                g->state_t = 0;
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

    float wipe_t = 0;
    if (g->state == GS_VICTORY_FADE_OUT) {
        wipe_t = g->state_t;
        wipe_t = cm_slow_start2(wipe_t);
        xo += wipe_t * gc->xres;
    } else if (g->state == GS_FADE_IN) {
        wipe_t = g->state_t;
        wipe_t = cm_slow_start2(1 - wipe_t);
        xo += -1 * wipe_t * gc->xres;
    }

    level_draw(&g->level, gc, xo, yo);

    SDL_Rect clip_wall = {0, 0, 16, 16};

    for (int i = (xo % tsize) - tsize; i < (gc->xres + 1) * tsize; i += tsize) {
        for (int j = 0; j < (gc->yres + 1) * tsize; j += tsize) {
            SDL_Rect to_rect = (SDL_Rect) {i, j, tsize, tsize};
            gef_draw_sprite(gc, clip_wall, to_rect);
        }
    }

    level_draw(&g->level, gc, xo, yo);

    snowflakes_draw(gc, gc->xres, gc->yres, shared_data->interp_time);
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
    if (g->history.backing_size == g->history.length - 1) {
        g->history.backing_size *= 2;
        g->history.records = realloc(g->history.records, sizeof(history_record) * g->history.backing_size);        
    }

    history_record r = (history_record) {
        .time = time,
        .v = entity_vla_clone(&g->level.entities),
    };

    g->history.records[g->history.length++] = r;
}

bool game_undo(game *g, shared_data *shared_data) {

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

    return true;
}

void game_on_focus(shared_data *shared_data, void *scene_data) {
    game *g = (game *)scene_data;
    
    history_erase(&g->history, shared_data->time);
    level_destroy(&g->level);
    level_init(&g->level, shared_data->levels[shared_data->selected_level]);
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


 