#include "scene.h"
#include "game.h"
#include "util.h"
#include "coolmath.h"
#include "snowflakes.h"

void history_erase(history *h);

bool game_check_victory(game *g);
bool game_is_tile_walkable(game *g, int x, int y, int dx, int dy);

bool tile_moves(tile_type t) {
    return (t & TT_BOX) || (t & TT_CRATE) || (t & TT_PLAYER);
}

bool game_is_tile_walkable(game *g, int x, int y, int dx, int dy) {
    tile_type t;
    grid_get(g->current_level, &t, x, y);
    if (t & TT_WALL) return false;
    if (t & TT_BOX) return game_is_tile_walkable(g, x+dx, y+dy, dx, dy);
    if (t & TT_CRATE) return game_is_tile_walkable(g, x+dx, y+dy, dx, dy);

    return true;
}

void game_move_entity(game *g, int x, int y, int dx, int dy, tile_type entity_type, shared_data *shared_data) {
    tile_type t;

    grid_get(g->current_level, &t, x, y);

    if (game_is_tile_walkable(g, x + dx, y + dy, dx, dy)) {
        tile_type t;
        grid_get(g->current_level, &t, x + dx, y + dy);


        // its a box or something we can move
        // its annoying that we cant move it without knowing the type
        if (t & TT_BOX) game_move_entity(g, x + dx, y + dy, dx, dy, TT_BOX, shared_data);
        if (t & TT_CRATE) game_move_entity(g, x + dx, y + dy, dx, dy, TT_CRATE, shared_data);
        if (t & TT_PLAYER) game_move_entity(g, x + dx, y + dy, dx, dy, TT_PLAYER, shared_data);
    }

    if (game_is_tile_walkable(g, x + dx, y + dy, dx, dy)) {
        t ^= entity_type;
        grid_set(g->current_level, &t, x, y);
        x += dx;
        y += dy;
        grid_get(g->current_level, &t, x, y);
        if (t & TT_HOLE) {
            // return before re placing
            return;
        }
        t ^= entity_type;
        grid_set(g->current_level, &t, x, y);

        // try and slide
        if (t & TT_ICE) {
            game_move_entity(g, x, y, dx, dy, entity_type, shared_data);
            if (entity_type == TT_PLAYER) {
                Mix_PlayChannel(CS_SLIP, g->audio->slip, 0);
            }
        } else {
            if ((t & TT_TARGET) && (t & TT_BOX)) {
                if (!game_check_victory(g)) {
                    // this is a bit jank, victory sound will be played instead from handle input
                    // if its victory
                    // this probably breaks if you slide over it. well actually who cares if u dont land on it anyway
                    // well actually currently if it ends on an ice block will it not make the sound
                    Mix_PlayChannel(CS_MOVEFINAL, g->audio->movefinal, 0);
                }

                
            }
        }
        return;
    }
    return;
}



