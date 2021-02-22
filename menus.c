#include "menus.h"
#include "util.h"
#include <stdbool.h>

void main_menu_on_focus(shared_data *shared_data, void *scene_data) {
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
                Mix_PlayChannel(-1, main_menu_data->move_noise, 0);
                main_menu_data->selection--;
            }
        } else if (down) {
            if (selection < 2) {
                Mix_PlayChannel(-1, main_menu_data->move_noise, 0);
                main_menu_data->selection++;
            }
        } else if (select) {
            Mix_PlayChannel(-1, main_menu_data->select_noise, 0);
            if (selection == 0) {
                shared_data->current_scene = SCENE_LEVEL_MENU;
            } else if (selection == 1) {
                printf("settings nyi\n");
            } else if (selection == 2) {
                shared_data->keep_going = false;
            }
        }
        
    }
}

void main_menu_draw(shared_data *shared_data, void *scene_data, gef_context *gc) {
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

    const colour highlight_colour = gef_rgb(255, 255, 0);
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
            .on_focus = main_menu_on_focus,
        },
        .move_noise = Mix_LoadWAV("assets/menu-move.wav"),
        .select_noise = Mix_LoadWAV("assets/menu-select.wav"),
        .selection = 0,
        .texts = {
                gef_make_text(gc, menu_font, "Play", 255, 255, 255),
                gef_make_text(gc, menu_font, "Settings", 255, 255, 255),
                gef_make_text(gc, menu_font, "Quit", 255, 255, 255),
            },
    };

    return mm;
}

void level_menu_on_focus(shared_data *shared_data, void *scene_data) {
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
            Mix_PlayChannel(-1, level_menu_data->move_noise, 0);
        } else if (down) {
            level_menu_data->sely = (level_menu_data->sely + 1) % h; 
            Mix_PlayChannel(-1, level_menu_data->move_noise, 0);
        } else if (right) {
            level_menu_data->selx = (level_menu_data->selx + 1) % w; 
            Mix_PlayChannel(-1, level_menu_data->move_noise, 0);
        } else if (left) {
            level_menu_data->selx = (level_menu_data->selx - 1 + w) % w; 
            Mix_PlayChannel(-1, level_menu_data->move_noise, 0);
        } else if (select) {
            Mix_PlayChannel(-1, level_menu_data->select_noise, 0);
            shared_data->selected_level = level_menu_data->selx + level_menu_data->sely * w;
            shared_data->current_scene = SCENE_GAME;
        }
    }
}

void level_menu_draw(shared_data *shared_data, void *scene_data, gef_context *gc) {
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
        },
        .move_noise = Mix_LoadWAV("assets/menu-move.wav"),
        .select_noise = Mix_LoadWAV("assets/menu-select.wav"),
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