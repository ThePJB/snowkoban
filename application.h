#pragma once

#include "gef.h"
#include "scene.h"
#include "menus.h"
#include "game.h"
#include <stdbool.h>

#define MAX_SCENES 16;



typedef struct {
    gef_context gc;

    main_menu main_menu;
    game game;
    //all scenes
    // game
    // menus

    application_data ad;

    
    
} application;

void application_init(application *app, int xres, int yres, bool do_start_level, int start_level);
void application_update(application *app);
void application_draw(application *app);
void application_handle_input(application *app);