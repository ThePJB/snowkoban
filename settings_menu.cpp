#include "settings_menu.hpp"
#include "snowflakes.h"
#include "util.h"

void settings_menu::draw(shared_data *app_d, double dt) {
    const int button_w = 600;
    const int button_h = 100;
    const int button_spacing = 50;
    const int highlight_size = 6;
    const int num_buttons = 6;

    gef_context *gc = &app_d->gc;


    // calculations to center the buttons
    const int screen_center_x = gc->xres/2;
    const int screen_center_y = gc->yres/2;
    const int menu_height = num_buttons * button_h + (num_buttons - 1) * button_spacing;

    const colour background_colour = gef_rgb(150, 150, 150);

    gef_draw_rect(gc, background_colour, 0, 0, gc->xres, gc->yres);

    if (app_d->draw_snow) {
        snowflakes_draw(gc, gc->xres, gc->yres, app_d->interp_time, app_d->snow_offset_base);
    }
    
    int menu_x = screen_center_x - button_w/2;
    int menu_y = screen_center_y - menu_height/2;

    SDL_Rect btn_rect = {menu_x, menu_y, button_w, button_h};

    int i = 0;
    button_bool_draw(gc, app_d->menu_button_style, btn_rect, "Snow", app_d->draw_snow, selection == i++);
    btn_rect.y += button_h + button_spacing;
    button_int_draw(gc, app_d->menu_button_style, btn_rect, "Max Scale", app_d->max_scale, selection == i++);
    btn_rect.y += button_h + button_spacing;
    button_generic_draw(gc, app_d->menu_button_style, btn_rect, "Resolution", selection == i++);
    btn_rect.y += button_h + button_spacing;
    button_percent_draw(gc, app_d->menu_button_style, btn_rect, "Music Volume", app_d->a.bgm_volume, selection == i++);
    btn_rect.y += button_h + button_spacing;
    button_percent_draw(gc, app_d->menu_button_style, btn_rect, "SFX Volume", app_d->a.sfx_volume, selection == i++);
    btn_rect.y += button_h + button_spacing;
    button_generic_draw(gc, app_d->menu_button_style, btn_rect, "Back", selection == i++);
}

void settings_menu::handle_input(shared_data *app_d, SDL_Event e) {
    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        if (sym == SDLK_ESCAPE) {
            app_d->current_scene = SCENE_MAIN_MENU;
        }

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool left = sym == SDLK_LEFT || sym == SDLK_a || sym == SDLK_h;
        bool right = sym == SDLK_RIGHT || sym == SDLK_d || sym == SDLK_l;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool select = sym == SDLK_RETURN || sym == SDLK_SPACE;

        const int num_buttons = 6;

        if (up) {
            if (selection > 0) {
                audio_play(&app_d->a, CS_MENU_MOVE);
                selection--;
            }
        } else if (down) {
            if (selection < num_buttons - 1) {
                audio_play(&app_d->a, CS_MENU_MOVE);
                selection++;
            }
        } else if (select || left || right) {
            audio_play(&app_d->a, CS_MENU_SELECT);
            if (selection == 0) {
                app_d->draw_snow = !app_d->draw_snow;
            } else if (selection == 1) {
                //max scale
                if (left) {
                    app_d->max_scale = max(app_d->max_scale - 1, 1);
                } else if (right) {
                    app_d->max_scale = min(app_d->max_scale + 1, 20);
                }
            } else if (selection == 2) {
                // res idk yet
            } else if (selection == 3) {
                // music volume
                if (left) {
                    audio_set_bgm_volume(&app_d->a, max(app_d->a.bgm_volume - 0.04, 0));
                } else if (right) {
                    audio_set_bgm_volume(&app_d->a, min(app_d->a.bgm_volume + 0.04, 1));
                }
            } else if (selection == 4) {
                //sfx volume
                if (left) {
                    audio_set_sfx_volume(&app_d->a, max(app_d->a.sfx_volume - 0.04, 0));
                } else if (right) {
                    audio_set_sfx_volume(&app_d->a, min(app_d->a.sfx_volume + 0.04, 1));
                }
            } else if (selection == 5) {
                app_d->current_scene = SCENE_MAIN_MENU;
            }
        }   
    }
}