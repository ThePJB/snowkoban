#pragma once

#include "gef.h"
#include "scene.h"
#include "menus.h"
#include "game.h"
#include <stdbool.h>
#include <SDL_mixer.h>
#include "audio.h"


#define MAX_SCENES 16;


typedef struct {
    scene_interface* scenes[NUM_SCENES];
    
    scene_index previous_scene;

    audio audio;

    shared_data shared_data;
    
} application;

void application_init(application *app, int xres, int yres, bool do_start_level, int start_level);
void application_update(application *app, double dt);
void application_draw(application *app, double dt);
void application_handle_input(application *app);