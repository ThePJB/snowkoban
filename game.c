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
        } else {
            return;
        }

        game_try_move_player(g, dx, dy);
        
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
            } else if (*current_pos == 't') {
                t = TT_SNOW | TT_TARGET;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'b') {
                t = TT_SNOW | TT_BOX;
                grid_set(g->current_level, &t, i, j);
            } else if (*current_pos == 'B') {
                t = TT_ICE | TT_BOX;
                grid_set(g->current_level, &t, i, j);
            }
            i++;
        }
        current_pos++;
    }
};


game game_init() {
    game g = {0};
    g.s = (scene_interface) {
        .draw = game_draw,
        .handle_input = game_handle_input,
    };

    game_load_level_from_str(&g, test_level_str);

    for (int i = 0; i < g.current_level.h; i++) {
        for (int j = 0; j < g.current_level.w; j++) {
            tile_type t;
            grid_get(g.current_level, &t, j, i);
        }
    }

    return g;
}