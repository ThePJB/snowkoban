#include "main_menu.h"
#include "util.h"
#include "snowflakes.h"
#include "audio.h"
#include "button.h"
#include <stdbool.h>

void main_menu_on_focus(shared_data *shared_data, void *scene_data) {
    return;
}

void main_menu_update(shared_data *shared_data, void *scene_data, double dt) {
    return;
}

void main_menu_handle_input(shared_data *shared_data, void *scene_data, SDL_Event e) {
    main_menu *main_menu_data = (main_menu*)scene_data;

    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool select = sym == SDLK_RETURN || sym == SDLK_SPACE;

        int selection = main_menu_data->selection;

        if (up) {
            if (selection > 0) {
                audio_play(&shared_data->a, CS_MENU_MOVE);
                main_menu_data->selection--;
            }
        } else if (down) {
            if (selection < 2) {
                audio_play(&shared_data->a, CS_MENU_MOVE);
                main_menu_data->selection++;
            }
        } else if (select) {
            audio_play(&shared_data->a, CS_MENU_SELECT);
            if (selection == 0) {
                shared_data->current_scene = SCENE_LEVEL_MENU;
            } else if (selection == 1) {
                shared_data->current_scene = SCENE_SETTINGS_MENU;
            } else if (selection == 2) {
                shared_data->keep_going = false;
            }
        }
        
    }
}

void main_menu_draw(shared_data *shared_data, void *scene_data, gef_context *gc, double dt) {
    main_menu *main_menu_data = (main_menu*)scene_data;

    const colour bg_colour = gef_rgb(150, 150, 200);

    const int num_buttons = 3;
    const int button_w = 400;
    const int button_h = 100;
    const int button_spacing = 50;

    // calculations to center the buttons
    const int screen_center_x = gc->xres/2;
    const int screen_center_y = gc->yres/2;
    const int menu_height = num_buttons * button_h + (num_buttons - 1) * button_spacing;

    gef_draw_rect(gc, bg_colour, 0, 0, gc->xres, gc->yres);

    if (shared_data->draw_snow) {
        snowflakes_draw(gc, gc->xres, gc->yres, shared_data->interp_time, shared_data->snow_offset_base);
    }

    int menu_x = screen_center_x - button_w/2;
    int menu_y = screen_center_y - menu_height/2;
    
    SDL_Rect btn_rect = {menu_x, menu_y, button_w, button_h};

    button_generic_draw(gc, shared_data->menu_button_style, btn_rect, "Play", main_menu_data->selection == 0);
    btn_rect.y += button_h + button_spacing;
    button_generic_draw(gc, shared_data->menu_button_style, btn_rect, "Settings", main_menu_data->selection == 1);
    btn_rect.y += button_h + button_spacing;
    button_generic_draw(gc, shared_data->menu_button_style, btn_rect, "Quit", main_menu_data->selection == 2);

}

main_menu main_menu_init(gef_context *gc) {
    font_handle menu_font = gef_load_font("assets/Hack-Regular.ttf", 48);

    main_menu mm = (main_menu) {
        .s = (scene_interface) {
            .draw = main_menu_draw,
            .handle_input = main_menu_handle_input,
            .on_focus = main_menu_on_focus,
            .update = main_menu_update,
        },
        .selection = 0,
    };

    return mm;
}
