#pragma once

#include <SDL.h>
#include "gef.h"
#include <stdbool.h>

struct application_data;

// the intention is that the void* points to this and that the rest of the structure
// e.g. game, menu, etc follows in memory
typedef struct {
    void (*draw)(struct application_data *ad, void *, gef_context *gc);
    void (*handle_input)(struct application_data *ad, void *, SDL_Event);
} scene_interface;

typedef struct application_data {
    bool keep_going;
    scene_interface *current_scene;
} application_data;