#include "main_menu.hpp"
#include "util.h"
#include "snowflakes.h"
#include "audio.h"
#include "button.h"
#include <stdbool.h>

void main_menu::handle_input(shared_data *app_d, SDL_Event e) {
    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool select = sym == SDLK_RETURN || sym == SDLK_SPACE;

        if (up) {
            if (selection > 0) {
                audio_play(&app_d->a, CS_MENU_MOVE);
                selection--;
            }
        } else if (down) {
            if (selection < 2) {
                audio_play(&app_d->a, CS_MENU_MOVE);
                selection++;
            }
        } else if (select) {
            audio_play(&app_d->a, CS_MENU_SELECT);
            if (selection == 0) {
                app_d->current_scene = SCENE_LEVEL_MENU;
            } else if (selection == 1) {
                app_d->current_scene = SCENE_SETTINGS_MENU;
            } else if (selection == 2) {
                app_d->keep_going = false;
            }
        }
    }
}

void main_menu::draw(shared_data *app_d, double dt) {
    const colour bg_colour = gef_rgb(150, 150, 200);

    const int num_buttons = 3;
    const int button_w = 400;
    const int button_h = 100;
    const int button_spacing = 50;

    gef_context *gc = &app_d->gc;

    // calculations to center the buttons
    const int screen_center_x = gc->xres/2;
    const int screen_center_y = gc->yres/2;
    const int menu_height = num_buttons * button_h + (num_buttons - 1) * button_spacing;

    gef_draw_rect(gc, bg_colour, 0, 0, gc->xres, gc->yres);

    if (app_d->draw_snow) {
        snowflakes_draw(gc, gc->xres, gc->yres, app_d->interp_time, app_d->snow_offset_base);
    }

    int menu_x = screen_center_x - button_w/2;
    int menu_y = screen_center_y - menu_height/2;
    
    SDL_Rect btn_rect = {menu_x, menu_y, button_w, button_h};

    button_generic_draw(gc, app_d->menu_button_style, btn_rect, "Play", selection == 0);
    btn_rect.y += button_h + button_spacing;
    button_generic_draw(gc, app_d->menu_button_style, btn_rect, "Settings", selection == 1);
    btn_rect.y += button_h + button_spacing;
    button_generic_draw(gc, app_d->menu_button_style, btn_rect, "Quit", selection == 2);    
}