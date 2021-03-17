#pragma once

#include <SDL.h>
#include "gef.h"
#include "audio.h"
#include "button.h"
#include "level_set.h"
#include <stdbool.h>



typedef enum {
    SCENE_MAIN_MENU,
    SCENE_LEVEL_MENU,
    SCENE_SETTINGS_MENU,
    SCENE_GAME,
    NUM_SCENES,
} scene_index;

#define NUM_LEVELS 50



// the intention is that the void* points to this and that the rest of the structure
// e.g. game, menu, etc follows in memory
typedef struct {
    void (*draw)(struct shared_data *shared_data, void *, gef_context *gc, double dt);
    void (*handle_input)(struct shared_data *shared_data, void *, SDL_Event);
    void (*on_focus)(struct shared_data *shared_data, void *);
    void (*update)(struct shared_data *shared_data, void *, double dt);
} scene_interface;


typedef struct shared_data {
    // subsystems (also settings in here)
    gef_context gc;
    audio a;

    // setting stuff
    bool draw_snow;
    int max_scale;

    bool keep_going;
    scene_index current_scene;
    
    float time;
    float interp_time;

    float snow_offset_base;
    float snow_offset_current;

    // style stuff
    button_style menu_button_style;
    font_handle title_font;

    // world stuff
    level_set worlds[10];
    int world_idx;
    int num_worlds;
    int level_idx;


} shared_data;

struct scene {
    virtual void draw(shared_data *app_d, double dt) = 0;
    virtual void update(shared_data *app_d, double dt) = 0;
    virtual void on_focus(shared_data *app_d) = 0;
    virtual void handle_input(shared_data *app_d, SDL_Event e) = 0;
};