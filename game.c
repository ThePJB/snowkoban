#include "scene.h"
#include "game.h"

const char *test_level_str = 
    "########\n"
    "#####t##\n"
    "# b////#\n"
    "#  /B//#\n"
    "#  ////#\n"
    "#    /b#\n"
    "#p b#//#\n"
    "#     /#\n"
    "#      #\n"
    "#      #\n"
    "########\n";

const char *levels[] = {
    "#####\n"
    "## t#\n"
    "# b #\n"
    "#   #\n"
    "##p##\n"
    "#####\n",
    
    "######\n"
    "#t t##\n"
    "##   #\n"
    "##b  #\n"
    "## b #\n"
    "##   #\n"
    "###p##\n"
    "######\n",

    // 2 steps forward 1 step back
    "########\n"
    "# tb   #\n"
    "#p #   #\n"
    "# bt   #\n"
    "########\n",

    "#########\n"
    "######t##\n"
    "##/////##\n"
    "#p ///b/#\n"
    "# #////##\n"
    "#  ////##\n"
    "#      ##\n"
    "#########\n",

    "########\n"
    "#####t##\n"
    "#pb////#\n"
    "#  ////#\n"
    "#  ////#\n"
    "#     b#\n"
    "#      #\n"
    "########\n",

    "########\n"
    "####t###\n"
    "#//////#\n"
    "#pb//#/#\n"
    "#//////#\n"
    "#////#/#\n"
    "#/#////#\n"
    "#//////#\n"
    "########\n",

    "###########\n"
    "# bt#######\n"
    "# #/////bt#\n"
    "#  /#######\n"
    "#  /#######\n"
    "#  /#######\n"
    "#p /#######\n"
    "###/#######\n"
    "###########\n"
    "###########\n",

    // elementary block puzzle
    "###########\n"
    "#  b     t#\n"
    "#     /  t#\n"
    "#    #b#  #\n"
    "#p        #\n"
    "###########\n",

    
    // easy double block puzzle
    "###########\n"
    "# b    t  #\n"
    "#    /t # #\n"
    "#   #b#b# #\n"
    "#p     t  #\n"
    "#         #\n"
    "###########\n",

    
    // tricky double block puzzle
    "###########\n"
    "# b    t  #\n"
    "#    /t # #\n"
    "#   #b#b# #\n"
    "#p     t  #\n"
    "###########\n",

    // cross
    // tricky if you dont get blocking the block and ur just
    // thinking which order to put them in the receptacle
    "#########\n"
    "####t##t#\n"
    "#  #/## #\n"
    "#  /B/t #\n"
    "#  #/## #\n"
    "#  #b# b#\n"
    "#   p   #\n"
    "#########\n",

    // railgun
    "#########\n"
    "####t####\n"
    "####t####\n"
    "####t####\n"
    "#t  /   #\n"
    "# #b/ b #\n"
    "# # /   #\n"
    "# # /b  #\n"
    "#p# /   #\n"
    "####/## #\n"
    "####B   #\n"
    "####/ ###\n"
    "####/   #\n"
    "####/####\n"
    "#########\n",

    // loom
    "###############\n"
    "#  ///#///###\n"
    "#  ///////tt#\n"
    "# b//#////###\n"
    "# b///////tt#\n"
    "# b/////#/###\n"
    "#  ///////tt#\n"
    "# #///////###\n"
    "#  bbb      #\n"
    "#p          #\n"
    "#############\n",

    // send block back and forth over ice lake
    // u actually have to start by constraining a block
    "###############\n"
    "#           t #\n"
    "#          #  #\n"
    "#   p      b/ #\n"
    "#          #  #\n"
    "#  #//////////#\n"
    "#  B//////////#\n"
    "#  #//////////#\n"
    "#             #\n"
    "#            t#\n"
    "###############\n",

    // calculator
    // oo this might need like one way ice so u can push it laterally still
    "################\n"
    "#   T/T/T//T   #\n"
    "#   T/T/T///   #\n"
    "#   //T/////   #\n"
    "#   ////////   #\n"
    "#   ////////   #\n"
    "#   ////////   #\n"
    "#     bbbb     #\n"
    "#   ////////   #\n"
    "#   ////////   #\n"
    "#     bbbb     #\n"
    "#              #\n"
    "################\n",

    "###############\n"
    "#### bt########\n"
    "#### ##########\n"
    "#//#/#//////#/#\n"
    "#//////#//////#\n"
    "#/#///////////#\n"
    "##///////////##\n"
    "#p//////#//#//#\n"
    "##//////##///##\n"
    "#//////#//#///#\n"
    "#//#//////////#\n"
    "#///////////#/#\n"
    "###############\n",

};

