#include "settings_menu.h"
#include "snowflakes.h"
#include "util.h"

void settings_menu_on_focus(shared_data *shared_data, void *scene_data) {
    return;
}

void settings_menu_update(shared_data *shared_data, void *scene_data, double dt) {
    return;
}

void settings_menu_handle_input(shared_data *shared_data, void *scene_data, SDL_Event e) {
    settings_menu *settings_menu_data = (settings_menu*)scene_data;

    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        if (sym == SDLK_ESCAPE) {
            shared_data->current_scene = SCENE_MAIN_MENU;
        }

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool left = sym == SDLK_LEFT || sym == SDLK_a || sym == SDLK_h;
        bool right = sym == SDLK_RIGHT || sym == SDLK_d || sym == SDLK_l;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool select = sym == SDLK_RETURN || sym == SDLK_SPACE;

        int selection = settings_menu_data->selection;
        const int num_buttons = 6;

        if (up) {
            if (selection > 0) {
                audio_play(&shared_data->a, CS_MENU_MOVE);
                settings_menu_data->selection--;
            }
        } else if (down) {
            if (selection < num_buttons - 1) {
                audio_play(&shared_data->a, CS_MENU_MOVE);
                settings_menu_data->selection++;
            }
        } else if (select || left || right) {
            audio_play(&shared_data->a, CS_MENU_SELECT);
            if (selection == 0) {
                shared_data->draw_snow = !shared_data->draw_snow;
            } else if (selection == 1) {
                //max scale
                if (left) {
                    shared_data->max_scale = max(shared_data->max_scale - 1, 1);
                } else if (right) {
                    shared_data->max_scale = min(shared_data->max_scale + 1, 20);
                }
            } else if (selection == 2) {
                // res idk yet
            } else if (selection == 3) {
                // music volume
                if (left) {
                    audio_set_bgm_volume(&shared_data->a, max(shared_data->a.bgm_volume - 0.04, 0));
                } else if (right) {
                    audio_set_bgm_volume(&shared_data->a, min(shared_data->a.bgm_volume + 0.04, 1));
                }
            } else if (selection == 4) {
                //sfx volume
                if (left) {
                    audio_set_sfx_volume(&shared_data->a, max(shared_data->a.sfx_volume - 0.04, 0));
                } else if (right) {
                    audio_set_sfx_volume(&shared_data->a, min(shared_data->a.sfx_volume + 0.04, 1));
                }
            } else if (selection == 5) {
                shared_data->current_scene = SCENE_MAIN_MENU;
            }
        }   
    }
}

void settings_menu_draw(shared_data *shared_data, void *scene_data, gef_context *gc, double dt) {
    settings_menu *settings_menu_data = (settings_menu*)scene_data;

    const int button_w = 600;
    const int button_h = 100;
    const int button_spacing = 50;
    const int highlight_size = 6;
    const int num_buttons = 6;

    // calculations to center the buttons
    const int screen_center_x = gc->xres/2;
    const int screen_center_y = gc->yres/2;
    const int menu_height = num_buttons * button_h + (num_buttons - 1) * button_spacing;

    const colour background_colour = gef_rgb(150, 150, 150);

    gef_draw_rect(gc, background_colour, 0, 0, gc->xres, gc->yres);

    if (shared_data->draw_snow) {
        snowflakes_draw(gc, gc->xres, gc->yres, shared_data->interp_time, shared_data->snow_offset_base);
    }
    
    int menu_x = screen_center_x - button_w/2;
    int menu_y = screen_center_y - menu_height/2;

    SDL_Rect btn_rect = {menu_x, menu_y, button_w, button_h};

    int i = 0;
    button_bool_draw(gc, shared_data->menu_button_style, btn_rect, "Snow", shared_data->draw_snow, settings_menu_data->selection == i++);
    btn_rect.y += button_h + button_spacing;
    button_int_draw(gc, shared_data->menu_button_style, btn_rect, "Max Scale", shared_data->max_scale, settings_menu_data->selection == i++);
    btn_rect.y += button_h + button_spacing;
    button_generic_draw(gc, shared_data->menu_button_style, btn_rect, "Resolution", settings_menu_data->selection == i++);
    btn_rect.y += button_h + button_spacing;
    button_percent_draw(gc, shared_data->menu_button_style, btn_rect, "Music Volume", shared_data->a.bgm_volume, settings_menu_data->selection == i++);
    btn_rect.y += button_h + button_spacing;
    button_percent_draw(gc, shared_data->menu_button_style, btn_rect, "SFX Volume", shared_data->a.sfx_volume, settings_menu_data->selection == i++);
    btn_rect.y += button_h + button_spacing;
    button_generic_draw(gc, shared_data->menu_button_style, btn_rect, "Back", settings_menu_data->selection == i++);
}

settings_menu settings_menu_init(gef_context *gc) {
    settings_menu menu = (settings_menu) {
        .s = (scene_interface) {
            .draw = settings_menu_draw,
            .handle_input = settings_menu_handle_input,
            .on_focus = settings_menu_on_focus,
            .update = settings_menu_update,
        },
        .selection = 0,
    };

    return menu;
}