#include "level_menu2.hpp"
#include "coolmath.hpp"
#include "snowflakes.hpp"

#define util_min(A,B) (A < B ? A : B)
#define util_max(A,B) (A > B ? A : B)


void level_menu2::update(shared_data *app_d, double dt) {
}

void level_menu2::on_focus(shared_data *app_d) {
}

void level_menu2::draw(shared_data *app_d, double dt) {
    // ok must still be getting x dimension wrong somehow
    // also y seems to sort of work but its not shooting for the middle but for the bottom
    // angry coding lol
    // matrix menu a bit overwhelming but maybe if its more spaced

    // could just put always in the middle

    const auto btn_space_x = 20;
    const auto btn_space_y = 50;
    const auto side = 200;
    
    const auto bg_colour = gef_rgb(64, 64, 64);
    gef_draw_rect(&app_d->gc, bg_colour, gef_screen_rect(&app_d->gc));
    const auto title_pane_colour = app_d->game_style.background;
    
    const auto num_levels_x = app_d->worlds.max([](world w){return w.lps.length;});
    const auto num_levels_y = app_d->worlds.length;
    const auto x_offset = app_d->gc.xres * 0.2 + btn_space_x - cm_clamp(0, app_d->level_idx - 3, num_levels_x - 5)*(btn_space_x + side);
    const auto y_offset = app_d->gc.yres * 0.2 - side/2 - cm_clamp(0, app_d->world_idx - 1, num_levels_y - 2) *(btn_space_y + side);

    if (app_d->draw_snow) {
        snowflakes_draw(&app_d->gc, app_d->time, app_d->snow_xo);
    }

    for (int i = 0; i < app_d->worlds.length; i++) {
        for (int j = 0; j < app_d->worlds.items[i].lps.length; j++) {
            const auto btn_rect = rect(x_offset + (btn_space_x+side) * j, y_offset + (btn_space_y+side) * i, side, side);
            const auto border_rect = btn_rect.dilate(app_d->game_style.line);
            const auto border_colour = app_d->world_idx == i && app_d->level_idx == j ? 
                app_d->game_style.highlight :
                app_d->worlds.items[i].lps.items[j].complete ?
                    gef_rgb(0, 255, 0) :
                    app_d->game_style.btn_line_colour;

            gef_draw_rect(&app_d->gc, border_colour, border_rect);
            const auto r = btn_rect.sdl_rect();
            SDL_RenderCopy(app_d->gc.renderer, app_d->worlds.items[i].lps.items[j].preview, NULL, &r);

            const auto vignette_colour = app_d->worlds.items[i].lps.items[j].complete ?
                gef_rgba(0, 0, 0, 0) :
                gef_rgba(0, 0, 0, 80);

            gef_draw_rect(&app_d->gc, vignette_colour, btn_rect);

            //char buf[64];
            //sprintf(buf, "%02d", j+1);
            //gef_draw_bmp_text_centered(&app_d->gc, app_d->game_style.game_font, app_d->game_style.small, buf, btn_rect.center().x, btn_rect.center().y);
        }
    }


    gef_draw_rect(&app_d->gc, title_pane_colour, 0, 0, app_d->gc.xres * 0.2, app_d->gc.yres);
    
    for (int i = 0; i < app_d->worlds.length; i++) {
        gef_draw_bmp_text_centered(&app_d->gc, app_d->game_style.game_font, app_d->game_style.small, app_d->worlds.items[i].name, app_d->gc.xres * 0.1, side/2 + y_offset + (btn_space_y+side) * i);
    }


}

bool previous_world(shared_data *app_d) {
    if (app_d->world_idx > 0) {
        app_d->world_idx--;
        app_d->level_idx = util_min(app_d->level_idx, app_d->current_world()->lps.length - 1);
        return true;
    }
    return false;
}

bool next_world(shared_data *app_d) {
    if (app_d->world_idx < app_d->worlds.length - 1) {
        app_d->world_idx++;
        app_d->level_idx = util_min(app_d->level_idx, app_d->current_world()->lps.length - 1);
        return true;
    }
    return false;
}

bool previous_level(shared_data *app_d) {
    if (app_d->level_idx > 0) {
        app_d->level_idx--;
        return true;
    }
    return false;
}

bool next_level(shared_data *app_d) {
    if (app_d->level_idx < app_d->current_world()->lps.length - 1) {
        app_d->level_idx++;
        return true;
    }
    return false;
}

void level_menu2::handle_input(shared_data *app_d, SDL_Event e) {
    // move selected world and level and maybe scroll
    if (e.type == SDL_KEYDOWN) {
        SDL_Keycode sym = e.key.keysym.sym;

        if (sym == SDLK_ESCAPE) {
            app_d->set_scene(SCENE_MAIN_MENU);
        }

        bool up = sym == SDLK_UP || sym == SDLK_w || sym == SDLK_k;
        bool left = sym == SDLK_LEFT || sym == SDLK_a || sym == SDLK_h;
        bool right = sym == SDLK_RIGHT || sym == SDLK_d || sym == SDLK_l;
        bool down = sym == SDLK_DOWN || sym == SDLK_s || sym == SDLK_j;
        bool select = sym == SDLK_RETURN || sym == SDLK_SPACE;

        if (left) {
            if (previous_level(app_d)) {
                audio_play(&app_d->a, CS_MENU_MOVE);
            }
        } else if (right) {
            if (next_level(app_d)) {
                audio_play(&app_d->a, CS_MENU_MOVE);
            }
        } else if (up) {
            if (previous_world(app_d)) {
                audio_play(&app_d->a, CS_MENU_MOVE);
            }
        } else if (down) {
            if (next_world(app_d)) {
                audio_play(&app_d->a, CS_MENU_MOVE);
            }
        }
        if (select) {
            app_d->wd = WD_RIGHT;
            audio_play(&app_d->a, CS_MENU_SELECT);
            app_d->set_scene(SCENE_GAME);
        }
    }
}
