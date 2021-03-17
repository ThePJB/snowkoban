#pragma once

#include "gef.h"
#include "scene.hpp"
#include "level_menu.hpp"
#include "main_menu.hpp"
#include "settings_menu.hpp"
#include "game.hpp"
#include <stdbool.h>
#include <SDL_mixer.h>
#include "audio.h"


#define MAX_SCENES 16;


typedef struct {
    scene *scenes[NUM_SCENES];
    
    scene_index previous_scene;

    shared_data m_shared_data;
    
} application;

void application_init(application *app, int xres, int yres, bool do_start_level, int start_level);
void application_update(application *app, double dt);
void application_draw(application *app, double dt);
void application_handle_input(application *app);