void game_handle_input(shared_data *shared_data, void *scene_data, SDL_Event e) {
    game *g = (game *)scene_data;

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
                g->player_faces_left = true;
            } else if (right) {
                dx = 1;
                g->player_faces_left = false;
            }
            tile_type t;

            for (int i = 0; i < g->current_level.w; i++) {
                for (int j = 0; j < g->current_level.h; j++) {
                    grid_get(g->current_level, &t, i, j);
                    if (t & TT_PLAYER) {
                        if (game_is_tile_walkable(g, i+dx, j+dy, dx, dy)) {
                            
                            // play current tile sound
                            if (t & TT_SNOW) {
                                Mix_PlayChannel(CS_LOSE, g->audio->snow_footstep, 0);
                            }
                            if (t & TT_ICE) {
                                Mix_PlayChannel(CS_SLIP, g->audio->slip, 0);
                            }
                            if (t & TT_HOLE) {
                                Mix_PlayChannel(CS_LOSE, g->audio->lose, 0);
                            }

                            // save undo
                            game_append_history(g, shared_data->time);

                            game_move_entity(g, i, j, dx, dy, TT_PLAYER, shared_data);

                            grid_get(g->current_level, &t, i+dx, j+dy);
                            
                            // play dest tile sound, i guess this still doesn't handle ice slip if its ' ///// '
                            // this is dumb because its only the next tile, so again doesnt account for slipping, needs to be in move entity.
                            // mm polyphony or do we just not care lol
                            // how many channels is it possible to have
                            if (t & TT_SNOW) {
                                Mix_PlayChannel(CS_LOSE, g->audio->snow_footstep, 0);
                            }
                            if (t & TT_ICE) {
                                Mix_PlayChannel(CS_SLIP, g->audio->slip, 0);
                            }
                            if (t & TT_HOLE) {
                                Mix_PlayChannel(CS_LOSE, g->audio->lose, 0);
                            }
                            
                        } else {
                            // could play a negatory noise to say that u cant move
                        }
                        goto break_player_moving_loop; // still not supporting multiple players
                    }
                }
            }
            break_player_moving_loop:
            if (game_check_victory(g)) {
                printf("u win\n");
                shared_data->completed[shared_data->selected_level] = true;
                shared_data->selected_level++;
                g->s.on_focus(shared_data, g);
            }



        } else if (reset) {
            g->s.on_focus(shared_data, g);
            Mix_PlayChannel(CS_LOSE, g->audio->lose, 0);
        } else if (sym == SDLK_q) {
            if (g->current_level_num == 0) {
                return;
            }
            shared_data->selected_level--;
            g->s.on_focus(shared_data, g);
        } else if (sym == SDLK_e) {
            if (g->current_level_num == shared_data->num_levels - 1) {
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

const SDL_Rect clip_wall = {0, 0, 16, 16};
const SDL_Rect clip_snow = {16, 0, 16, 16};
const SDL_Rect clip_ice = {32, 0, 16, 16};
const SDL_Rect clip_box = {48, 0, 16, 16};
const SDL_Rect clip_player[4] = {
    (SDL_Rect) {64, 0, 16, 16},
    (SDL_Rect) {64, 16, 16, 16},
    (SDL_Rect) {64, 0, 16, 16},
    (SDL_Rect) {64, 32, 16, 16},
};

const SDL_Rect clip_target = {80, 0, 16, 16};
const SDL_Rect clip_crate = {96, 0, 16, 16};
const SDL_Rect clip_hole = {112, 0, 16, 16};

const SDL_Rect clip_background_snow = {0, 96, 64, 64};


void game_draw(shared_data *shared_data, void *scene_data, gef_context *gc, double dt) {
    game *g = (game *)scene_data;

    const int tsize = 64;

    const int level_w_px = tsize * g->current_level.w;
    const int level_h_px = tsize * g->current_level.h;

    int xo = gc->xres / 2 - level_w_px/2;
    int yo = gc->yres / 2 - level_h_px/2;

    xo = xo / tsize * tsize;
    yo = yo / tsize * tsize;

    for (int i = 0; i < (gc->xres + 1) * tsize; i += tsize) {
        for (int j = 0; j < (gc->yres + 1) * tsize; j += tsize) {
            SDL_Rect to_rect = (SDL_Rect) {i, j, tsize, tsize};
            gef_draw_sprite(gc, clip_wall, to_rect);
        }
    }

    for (int i = 0; i < g->current_level.w; i++) {
        for (int j = 0; j < g->current_level.h; j++) {
            SDL_Rect to_rect = (SDL_Rect) {xo + i*tsize, yo + j*tsize, tsize, tsize};

            tile_type t;
            grid_get(g->current_level, &t, i, j);
            if (t & TT_SNOW) {
                gef_draw_sprite(gc, clip_snow, to_rect);
            }
            if (t & TT_WALL) {
                gef_draw_sprite(gc, clip_wall, to_rect);
            }
            if (t & TT_ICE) {
                gef_draw_sprite(gc, clip_ice, to_rect);
            }
            if (t & TT_HOLE) {
                gef_draw_sprite(gc, clip_hole, to_rect);
            }
            if (t & TT_TARGET) {
                gef_draw_sprite(gc, clip_target, to_rect);
            }
            if (t & TT_BOX) {
                gef_draw_sprite(gc, clip_box, to_rect);
            }
            if (t & TT_PLAYER) {
                SDL_RendererFlip flip = g->player_faces_left ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

                float x = cm_frac(shared_data->time / 2);
                if (x < 0.25) {
                    gef_draw_sprite_ex(gc, clip_player[0], to_rect, 0, flip);
                } else if (x < 0.5) {
                    gef_draw_sprite_ex(gc, clip_player[1], to_rect, 0, flip);
                } else if (x < 0.75) {
                    gef_draw_sprite_ex(gc, clip_player[0], to_rect, 0, flip);
                } else {
                    gef_draw_sprite_ex(gc, clip_player[1], to_rect, 0, flip);
                }
            }
            if (t & TT_CRATE) {
                gef_draw_sprite(gc, clip_crate, to_rect);
            }
        }
    }


    snowflakes_draw(gc, gc->xres, gc->yres, shared_data->time);
}

void game_load_level_from_str(game *g, const char *level_str, shared_data *shared_data) {
    bool title = true;
    int width;
    int height;
    int i = 0;
    int j = 0;

    const char *current_pos = level_str;
    const char *grid_start = 0;

    // check and get dimensions
    while(*current_pos) {
        // load the title first
        if (title) {
            i++;
            if (*current_pos == '\n') {
                g->current_level_name = malloc(sizeof(char) * i);
                memcpy(g->current_level_name, level_str, sizeof(char) * i);
                g->current_level_name[i-1] = '\0';
                i = 0;
                title = false;
            }
            current_pos++;
            continue;
        }

        if (!grid_start) {
            grid_start = current_pos;
        }

        if (*current_pos == '\n') {
            if (j == 0) {
                width = i;
            } else {
                if (i == 0) {
                    // empty line, just ignore it
                    j--;
                } else if (width != i) {
                    printf("problem loading level: inconsistent width\n");
                }
            }
            i = 0;
            j++;
        } else {
            i++;
        }
        current_pos++;
    }

    printf("loading %s\n", g->current_level_name);

    g->current_level = grid_init(sizeof(tile_type), width, j);
    current_pos = grid_start;
    tile_type t;
    i = 0;
    j = 0;

    // fill grid
    while(*current_pos) {
        if (*current_pos == '\n') {
            i = 0;
            j++;
        } else {
            if (*current_pos == '#') {
                t = TT_WALL;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == ' ') {
                t = TT_SNOW;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == '/') {
                t = TT_ICE;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'p') {
                t = TT_SNOW | TT_PLAYER;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'P') {
                t = TT_ICE | TT_PLAYER;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 't') {
                t = TT_SNOW | TT_TARGET;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'T') {
                t = TT_ICE | TT_TARGET;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'b') {
                t = TT_SNOW | TT_BOX;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'B') {
                t = TT_ICE | TT_BOX;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'f') {
                t = TT_SNOW | TT_BOX | TT_TARGET;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'F') {
                t = TT_ICE | TT_BOX | TT_TARGET;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'c') {
                t = TT_SNOW | TT_CRATE;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'C') {
                t = TT_ICE | TT_CRATE;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'h') {
                t = TT_HOLE;
                grid_set(g->current_level, &t, i, j);
            }
            i++;
        }
        current_pos++;
    }
    game_append_history(g, shared_data->time);
};

bool game_check_victory(game *g) {
    tile_type t;
    for (int i = 0; i < g->current_level.w; i++) {
        for (int j = 0; j < g->current_level.h; j++) {
            grid_get(g->current_level, &t, i, j);
            if (t & TT_TARGET) {
                if (t& TT_BOX) {
                    // ok
                } else {
                    return false;
                }
            }
        }
    }
    Mix_PlayChannel(CS_WIN, g->audio->win, 0);
    return true;
}

#define HISTORY_STARTING_SIZE 100

history history_init() {
    history h = (history) {
        .backing_size = HISTORY_STARTING_SIZE,
        .length = 1,
        .records = malloc(HISTORY_STARTING_SIZE * sizeof(history_record)),
    };

    h.records[0] = (history_record) {.t = TT_SENTINEL, .time = 0};

    return h;
}

void history_erase(history *h) {
    h->length = 1;
}

void history_append_record(history *h, history_record r) {
    if (h->length == h->backing_size) {
        h->backing_size *= 2;
        h->records = realloc(h->records, sizeof(history_record) * h->backing_size);
    }
    h->records[h->length++] = r;
}

void game_append_history(game *g, float time) {
    tile_type t;
    for (int i = 0; i < g->current_level.w; i++) {
        for (int j = 0; j < g->current_level.h; j++) {
            grid_get(g->current_level, &t, i, j);
            tile_type mobile_t = 0;
            if (t & TT_BOX) mobile_t = TT_BOX;
            if (t & TT_PLAYER) mobile_t = TT_PLAYER;
            if (t & TT_CRATE) mobile_t = TT_CRATE;
            if (mobile_t == 0) { continue;}

            history_record r = (history_record) {
                .t = mobile_t,
                .x = i,
                .y = j,
            };
            history_append_record(&g->history, r);
        }
    }
    history_append_record(&g->history, (history_record) {.t = TT_SENTINEL, .time = time});
}

bool game_undo(game *g, shared_data *shared_data) {
    // already at the start
    if (g->history.length == 1) {
        return false;
    }

    // erase mobile entities
    tile_type t;
    for (int i = 0; i < g->current_level.w; i++) {
        for (int j = 0; j < g->current_level.h; j++) {
            grid_get(g->current_level, &t, i, j);
            if (t & TT_BOX) {
                t ^= TT_BOX;
            }
            if (t & TT_PLAYER) {
                t ^= TT_PLAYER;
            }
            if (t & TT_CRATE) {
                t ^= TT_CRATE;
            }
            grid_set(g->current_level, &t, i, j);
        }
    }

    int i = g->history.length - 2; // 1 for sentinel and 1 for length being index+1
    history_record r = g->history.records[i];
    while (r.t != TT_SENTINEL) {
        tile_type t;
        grid_get(g->current_level, &t, r.x, r.y);
        t ^= r.t;
        grid_set(g->current_level, &t, r.x, r.y);
        i--;
        r = g->history.records[i];
    }
    g->history.length = i + 1;
    shared_data->time = g->history.records[i].time;

    Mix_RewindMusic();
    Mix_SetMusicPosition(shared_data->time);

    return true;
}

void game_on_focus(shared_data *shared_data, void *scene_data) {
    game *g = (game *)scene_data;
    
    g->current_level_num = shared_data->selected_level;
    history_erase(&g->history);
    grid_delete(g->current_level);
    game_load_level_from_str(g, shared_data->levels[g->current_level_num], shared_data);
}

game game_init(audio *audio, shared_data *shared_data) {
    game g = {0};
    g.history = history_init();

    g.s = (scene_interface) {
        .draw = game_draw,
        .handle_input = game_handle_input,
        .on_focus = game_on_focus,
    };

    g.player_faces_left = true;
    g.audio = audio;

    return g;
}


 