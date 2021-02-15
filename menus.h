#pragma once

#include "scene.h"

typedef struct {
    scene_interface s;
    int selection;
    text_handle texts[3];

} main_menu;

main_menu main_menu_init(gef_context *gc);
//void main_menu_handle_input(application_data *ad, void *scene_data, SDL_Event e);
//void main_menu_draw(application_data *ad, void *scene_data, gef_context *gc);