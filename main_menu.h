#pragma once

#include <SDL_mixer.h>
#include "scene.h"

typedef struct {
    scene_interface s;
    int selection;
} main_menu;

main_menu main_menu_init(gef_context *gc);