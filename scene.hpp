#pragma once

#include <SDL2/SDL.h>
#include "gef.hpp"
#include "audio.hpp"
#include "world.hpp"
#include <stdbool.h>

typedef enum {
    SCENE_MAIN_MENU,
    SCENE_LEVEL_MENU,
    SCENE_SETTINGS_MENU,
    SCENE_GAME,
    NUM_SCENES,
} scene_index;

#define NUM_LEVELS 50

// ui style
struct style {
    colour background = gef_rgb(100, 100, 200);
    colour pane = gef_rgba(100, 100, 100, 150);
    colour highlight = gef_rgb(255, 255, 0);
    colour btn_colour = gef_rgb(80, 150, 220);
    colour btn_line_colour = gef_rgb(100, 100, 100);

    int line = 8;

    bmp_font game_font;
    bmp_font_settings small;
    bmp_font_settings big;

    style() {};

    style(gef_context *gc) {
        game_font = gef_load_bmp_font(gc, "assets/custombold.png", 7, 8);

        // base off xres, yres
        small = (bmp_font_settings) {.scale = 3, .spacing = 1};
        big = (bmp_font_settings) {.scale = 6, .spacing = 1};
    }
};

struct shared_data {
    // subsystems (also settings in here)
    gef_context gc;
    audio a = audio_init();

    // setting stuff
    bool draw_snow = true;
    int max_scale = 8;

    bool keep_going = true;
    scene_index current_scene = SCENE_MAIN_MENU;
    
    float time = 0;
    float interp_time = 0;

    float snow_offset_base = 0;
    float snow_offset_current = 0;

    style game_style;

    int world_idx = 0;
    int num_worlds = 5;
    int level_idx = 0;

    // world stuff
    world worlds[5];

    shared_data(int xres, int yres, const char *title) {
        gc = gef_init(title, xres, yres);
        gef_load_atlas(&gc, "assets/snowkoban.png");
        game_style = style(&gc);
        worlds[0] = make_world1(gc.renderer);
    }

    world *current_world() {
        return &worlds[world_idx];
    }

    level_prototype *current_level_proto() {
        return &current_world()->lps.items[level_idx];
    }
};

struct scene {
    virtual void draw(shared_data *app_d, double dt) = 0;
    virtual void update(shared_data *app_d, double dt) = 0;
    virtual void on_focus(shared_data *app_d) = 0;
    virtual void handle_input(shared_data *app_d, SDL_Event e) = 0;
};