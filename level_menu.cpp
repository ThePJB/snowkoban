#include "level_menu.hpp"
#include "util.hpp"
#include "snowflakes.hpp"
#include "dankstrings.hpp"
#include "entity.hpp"
#include "coolmath.hpp"
#include "rect.hpp"

struct layout {
    rect level_rects[10];
    int num_levels = 0;

    layout(){};
};

layout get_layout(int xres, int yres, int num_levels) {
    auto l = layout();
    l.num_levels = num_levels;
    
    int level_s = 0.15 * xres;
    int level_space = 0.01 * xres;

    int n_bot = num_levels / 2;
    int n_top = num_levels - n_bot;

    int w_bot = n_bot * level_s + (n_bot - 1) * level_space;
    int w_top = n_top * level_s + (n_top - 1) * level_space;
    int top_offset = xres / 2 - w_top / 2;
    int bot_offset = xres / 2 - w_bot / 2;

    int top_y = yres / 3;
    int bot_y = 2 * yres / 3;

    for (int i = 0; i < n_top; i++) {
        l.level_rects[i] = rect(
            top_offset + i*(level_s + level_space),
            top_y - level_s/2,
            level_s,
            level_s
        );
    }

    for (int i = 0; i < n_bot; i++) {
        l.level_rects[n_top + i] = rect(
            bot_offset + i*(level_s + level_space),
            bot_y - level_s/2,
            level_s,
            level_s
        );
    }

    return l;
}

void level_menu::draw(shared_data *app_d, double dt) {
    gef_draw_rect(&app_d->gc, app_d->game_style.background, 0, 0, app_d->gc.xres, app_d->gc.yres);
    
    snowflakes_draw(&app_d->gc, app_d->time, app_d->snow_offset_base);

    gef_draw_rect(&app_d->gc, app_d->game_style.pane, rect::centered(
        app_d->gc.xres/2, 
        app_d->gc.yres/2, 
        app_d->gc.xres * 0.8, 
        app_d->gc.yres * 0.8)
    );

    gef_draw_bmp_text_centered(&app_d->gc, app_d->game_style.game_font, app_d->game_style.big, app_d->current_world()->name, app_d->gc.xres / 2, app_d->gc.yres * 0.05);

    layout l = get_layout(app_d->gc.xres, app_d->gc.yres, app_d->current_world()->lps.length);

    const auto entity_size = 64;

    for (int i = 0; i < l.num_levels; i++) {
        if (i == app_d->level_idx) {
            gef_draw_rect(&app_d->gc, app_d->game_style.highlight, l.level_rects[i].dilate(app_d->game_style.line));
        }
        SDL_Rect r = l.level_rects[i].sdl_rect();
        SDL_RenderCopy(app_d->gc.renderer, app_d->current_world()->lps.items[i].preview, NULL, &r);

        const auto vignette_colour = app_d->current_world()->lps.items[i].complete ? 
            gef_rgba(0, 255, 0, 0):
            gef_rgba(255, 50, 50, 50);

        gef_draw_rect(&app_d->gc, vignette_colour, l.level_rects[i]);
        
        if (i == app_d->level_idx) {
            auto player_clip = entity_prototype_get(ET_PLAYER).clip;
            // bop animation
            if (cm_frac(app_d->time) > 0.5) {
                player_clip.y += 16;
            }

            auto to_rect = (SDL_Rect) {
                l.level_rects[i].x - entity_size/2,
                l.level_rects[i].y + l.level_rects[i].h/2 - entity_size/2,
                entity_size,
                entity_size,
            };

            gef_draw_sprite_ex(&app_d->gc, player_clip, to_rect, 0, SDL_FLIP_HORIZONTAL);
        }
    }

    // Draw present counter


    
    char buf[64] = {0};
    sprintf(buf, "%d/%d", app_d->current_world()->num_presents_collected, app_d->current_world()->total_presents);
    const auto present_text_dimensions = gef_bmp_font_size(app_d->game_style.game_font, app_d->game_style.big, strings_length(buf));
    gef_draw_bmp_text_centered(&app_d->gc, app_d->game_style.game_font, app_d->game_style.big, buf, app_d->gc.xres * 0.5, app_d->gc.yres * 0.95);

    const auto present_clip = entity_prototype_get(ET_PRESENT).clip;
    const auto present_to_rect = (SDL_Rect) {
        .x = app_d->gc.xres * 0.5 - entity_size - present_text_dimensions.x/2, 
        .y = app_d->gc.yres * 0.95 - entity_size/2,
        .w = entity_size,
        .h = entity_size,
    };
    gef_draw_sprite(&app_d->gc, present_clip, present_to_rect);
    const auto present_to_rect2 = (SDL_Rect) {
        .x = app_d->gc.xres * 0.5 + present_text_dimensions.x/2, 
        .y = app_d->gc.yres * 0.95 - entity_size/2,
        .w = entity_size,
        .h = entity_size,
    };
    gef_draw_sprite(&app_d->gc, present_clip, present_to_rect2);
}

/*
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

        gef_draw_bmp_text(gc, app_d->game_style.game_font, app_d->game_style.big, app_d->worlds[i].name, world_x, world_y);

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
            gef_draw_bmp_text_centered(gc, app_d->game_style.game_font, app_d->game_style.small, buf, level_x + level_s/2, level_y + level_s/2);
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
        
        

        world_y += world_spacing + world_h;
    }
}
*/

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
        } else if (right && app_d->level_idx < app_d->current_world()->lps.length - 1) {
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