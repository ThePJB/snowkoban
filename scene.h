#pragma once

#include <SDL.h>
#include "gef.h"
#include <stdbool.h>

typedef enum {
    SCENE_MAIN_MENU,
    SCENE_LEVEL_MENU,
    SCENE_GAME,
    NUM_SCENES,
} scene_index;

#define NUM_LEVELS 50


struct shared_data;

// the intention is that the void* points to this and that the rest of the structure
// e.g. game, menu, etc follows in memory
typedef struct {
    void (*draw)(struct shared_data *shared_data, void *, gef_context *gc, double dt);
    void (*handle_input)(struct shared_data *shared_data, void *, SDL_Event);
    void (*on_focus)(struct shared_data *shared_data, void *);
} scene_interface;

typedef struct shared_data {
    bool keep_going;
    scene_index current_scene;
    const char** levels;
    int num_levels;
    int selected_level;
    bool completed[NUM_LEVELS];
    float time;
    float interp_time;
} shared_data;