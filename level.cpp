#include "level.hpp"

#include "coolmath.h"

tile_prototype tile_prototypes[NUM_TT];

void level_init(level *l, const char *level_str, gef_context *gc, font_handle font, shared_data *shared_data) {
    l->player_faces_left = false;

    tile_prototypes[TT_SNOW] = (tile_prototype){"snow", " ptbc", {16, 0, 16, 16}, CS_SNOW_FOOTSTEP};
    tile_prototypes[TT_ICE] = (tile_prototype){"ice", "/PTBC", {32, 16, 16, 16}, CS_SLIP};
    tile_prototypes[TT_HOLE] = (tile_prototype){"hole", "h", {112, 0, 16, 16}, CS_NONE};
    tile_prototypes[TT_WALL] = (tile_prototype){"wall", "#", {0, 0, 16, 16}, CS_NONE};
    tile_prototypes[TT_NONE] = (tile_prototype){"none", "", {112, 0, 16, 16}, CS_NONE};
    
    l->entities = vla<entity>();

    bool title = true;
    int width = 0;
    int height = 0;
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
                l->title = (char *)malloc(sizeof(char) * i);
                memcpy(l->title, level_str, sizeof(char) * i);
                l->title[i-1] = '\0';
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

    char buf[256] = {0};
    sprintf(buf, "%d - %d %s", shared_data->world_idx, shared_data->level_idx, l->title);
    l->title_handle = gef_make_text(gc, font, buf, 255, 255, 255);

    l->tiles = grid_init(sizeof(tile_type), width, j);
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
            // load tiles
            for (int t = 0; t < (int)NUM_TT; t++) {
                const char *c = tile_prototypes[t].symbols;
                while(*c) {
                    if (*c == *current_pos) {
                        level_set_tile(l, i, j, (tile_type)t);
                        goto load_entities;
                    }
                    c++;
                }
            }

            load_entities:
            for (int e = 0; e < (int)NUM_ET; e++) {
                const char *c = entity_prototype_get((entity_type)e).symbols;
                while(*c) {
                    if (*c == *current_pos) {
                        l->entities.push((entity) {
                            .et = (entity_type)e,
                            .x = i,
                            .y = j,
                            .dx = 0,
                            .dy = 0,
                        });
                    }

                    c++;
                }
            }

            i++;
        }
        current_pos++;
    }
}

// t from 0 to 1
void level_draw(level *l, gef_context *gc, int xo, int yo, int pxsize, float t, float time) {
    const int spritesheet_size = 16;
    const int tsize = spritesheet_size * pxsize;
    const float reflection_aspect_ratio = 1;
    const SDL_Rect ice_bg_clip = {32, 0, 16, 16};

    int player_frame = cm_frac(time) > 0.5;

    // first draw ice background
    for (int i = 0; i < l->tiles.w; i++) {
        for (int j = 0; j < l->tiles.h; j++) {
            if (level_get_tile(l, i, j) == TT_ICE) {
                SDL_Rect to_rect = {xo + tsize * i, yo + tsize * j, tsize, tsize};
                gef_draw_sprite(gc, ice_bg_clip, to_rect);
            }
        }
    }

    // then draw entity reflections
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->et != ET_TARGET && e->et != ET_NONE) {

            int reflection_offset = tsize - (2*pxsize*entity_prototype_get(e->et).base_h);
            SDL_Rect to_rect = {
                xo + tsize * cm_lerp(e->x - e->previous_dx, e->x, t),
                yo + tsize * cm_lerp(e->y - e->previous_dy, e->y, t) + reflection_offset,
                tsize, tsize * reflection_aspect_ratio
            };
            SDL_Rect from_rect = entity_prototype_get(e->et).clip;
            SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
            if (e->et == ET_PLAYER) {
                if (!l->player_faces_left) {
                    flip = (SDL_RendererFlip)(((int)flip) | ((int)SDL_FLIP_HORIZONTAL));
                }
                
                from_rect.y += player_frame * spritesheet_size;
            }
            gef_draw_sprite_ex(gc, from_rect, to_rect, 0, flip);
        }
    }

    // then draw tiles incl. ice fg    
    for (int i = 0; i < l->tiles.w; i++) {
        for (int j = 0; j < l->tiles.h; j++) {
            SDL_Rect to_rect = {xo + tsize * i, yo + tsize * j, tsize, tsize};
            gef_draw_sprite(gc, tile_prototypes[level_get_tile(l, i, j)].clip, to_rect);
        }
    }

    // then draw entities

    // draw order: first draw receptacles
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->et == ET_TARGET) {
            SDL_Rect to_rect = {
                xo + tsize * cm_lerp(e->x - e->previous_dx, e->x, t),
                yo + tsize * cm_lerp(e->y - e->previous_dy, e->y, t),
                tsize, tsize
            };
            gef_draw_sprite(gc, entity_prototype_get(e->et).clip, to_rect);

        }
    }

    // then draw other entity types
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->et != ET_TARGET && e->et != ET_NONE) {
            SDL_Rect to_rect = {
                xo + tsize * cm_lerp(e->x - e->previous_dx, e->x, t),
                yo + tsize * cm_lerp(e->y - e->previous_dy, e->y, t),
                tsize, tsize
            };
            SDL_Rect from_rect = entity_prototype_get(e->et).clip;
            SDL_RendererFlip flip = SDL_FLIP_NONE;
            if (e->et == ET_PLAYER) {
                if (!l->player_faces_left) {
                    //flip |= SDL_FLIP_HORIZONTAL;
                    flip = (SDL_RendererFlip)(((int)flip) | ((int)SDL_FLIP_HORIZONTAL));
                }
                from_rect.y += player_frame * spritesheet_size;
            }
            gef_draw_sprite_ex(gc, from_rect, to_rect, 0, flip);
        }
    }
}

