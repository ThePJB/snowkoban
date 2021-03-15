#pragma once

#include "scene.h"

typedef struct {
    scene_interface s;

} level_menu_new;

level_menu_new level_menu_new_init(gef_context *gc, shared_data *shared_data);
