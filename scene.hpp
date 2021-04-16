#pragma once

#include <SDL2/SDL.h>
#include "gef.hpp"
#include "audio.hpp"
#include "world.hpp"
#include "world_definitions.hpp"
#include <stdbool.h>
#include "util.hpp"

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
    colour background = gef_rgb(0x7A, 0x87, 0x99);
    colour pane = gef_rgba(100, 100, 200, 150);
    colour highlight = gef_rgb(0xFF, 0x6B, 0x6B);
    colour secondary = gef_rgb(0xD5, 0xFF, 0xAB);
    colour btn_colour = gef_rgb(0x1F, 0x35, 0x57);
    colour btn_line_colour = gef_rgb(0x12, 0x1F, 0x33);

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

enum transition_type{TRANS_NONE, TRANS_WIPE_LEFT, TRANS_WIPE_RIGHT, TRANS_ROTOZOOM};
const float trans_wipe_time = 0.5;
const float trans_rotozoom_time = 2.0;

// so use current scene and next scene
// deferred rendering for the transitions 
struct shared_data {
    // subsystems (also settings in here)
    gef_context gc;
    audio a = audio_init();

    // setting stuff
    bool draw_snow = true;
    int max_scale = 8;

    bool keep_going = true;
    
    // transitional stuff
    scene_index current_scene = SCENE_INSTRUCTIONS;
    scene_index next_scene = SCENE_NONE;
    transition_type transition = TRANS_NONE;
    float trans_t_max = 0;
    float trans_t_current = 0;
    bool do_on_focus = true;
    
    float time = 0;
    float abs_time = 0;
    float interp_time = 0;

    float snow_xo = 0;

    style game_style;

    int world_idx = 0;
    int level_idx = 0;

    // world stuff
    vla<world> worlds;

    shared_data(int xres, int yres, const char *title) {
        gc = gef_init(title, xres, yres);
        gef_load_atlas(&gc, "assets/snowkoban.png");
        game_style = style(&gc);
        worlds = make_worlds(&gc); // leak first worlds whatever
        load();
    }

    world *current_world() {
        return &worlds.items[world_idx];
    }

    level_prototype *current_level_proto() {
        return &current_world()->lps.items[level_idx];
    }

    void set_scene(scene_index s, transition_type trans_type, float trans_time) {
        if (this->next_scene != SCENE_NONE) {
            this->current_scene = this->next_scene;
        }
        this->next_scene = s;
        this->trans_t_max = trans_time;
        this->trans_t_current = 0;
        this->transition = trans_type;
        this->do_on_focus = true;
    }

    const char* save_path = "./save.file";
    const char save_lut[64] = "1234567890-=!@#$%^&*()_+qwertyuiop[]QWERTYUIOP{}asdfghjkl;ASDFG";

    void save() {
        FILE *file_ptr = fopen(save_path, "wb");
        if (file_ptr == NULL) {
            printf("couldn't open %s for saving\n", save_path);
            return;
        }
        const int num_levels = worlds.acc([](world w){return w.lps.length;});
        int level_num = 0;
        int world_num = 0;

        int i = 0;
        printf("saving ");
        for (world *w = worlds.begin(); worlds.is_next(); w = worlds.next()) {
            for (level_prototype *l = w->lps.begin(); w->lps.is_next(); l = w->lps.next()) {
                printf("saving progress for %s\n", l->title);
                printf("%c", l->complete ? '1' : '0');
                fprintf(file_ptr, "%c", l->complete ? '1' : '0');
                i++;
            }
        }
        printf("\n");
        fclose(file_ptr);
    }


    void load() {
        char *save_data;
        if (!check_slurp(save_path, &save_data)) {
            printf("no save file %s\n", save_path);
            return;
        }
        printf("found save file %s, loaded %s\n", save_path, save_data);

        int i = 0;
        for (world *w = worlds.begin(); worlds.is_next(); w = worlds.next()) {
            for (level_prototype *l = w->lps.begin(); w->lps.is_next(); l = w->lps.next()) {
                printf("i = %d\n", i);
                if (save_data[i] == '1') {
                    printf("its completed\n");
                    l->complete = true;
                }
                i++;
            }
        }
        free(save_data);
    }
};

struct scene {
    virtual void draw(shared_data *app_d, double dt) = 0;
    virtual void update(shared_data *app_d, double dt) = 0;
    virtual void on_focus(shared_data *app_d) = 0;
    virtual void handle_input(shared_data *app_d, SDL_Event e) = 0;
};