#include "level_menu.h"
#include "util.h"
#include "snowflakes.h"
#include "audio.h"
#include <stdbool.h>

void level_menu_on_focus(shared_data *shared_data, void *scene_data) {
    return;
}

void level_menu_update(shared_data *shared_data, void *scene_data, double dt) {
    return;
}

void level_menu_handle_input(shared_data *shared_data, void *scene_data, SDL_Event e) {
    const int w = 10;
    const int h = 5;

    level_menu *level_menu_data = (level_menu*)scene_data;

    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        if (sym == SDLK_ESCAPE) {
            shared_data->current_scene = SCENE_MAIN_MENU;
            return;
        }

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool left = sym == SDLK_LEFT || sym == SDLK_a || sym == SDLK_h;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool right = sym == SDLK_RIGHT || sym == SDLK_d || sym == SDLK_l;
        bool select = sym == SDLK_RETURN || sym == SDLK_SPACE;

        if (up) {
            level_menu_data->sely = (level_menu_data->sely - 1 + h) % h; 
            audio_play(&shared_data->a, CS_MENU_MOVE);
        } else if (down) {
            level_menu_data->sely = (level_menu_data->sely + 1) % h; 
            audio_play(&shared_data->a, CS_MENU_MOVE);
        } else if (right) {
            level_menu_data->selx = (level_menu_data->selx + 1) % w; 
            audio_play(&shared_data->a, CS_MENU_MOVE);
        } else if (left) {
            level_menu_data->selx = (level_menu_data->selx - 1 + w) % w; 
            audio_play(&shared_data->a, CS_MENU_MOVE);
        } else if (select) {
            audio_play(&shared_data->a, CS_MENU_SELECT);
            shared_data->selected_level = level_menu_data->selx + level_menu_data->sely * w;
            shared_data->current_scene = SCENE_GAME;
        }
    }
}

void level_menu_draw(shared_data *shared_data, void *scene_data, gef_context *gc, double dt) {
    level_menu *level_menu_data = (level_menu*)scene_data;

    const int w = 10;
    const int h = 5;

    const int btn_area_x = 1200;
    const int btn_area_y = 600;

    const int btn_pad = 6;
    const int highlight_size = 6;

    const int btn_x = btn_area_x / w - (btn_pad);
    const int btn_y = btn_area_y / h - (btn_pad);

    const int area_start_x = gc->xres/2 - btn_area_x/2;
    const int area_start_y = gc->yres/2 - btn_area_y/2;

    const colour highlight_colour = gef_rgb(255, 255, 0);
    const colour button_colour = gef_rgb(100, 100, 100);
    const colour button_win_colour = gef_rgb(100, 100, 255);
    const colour background_colour = gef_rgb(150, 150, 150);

    gef_draw_rect(gc, background_colour, 0, 0, gc->xres, gc->yres);


    if (shared_data->draw_snow) {
        snowflakes_draw(gc, gc->xres, gc->yres, shared_data->interp_time, shared_data->snow_offset_base);
    }

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            const int level_index = i + j*w;

            const int curr_x = area_start_x + btn_pad + (btn_pad + btn_x) * i;
            const int curr_y = area_start_y + btn_pad + (btn_pad + btn_y) * j;


            if (level_menu_data->selx == i && level_menu_data->sely == j) {
                // draw highlight
                gef_draw_rect(gc, highlight_colour, 
                    curr_x - highlight_size, curr_y - highlight_size, 
                    btn_x + 2*highlight_size, btn_y + 2*highlight_size
                );
            }

            if (shared_data->completed[level_index]) {
                gef_draw_rect(gc, button_win_colour, curr_x, curr_y, btn_x, btn_y);
            } else {
                gef_draw_rect(gc, button_colour, curr_x, curr_y, btn_x, btn_y);
            }

            text_handle t = level_menu_data->level_numbers[level_index];
            const int text_x = curr_x + btn_x/2 - t.w/2;
            const int text_y = curr_y + btn_y/2 - t.h/2;
            gef_draw_text(gc, level_menu_data->level_numbers[level_index], text_x, text_y);
        }
    }
}

level_menu level_menu_init(gef_context *gc, shared_data *shared_data) {
    font_handle level_font = gef_load_font("assets/Hack-Regular.ttf", 48);

    level_menu lm = (level_menu) {
        .s = (scene_interface) {
            .draw = level_menu_draw,
            .handle_input = level_menu_handle_input,
            .on_focus = level_menu_on_focus,
            .update = level_menu_update,
        },
        .selx = 0,
        .sely = 0,
    };
    for (int i = 0; i < NUM_LEVELS; i++) {
        //printf("make level number %d\n", i);
        char buf[5] = {0};
        strings_itoa(buf, i+1);
        lm.level_numbers[i] = gef_make_text(gc, level_font, buf, 255, 255, 255);
    }

    return lm;
}