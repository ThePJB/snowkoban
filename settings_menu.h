#pragma once

#include "scene.h"
#include "gef.h"
#include "SDL_mixer.h"


typedef struct {
    scene_interface s;
    int selection;
    text_handle texts[2];

    Mix_Chunk *move_noise;
    Mix_Chunk *select_noise;
    
} settings_menu;

settings_menu settings_menu_init(gef_context *gc);
