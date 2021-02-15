#include "menus.h"
#include "util.h"
#include <stdbool.h>

void main_menu_handle_input(application_data *ad, void *scene_data, SDL_Event e) {
    main_menu *main_menu_data = (main_menu*)scene_data;

    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool select = sym == SDLK_RETURN || sym == SDLK_SPACE;

        int selection = main_menu_data->selection;

        if (up) {
            main_menu_data->selection = max(selection - 1, 0);
        } else if (down) {
            main_menu_data->selection = min(selection + 1, 2);
        } else if (select) {
            if (selection == 0) {
                printf("play nyi\n");
            } else if (selection == 1) {
                printf("settings nyi\n");
            } else if (selection == 2) {
                ad->keep_going = false;
            }
        }
        
    }
}

void main_menu_draw(application_data *ad, void *scene_data, gef_context *gc) {
    main_menu *main_menu_data = (main_menu*)scene_data;

    const int button_w = 400;
    const int button_h = 100;
    const int button_spacing = 50;
    const int highlight_size = 6;
    const int num_buttons = 3;

    // calculations to center the buttons
    const int screen_center_x = gc->xres/2;
    const int screen_center_y = gc->yres/2;
    const int menu_height = num_buttons * button_h + (num_buttons - 1) * button_spacing;

    const colour highlight_colour = gef_rgb(0, 255, 255);
    const colour button_colour = gef_rgb(100, 100, 255);
    const colour background_colour = gef_rgb(150, 150, 150);

    int menu_x = screen_center_x - button_w/2;
    int menu_y = screen_center_y - menu_height/2;

    gef_draw_rect(gc, background_colour, 0, 0, gc->xres, gc->yres);
    
    for (int i = 0; i < num_buttons; i++) {
        if (main_menu_data->selection == i) {
            gef_draw_rect(gc, highlight_colour, 
                menu_x - highlight_size, menu_y - highlight_size, 
                button_w + 2*highlight_size, button_h + 2*highlight_size
            );
        }
        gef_draw_rect(gc, button_colour, menu_x, menu_y, button_w, button_h);
        //gef_draw_text(gc, main_menu_data->texts[i], menu_x, menu_y);
        gef_draw_text(gc, main_menu_data->texts[i], 
            menu_x + button_w/2 - main_menu_data->texts[i].w/2,
            menu_y + button_h/2 - main_menu_data->texts[i].h/2);


        menu_y += button_h + button_spacing;
    }
}

main_menu main_menu_init(gef_context *gc) {
    font_handle menu_font = gef_load_font("assets/Hack-Regular.ttf", 48);

    main_menu mm = (main_menu) {
        .s = (scene_interface) {
            .draw = main_menu_draw,
            .handle_input = main_menu_handle_input,
        },
        .selection = 0,
        .texts = {
                gef_make_text(gc, menu_font, "Play", 255, 255, 255),
                gef_make_text(gc, menu_font, "Settings", 255, 255, 255),
                gef_make_text(gc, menu_font, "Quit", 255, 255, 255),
            },
    };

    return mm;
}