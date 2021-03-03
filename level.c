#include "level.h"

#include "coolmath.h"

tile_prototype tile_prototypes[NUM_TT];

entity *level_get_entity(level *l, int idx) {
    return &l->entities.entities[idx];
}

void level_init(level *l, const char *level_str) {
    tile_prototypes[TT_SNOW] = (tile_prototype){"snow", " ptb", {16, 0, 16, 16}};
    tile_prototypes[TT_ICE] = (tile_prototype){"ice", "/PTB", {32, 0, 16, 16}};
    tile_prototypes[TT_HOLE] = (tile_prototype){"hole", "h", {112, 0, 16, 16}};
    tile_prototypes[TT_WALL] = (tile_prototype){"wall", "#", {0, 0, 16, 16}};
    tile_prototypes[TT_NONE] = (tile_prototype){"none", "", {112, 0, 16, 16}};

    
    entity_vla_init(&l->entities);

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
                l->title = malloc(sizeof(char) * i);
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

    printf("loading %s\n", l->title);

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
            for (tile_type t = 0; t < NUM_TT; t++) {
                const char *c = tile_prototypes[t].symbols;
                while(*c) {
                    if (*c == *current_pos) {
                        level_set_tile(l, i, j, t);
                        goto load_entities;
                    }
                    c++;
                }
            }

            load_entities:
            for (entity_type e = 0; e < NUM_ET; e++) {
                const char *c = entity_prototype_get(e).symbols;
                while(*c) {
                    if (*c == *current_pos) {
                        entity_vla_append(&l->entities, (entity) {
                            .et = e,
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
void level_draw(level *l, gef_context *gc, int xo, int yo, float t) {
    const int tsize = 64;
    
    for (int i = 0; i < l->tiles.w; i++) {
        for (int j = 0; j < l->tiles.h; j++) {
            SDL_Rect to_rect = {xo + tsize * i, yo + tsize * j, tsize, tsize};
            gef_draw_sprite(gc, tile_prototypes[level_get_tile(l, i, j)].clip, to_rect);
        }
    }

    // draw order: first draw receptacles
    for (int i = 0; i < l->entities.num_entities; i++) {
        entity *e = level_get_entity(l, i);
        if (e->et == ET_TARGET) {
            SDL_Rect to_rect = {
                xo + tsize * cm_lerp(e->x - e->dx, e->x, t),
                yo + tsize * cm_lerp(e->y - e->dy, e->y, t),
                tsize, tsize
            };
            gef_draw_sprite(gc, entity_prototype_get(e->et).clip, to_rect);
        }
    }

    // then draw other entity types
    for (int i = 0; i < l->entities.num_entities; i++) {
        entity *e = level_get_entity(l, i);
        if (e->et != ET_TARGET && e->et != ET_NONE) {
            SDL_Rect to_rect = {
                xo + tsize * cm_lerp(e->x - e->dx, e->x, t),
                yo + tsize * cm_lerp(e->y - e->dy, e->y, t),
                tsize, tsize
            };
            gef_draw_sprite(gc, entity_prototype_get(e->et).clip, to_rect);
        }
    }
}

bool level_is_entity_at(level *l, entity_type et, int x, int y) {
    for (int i = 0; i < l->entities.num_entities; i++) {
        entity *e = level_get_entity(l, i);
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
    entity_vla_destroy(&l->entities);
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
    for (int i = 0; i < l->entities.num_entities; i++) {
        entity *e1 = level_get_entity(l, i);
        if (e1->et == ET_TARGET) {
            bool this_target_is_covered = false;
            for (int j = 0; j < l->entities.num_entities; j++) {
                entity *e2 = level_get_entity(l, j);
                if (e2->et == ET_PRESENT && 
                        e1->x == e2->x &&
                        e1->y == e2->y) {
                    this_target_is_covered = true;
                    break;
                }
            }
            if (!this_target_is_covered) {
                return false;
            }

        }
    }
    return true;
}

// its kind of an invariant that theres only one 'moveable' entity at a location
// really the receptacle has nothing in common with the other slidy boys
int level_get_movable_entity_index_at(level *l, int x, int y) {
    for (int i = 0; i < l->entities.num_entities; i++) {
        entity *e = level_get_entity(l, i);
        if (e->et != ET_TARGET && e->x == x && e->y == y) {
            return i;
        }
    }
    return -1;
}

bool level_can_move_entity(level *l, int entity_idx, int dx, int dy) {
    entity *e = level_get_entity(l, entity_idx);
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
bool level_move_entity(level *l, int entity_idx, int dx, int dy) {
    if (!level_can_move_entity(l, entity_idx, dx, dy)) {
        return false;
    }
    entity *e = level_get_entity(l, entity_idx);
    e->dx = dx;
    e->dy = dy;
    int dest_x = e->x + dx;
    int dest_y = e->y + dy;
    tile_type t = level_get_tile(l, dest_x, dest_y);
    if (t == TT_NONE || t == TT_WALL) {
        return false;
    }
    int dest_entity_idx = level_get_movable_entity_index_at(l, dest_x, dest_y);
    if (dest_entity_idx == -1 || level_move_entity(l, dest_entity_idx, dx, dy)) {
        // no entity or its able to move
        e->x = dest_x;
        e->y = dest_y;
        // todo play sound as well, sound goes in tile prototype
        // todo ice
        return true;
    }
    return false;
}

bool level_do_ice(level *l) {
    bool any_ice = false;
    for (int i = 0; i < l->entities.num_entities; i++) {
        entity *e = &l->entities.entities[i];

        if (e->dx == 0 && e->dy == 0) {
            continue;
        }

        if (level_get_tile(l, e->x, e->y) == TT_ICE && level_can_move_entity(l, i, e->dx, e->dy)) {
            printf("do ice for %s: true\n", entity_prototype_get(e->et).name);
            level_move_entity(l, i, e->dx, e->dy);
            any_ice = true;
        } else {
            e->dx = 0;
            e->dy = 0;
            printf("do ice for %s: false\n", entity_prototype_get(e->et).name);
        }

    }
    return any_ice;
}