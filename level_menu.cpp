#include "level_menu.hpp"
#include "util.h"
#include "snowflakes.h"
#include "dankstrings.h"
#include "entity.h"
#include "coolmath.h"


void level_menu::draw(shared_data *app_d, double dt) {
    gef_context *gc = &app_d->gc;

    const int world_w = 0.8 * gc->xres;
    const int world_h = 0.2 * gc->yres;
    const int world_spacing = 0.03 * gc->yres;

    const colour background_colour = gef_rgb(160, 160, 160);
    gef_draw_rect(gc, background_colour, 0, 0, gc->xres, gc->yres);
    snowflakes_draw(gc, gc->xres, gc->yres, app_d->time, app_d->snow_offset_base);

    const colour select_colour = gef_rgb(255, 255, 0);
    const colour level_colour = gef_rgb(128, 128, 128);
    const colour level_done_colour = gef_rgb(128, 255, 128);
    const int highlight_px = 4;
    const int inset_px = 4;
    const int title_h = 48;

    const int level_s = world_h * 0.5;
    const int level_spacing = 0.1 * world_h;

    int world_x = gc->xres / 2 - world_w / 2;
    int world_y = world_spacing;

    for (int i = 0; i < app_d->num_worlds; i++) {
        if (app_d->world_idx == i) {
            gef_draw_rect(gc, select_colour, 
                world_x - highlight_px, 
                world_y - highlight_px, 
                world_w + 2 * highlight_px, 
                world_h + 2 * highlight_px
            );

            // must draw player character as well
            int player_frame = cm_frac(app_d->time) > 0.5;
            SDL_Rect from_rect = entity_prototype_get(ET_PLAYER).clip;
            const int spritesheet_size = 16;
            from_rect.y += player_frame * spritesheet_size;
            int player_x = 0.05 * gc->xres;
            int player_y = world_y + world_h/2 - 32;
            SDL_Rect to_rect = {player_x, player_y, 64, 64};
            gef_draw_sprite_ex(gc, from_rect, to_rect, 0, SDL_FLIP_HORIZONTAL);
        }
        
        gef_draw_rect(gc, app_d->worlds[i].highlight, 
            world_x, 
            world_y, 
            world_w, 
            world_h
        );

        text_handle t = gef_make_text(gc, app_d->title_font, app_d->worlds[i].name, 255, 255, 255);
        gef_draw_text(gc, t, world_x, world_y);
        gef_destroy_text(t);

        int inset_x = world_x + inset_px;
        int inset_y = world_y + inset_px + title_h;

        gef_draw_rect(gc, app_d->worlds[i].background, 
            inset_x, 
            inset_y, 
            world_w - 2*inset_px, 
            world_h - 2*inset_px - title_h
        );

        for (int j = 0; j < app_d->worlds[i].num_levels; j++) {
            int level_x = inset_x + level_spacing + j * (level_spacing + level_s);
            int level_y = inset_y + level_spacing;
            
            if (app_d->world_idx == i && app_d->level_idx == j) {
                gef_draw_rect(gc, select_colour, 
                    level_x - highlight_px, 
                    level_y - highlight_px, 
                    level_s + 2 * highlight_px, 
                    level_s + 2 * highlight_px
                );
            }

            if (app_d->worlds[i].completed[j]) {
                gef_draw_rect(gc, level_done_colour, level_x, level_y, level_s, level_s);
            } else {
                gef_draw_rect(gc, level_colour, level_x, level_y, level_s, level_s);
            }
            

            char buf[64] = {0};
            strings_itoa(buf, j);
            text_handle t = gef_make_text(gc, app_d->title_font, buf, 255, 255, 255);
            gef_draw_text(gc, t, level_x + level_s/2 - t.w/2, level_y + level_s/2 - t.h/2);
            gef_destroy_text(t);
        }


        int present_counter_x = world_x + world_w - 250;
        int present_counter_y = world_y + world_h - 74;
        SDL_Rect present_to_rect = (SDL_Rect) {
            present_counter_x,
            present_counter_y,
            64,
            64,
        };
        present_counter_x += 74;

        gef_draw_sprite(gc, entity_prototype_get(ET_PRESENT).clip, present_to_rect);
        
        char buf[64] = {0};
        sprintf(buf, "%d/%d", app_d->worlds[i].collected_presents, app_d->worlds[i].total_presents);
        t = gef_make_text(gc, app_d->title_font, buf, 255, 255, 255);
        gef_draw_text(gc, t, present_counter_x, present_counter_y);
        gef_destroy_text(t);

        world_y += world_spacing + world_h;
    }
}

void level_menu::handle_input(shared_data *app_d, SDL_Event e) {
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

        if (left && app_d->level_idx > 0) {
            app_d->level_idx--;
            audio_play(&app_d->a, CS_MENU_MOVE);
        } else if (right && app_d->level_idx < app_d->worlds[app_d->world_idx].num_levels - 1) {
            app_d->level_idx++;
            audio_play(&app_d->a, CS_MENU_MOVE);
        } else if (up && app_d->world_idx > 0) {
            app_d->world_idx--;
            app_d->level_idx = 0;
            audio_play(&app_d->a, CS_MENU_MOVE);
        } else if (down && app_d->world_idx < app_d->num_worlds - 1) {
            app_d->world_idx++;
            app_d->level_idx = 0;
            audio_play(&app_d->a, CS_MENU_MOVE);   
        }
        if (select) {
            app_d->current_scene = SCENE_GAME;
            audio_play(&app_d->a, CS_MENU_SELECT);
        }
    }
}