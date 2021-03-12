#pragma once

#include "scene.h"

typedef struct {
    scene_interface s;
    int selx;
    int sely;

    text_handle level_numbers[NUM_LEVELS];

} level_menu;

level_menu level_menu_init(gef_context *gc, shared_data *shared_data);

// level menu init, w, h, etc