void history_erase(history *h);

bool game_check_victory(game *g);
bool game_is_tile_walkable(game *g, int x, int y, int dx, int dy);

bool game_try_move_box(game *g, int x, int y, int dx, int dy) {
    tile_type t;

    grid_get(g->current_level, &t, x, y);

    if (game_is_tile_walkable(g, x + dx, y + dy, dx, dy)) {
        t ^= TT_BOX;
        grid_set(g->current_level, &t, x, y);
        x += dx;
        y += dy;
        grid_get(g->current_level, &t, x, y);
        t ^= TT_BOX;
        grid_set(g->current_level, &t, x, y);

        // try and slide
        if (t & TT_ICE) {
            game_try_move_box(g, x, y, dx, dy);
        }
        return true;
    }
    return false;
}

bool game_is_tile_walkable(game *g, int x, int y, int dx, int dy) {
    tile_type t;
    grid_get(g->current_level, &t, x, y);
    if (t & TT_WALL) return false;
    if (t & TT_BOX) return game_try_move_box(g, x, y, dx, dy);

    return true;
}



void game_try_move_player(game *g, int dx, int dy) {
    tile_type t;

    grid_get(g->current_level, &t, g->player_x, g->player_y);

    if (game_is_tile_walkable(g, g->player_x + dx, g->player_y + dy, dx, dy)) {
        t ^= TT_PLAYER;
        grid_set(g->current_level, &t, g->player_x, g->player_y);
        g->player_x += dx;
        g->player_y += dy;
        grid_get(g->current_level, &t, g->player_x, g->player_y);
        t ^= TT_PLAYER;
        grid_set(g->current_level, &t, g->player_x, g->player_y);

        // try and slide
        if (t & TT_ICE) {
            game_try_move_player(g, dx, dy);
        }
    }
}




void game_handle_input(application_data *ad, void *scene_data, SDL_Event e) {
    game *g = (game *)scene_data;

    if (e.type  == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool left = sym == SDLK_LEFT || sym == SDLK_a || sym == SDLK_h;
        bool right = sym == SDLK_RIGHT || sym == SDLK_d || sym == SDLK_l;
        bool reset = sym == SDLK_r;

        bool movement = up || down || left || right;

        if (movement) {
            game_append_history(g);

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
            game_try_move_player(g, dx, dy);
            if (game_check_victory(g)) {
                printf("u win\n");
                history_erase(&g->history);
                grid_delete(g->current_level);
                game_load_level_from_str(g, levels[++g->current_level_num]);
            }

        } else if (reset) {
            history_erase(&g->history);
            grid_delete(g->current_level);
            game_load_level_from_str(g, levels[g->current_level_num]);
        } else if (sym == SDLK_q) {
            history_erase(&g->history);
            grid_delete(g->current_level);
            game_load_level_from_str(g, levels[--g->current_level_num]);
        } else if (sym == SDLK_e) {
            history_erase(&g->history);
            grid_delete(g->current_level);
            game_load_level_from_str(g, levels[++g->current_level_num]);            
        } else if (sym == SDLK_u) {
            game_undo(g);
        }
    }
}

