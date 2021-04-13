#pragma once

#include <SDL2/SDL.h>
#include "gef.hpp"
#include "audio.hpp"
#include "world.hpp"
#include <stdbool.h>

typedef enum {
    SCENE_NONE,
    SCENE_INSTRUCTIONS,
    SCENE_MAIN_MENU,
    SCENE_LEVEL_MENU,
    SCENE_SETTINGS_MENU,
    SCENE_GAME,
    NUM_SCENES,
} scene_index;

#define NUM_LEVELS 50

// ui style
struct style {
    colour background = gef_rgb(160, 160, 160);
    colour pane = gef_rgba(100, 100, 200, 150);
    colour highlight = gef_rgb(255, 255, 0);
    colour btn_colour = gef_rgb(80, 150, 220);
    colour btn_line_colour = gef_rgb(0, 255, 255);

    float wipe_time = 0.8;

    int line = 6;

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

enum level_menu_state {LMS_NORMAL, LMS_FADE_OUT_LEVEL, LMS_FADE_IN_LEVEL, LMS_FADE_OUT_WORLD, LMS_FADE_IN_WORLD};
enum wipe_dir {WD_LEFT, WD_RIGHT};

struct shared_data {
    // subsystems (also settings in here)
    gef_context gc;
    audio a = audio_init();

    // setting stuff
    bool draw_snow = true;
    int max_scale = 8;

    bool keep_going = true;
    scene_index current_scene = SCENE_INSTRUCTIONS;
    scene_index next_scene = SCENE_NONE;
    
    float time = 0;
    float abs_time = 0;
    float interp_time = 0;

    float snow_xo = 0;

    style game_style;

    int world_idx = 0;
    int level_idx = 0;

    level_menu_state lms = LMS_FADE_IN_WORLD;
    wipe_dir wd = WD_RIGHT;

    // world stuff
    vla<world> worlds;

    shared_data(int xres, int yres, const char *title) {
        gc = gef_init(title, xres, yres);
        gef_load_atlas(&gc, "assets/snowkoban.png");
        game_style = style(&gc);
        worlds.push(make_world1(&gc));
        worlds.push(make_world2(&gc));
        worlds.push(make_world3(&gc));
        worlds.push(make_world4(&gc));
        worlds.push(make_world5(&gc));
        worlds.push(make_workshop(&gc));
        worlds.push(make_world_mp2(&gc));
        worlds.push(make_world_mp(&gc));
    }

    world *current_world() {
        return &worlds.items[world_idx];
    }

    level_prototype *current_level_proto() {
        return &current_world()->lps.items[level_idx];
    }

    void set_scene(scene_index s) {
        this->next_scene = s;
    }
};

struct scene {
    virtual void draw(shared_data *app_d, double dt) = 0;
    virtual void update(shared_data *app_d, double dt) = 0;
    virtual void on_focus(shared_data *app_d) = 0;
    virtual void handle_input(shared_data *app_d, SDL_Event e) = 0;
};