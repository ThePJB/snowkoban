#include "settings_menu.h"
#include "snowflakes.h"

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

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool select = sym == SDLK_RETURN || sym == SDLK_SPACE;

        int selection = settings_menu_data->selection;

        if (up) {
            if (selection > 0) {
                Mix_PlayChannel(-1, settings_menu_data->move_noise, 0);
                settings_menu_data->selection--;
            }
        } else if (down) {
            if (selection < 2) {
                Mix_PlayChannel(-1, settings_menu_data->move_noise, 0);
                settings_menu_data->selection++;
            }
        } else if (select) {
            Mix_PlayChannel(-1, settings_menu_data->select_noise, 0);
            if (selection == 0) {
                shared_data->draw_snow = !shared_data->draw_snow;
            } else if (selection == 1) {
                shared_data->current_scene = SCENE_MAIN_MENU;
            }
        }
        
    }
}

void settings_menu_draw(shared_data *shared_data, void *scene_data, gef_context *gc, double dt) {
    settings_menu *settings_menu_data = (settings_menu*)scene_data;

    const int button_w = 400;
    const int button_h = 100;
    const int button_spacing = 50;
    const int highlight_size = 6;
    const int num_buttons = 2;

    // calculations to center the buttons
    const int screen_center_x = gc->xres/2;
    const int screen_center_y = gc->yres/2;
    const int menu_height = num_buttons * button_h + (num_buttons - 1) * button_spacing;

    const colour highlight_colour = gef_rgb(255, 255, 0);
    const colour button_colour = gef_rgb(100, 100, 255);
    const colour background_colour = gef_rgb(150, 150, 150);

    gef_draw_rect(gc, background_colour, 0, 0, gc->xres, gc->yres);

    if (shared_data->draw_snow) {
        snowflakes_draw(gc, gc->xres, gc->yres, shared_data->interp_time, shared_data->snow_offset_base);
    }
    

    int menu_x = screen_center_x - button_w/2;
    int menu_y = screen_center_y - menu_height/2;

    
    for (int i = 0; i < num_buttons; i++) {
        if (settings_menu_data->selection == i) {
            gef_draw_rect(gc, highlight_colour, 
                menu_x - highlight_size, menu_y - highlight_size, 
                button_w + 2*highlight_size, button_h + 2*highlight_size
            );
        }
        gef_draw_rect(gc, button_colour, menu_x, menu_y, button_w, button_h);
        gef_draw_text(gc, settings_menu_data->texts[i], 
            menu_x + button_w/2 - settings_menu_data->texts[i].w/2,
            menu_y + button_h/2 - settings_menu_data->texts[i].h/2);


        menu_y += button_h + button_spacing;
    }
}

settings_menu settings_menu_init(gef_context *gc) {
    font_handle menu_font = gef_load_font("assets/Hack-Regular.ttf", 48);

    settings_menu menu = (settings_menu) {
        .s = (scene_interface) {
            .draw = settings_menu_draw,
            .handle_input = settings_menu_handle_input,
            .on_focus = settings_menu_on_focus,
            .update = settings_menu_update,
        },
        .move_noise = Mix_LoadWAV("assets/menu-move.wav"),
        .select_noise = Mix_LoadWAV("assets/menu-select.wav"),
        .selection = 0,
        .texts = {
                gef_make_text(gc, menu_font, "Snow", 255, 255, 255),
                gef_make_text(gc, menu_font, "Back", 255, 255, 255),
            },
    };

    return menu;
}