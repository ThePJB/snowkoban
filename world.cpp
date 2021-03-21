#include "world.hpp"

level_prototype::level_prototype(const char* level_str, SDL_Renderer *r) {
    starting_entities = vla<entity>();

    bool parsing_title = true;
    int width = 0;
    int height = 0;
    int i = 0;
    int j = 0;

    const char *current_pos = level_str;
    const char *grid_start = 0;

    // check and get dimensions
    while(*current_pos) {
        // load the title first
        if (parsing_title) {
            i++;
            if (*current_pos == '\n') {
                title = (char *)malloc(sizeof(char) * i);
                memcpy((void *)title, level_str, sizeof(char) * i);
                title[i-1] = '\0';
                i = 0;
                parsing_title = false;
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
    tiles = grid<tile>(width, j);
    current_pos = grid_start;
    tile t;
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
                const char *c = tile_prototype_get(t).symbols;
                while(*c) {
                    if (*c == *current_pos) {
                        tiles.set(i, j, (tile)t);
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
                        starting_entities.push((entity) {
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
    preview = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 400, 400 );
    SDL_SetRenderTarget(r, preview);
    SDL_RenderClear(r);
    SDL_Rect tstrect = {100, 100, 100, 200};
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderDrawRect(r, &tstrect);
    // draw or whatever
    SDL_RenderPresent(r);
    SDL_SetRenderTarget(r, NULL);
}


world::world(const char *name, const char** level_strs, int n_levels, SDL_Renderer *r) {
    this->name = name;
    lps = vla<level_prototype>();

    for (int i = 0; i < n_levels; i++) {
        auto new_lp = level_prototype(level_strs[i], r);
        total_presents += new_lp.starting_entities.acc([](entity e){return e.et == ET_PRESENT ? 1 : 0;});
        lps.push(new_lp);
    }
}