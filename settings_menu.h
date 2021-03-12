#pragma once

#include "scene.h"
#include "gef.h"

typedef struct {
    scene_interface s;
    int selection;
} settings_menu;

settings_menu settings_menu_init(gef_context *gc);
