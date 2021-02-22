#pragma once

#include <SDL_mixer.h>
#include "scene.h"

typedef struct {
    scene_interface s;
    int selection;
    text_handle texts[3];

    Mix_Chunk *move_noise;
    Mix_Chunk *select_noise;
    
} main_menu;

main_menu main_menu_init(gef_context *gc);

typedef struct {
    scene_interface s;
    int selx;
    int sely;

    Mix_Chunk *move_noise;
    Mix_Chunk *select_noise;

    text_handle level_numbers[NUM_LEVELS];

} level_menu;

level_menu level_menu_init(gef_context *gc, shared_data *shared_data);

// level menu init, w, h, etc