const SDL_Rect clip_wall = {0, 0, 16, 16};
const SDL_Rect clip_snow = {16, 0, 16, 16};
const SDL_Rect clip_ice = {32, 0, 16, 16};
const SDL_Rect clip_box = {48, 0, 16, 16};
const SDL_Rect clip_player = {64, 0, 16, 16};
const SDL_Rect clip_target = {80, 0, 16, 16};

void game_draw(application_data *ad, void *scene_data, gef_context *gc) {
    game *g = (game *)scene_data;
    for (int i = 0; i < g->current_level.w; i++) {
        for (int j = 0; j < g->current_level.h; j++) {
            const int tsize = 64;
            SDL_Rect to_rect = (SDL_Rect) {i*tsize, j*tsize, tsize, tsize};

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
            if (t & TT_TARGET) {
                gef_draw_sprite(gc, clip_target, to_rect);
            }
            if (t & TT_BOX) {
                gef_draw_sprite(gc, clip_box, to_rect);
            }
            if (t & TT_PLAYER) {
                gef_draw_sprite(gc, clip_player, to_rect);
            }
        }
    }
}

void game_load_level_from_str(game *g, const char *level_str) {
    int width;
    int height;
    int i = 0;
    int j = 0;

    const char *current_pos = level_str;

    // check and get dimensions
    while(*current_pos) {
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

    g->current_level = grid_init(sizeof(tile_type), width, j);
    current_pos = level_str;
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
                g->player_x = i;
                g->player_y = j;
                t = TT_SNOW | TT_PLAYER;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'P') {
                g->player_x = i;
                g->player_y = j;
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
            }
            i++;
        }
        current_pos++;
    }
    game_append_history(g);
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
    return true;
}

#define HISTORY_STARTING_SIZE 100

history history_init() {
    history h = (history) {
        .backing_size = HISTORY_STARTING_SIZE,
        .length = 1,
        .records = malloc(HISTORY_STARTING_SIZE * sizeof(history_record)),
    };

    h.records[0] = (history_record) {.t = TT_SENTINEL, 0, 0};

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

void game_append_history(game *g) {
    tile_type t;
    for (int i = 0; i < g->current_level.w; i++) {
        for (int j = 0; j < g->current_level.h; j++) {
            grid_get(g->current_level, &t, i, j);
            if (t & TT_BOX) {
                history_record r = (history_record) {
                    .t = TT_BOX,
                    .x = i,
                    .y = j,
                };
                history_append_record(&g->history, r);
            }
            if (t & TT_PLAYER) {
                history_record r = (history_record) {
                    .t = TT_PLAYER,
                    .x = i,
                    .y = j,
                };
                history_append_record(&g->history, r);
            }
        }
    }
    history_append_record(&g->history, (history_record) {.t = TT_SENTINEL});
    printf("history len %d\n", g->history.length);
}

void game_undo(game *g) {
    // already at the start
    if (g->history.length == 1) {
        return;
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
            grid_set(g->current_level, &t, i, j);
        }
    }

    int i = g->history.length - 2; // 1 for sentinel and 1 for length being index+1
    history_record r = g->history.records[i];
    while (r.t != TT_SENTINEL) {
        tile_type t;
        grid_get(g->current_level, &t, r.x, r.y);
        t ^= r.t;
        if (t & TT_PLAYER) {
            g->player_x = r.x;
            g->player_y = r.y;
        }
        grid_set(g->current_level, &t, r.x, r.y);
        i--;
        r = g->history.records[i];
    }
    g->history.length = i + 1;
}


game game_init(int starting_level_num) {
    game g = {0};
    g.s = (scene_interface) {
        .draw = game_draw,
        .handle_input = game_handle_input,
    };
    g.current_level_num = starting_level_num;
    game_load_level_from_str(&g, levels[g.current_level_num]);

    for (int i = 0; i < g.current_level.h; i++) {
        for (int j = 0; j < g.current_level.w; j++) {
            tile_type t;
            grid_get(g.current_level, &t, j, i);
        }
    }

    g.history = history_init();

    return g;
}