bool level_is_entity_at(level *l, entity_type et, int x, int y) {
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->et == et && e->x == x && e->y == y) {
            return true;
        }
    }
    return false;
}

void level_destroy(level *l) {
    grid_delete(l->tiles);
    if (l->title) {
        free(l->title);
        l->title = 0;
    }

    l->entities.destroy();

    gef_destroy_text(l->title_handle);
}

tile_type level_get_tile(level *l, int x, int y) {
    tile_type t;
    if (grid_get(l->tiles, &t, x, y)) {
        return t;
    };
    return TT_NONE;
}

void level_set_tile(level *l, int x, int y, tile_type t) {
    grid_set(l->tiles, &t, x, y);
}

bool level_check_victory(level *l) {
    return l->entities.all([l](entity e1) {
        return e1.et != ET_TARGET || l->entities.any([e1](entity e2) {
            return e2.et == ET_PRESENT && e2.x == e1.x && e2.y == e1.y;
        });
    });
}

// its kind of an invariant that theres only one 'moveable' entity at a location
// really the receptacle has nothing in common with the other slidy boys
int level_get_movable_entity_index_at(level *l, int x, int y) {
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];
        if (e->et != ET_TARGET && e->x == x && e->y == y) {
            return i;
        }
    }
    return -1;
}

bool level_can_move_entity(level *l, int entity_idx, int dx, int dy) {
    entity *e = &l->entities.items[entity_idx];
    int dest_x = e->x + dx;
    int dest_y = e->y + dy;
    tile_type t = level_get_tile(l, dest_x, dest_y);
    if (t == TT_NONE || t == TT_WALL) {
        return false;
    }
    int dest_entity_idx = level_get_movable_entity_index_at(l, dest_x, dest_y);
    if (dest_entity_idx == -1 || level_can_move_entity(l, dest_entity_idx, dx, dy)) {
        return true;
    }
    return false;    
}

// stop doing it recursively, just do 1 step at a time, then it will animate easily
// and avoid the recursive sound tangly mess situation
// player cant move while stuff happening
bool level_move_entity(level *l, int entity_idx, int dx, int dy, audio *a) {
    if (!level_can_move_entity(l, entity_idx, dx, dy)) {
        return false;
    }
    entity *e = &l->entities.items[entity_idx];
    e->dx = dx;
    e->dy = dy;
    int dest_x = e->x + dx;
    int dest_y = e->y + dy;
    tile_type t = level_get_tile(l, dest_x, dest_y);

    audio_play(a, tile_prototypes[t].walk_sound);

    if (t == TT_NONE || t == TT_WALL) {
        return false;
    }
    int dest_entity_idx = level_get_movable_entity_index_at(l, dest_x, dest_y);
    if (dest_entity_idx == -1 || level_move_entity(l, dest_entity_idx, dx, dy, a)) {
        // no entity or its able to move
        // todo play sound as well, sound goes in tile prototype
        // todo ice
        return true;
    }
    return false;
}

void level_step(level *l) {
    l->entities.for_each_mut([](entity *e) {
        e->x += e->dx;
        e->y += e->dy;

        e->previous_dx = e->dx;
        e->previous_dy = e->dy;

        e->dx = 0;
        e->dy = 0;
    });
}

bool level_do_ice(level *l, audio *a) {
    bool any_ice = false;
    for (int i = 0; i < l->entities.length; i++) {
        entity *e = &l->entities.items[i];

        if (e->previous_dx == 0 && e->previous_dy == 0) {
            continue;
        }

        if (level_get_tile(l, e->x, e->y) == TT_ICE && level_can_move_entity(l, i, e->previous_dx, e->previous_dy)) {
            level_move_entity(l, i, e->previous_dx, e->previous_dy, a);
            any_ice = true;
        } else {
            //e->dx = 0;
            //e->dy = 0;
        }

    }
    return any_ice;
}