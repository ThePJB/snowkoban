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

layout get_layout(rect container, int num_levels) {
    auto l = layout();
    l.num_levels = num_levels;
    
    const auto n_bot = num_levels / 2;
    const auto n_top = num_levels - n_bot;

    const auto level_s = min(
        0.8 * container.w / n_top,
        0.8 * container.h / 2
    );

    for (int i = 0; i < n_top; i++) {
        l.level_rects[i] = rect::centered_layout(container, level_s, level_s, n_top, 2, i, 0);
    }

    for (int i = 0; i < n_bot; i++) {
        l.level_rects[n_top + i] = rect::centered_layout(container, level_s, level_s, n_bot, 2, i, 1);
    }

    return l;
}

void level_menu::draw(shared_data *app_d, double dt) {
    gef_draw_rect(&app_d->gc, app_d->game_style.background, 0, 0, app_d->gc.xres, app_d->gc.yres);
    
    if (app_d->draw_snow) {
        snowflakes_draw(&app_d->gc, app_d->time, app_d->snow_xo);
    }

    const auto pane_rect = rect::centered(
        app_d->gc.xres/2, 
        app_d->gc.yres/2, 
        app_d->gc.xres * 0.8, 
        app_d->gc.yres * 0.8
    );

    gef_draw_rect(&app_d->gc, app_d->game_style.pane, pane_rect);

    gef_draw_bmp_text_centered(&app_d->gc, app_d->game_style.game_font, app_d->game_style.big, app_d->current_world()->name, app_d->gc.xres / 2, app_d->gc.yres * 0.05);

    layout l = get_layout(pane_rect, app_d->current_world()->lps.length);

    const auto entity_size = 64;

    for (int i = 0; i < l.num_levels; i++) {
        const auto line_colour = app_d->level_idx == i ?
            app_d->game_style.highlight:
            app_d->game_style.btn_line_colour;
        gef_draw_rect(&app_d->gc, line_colour, l.level_rects[i].dilate(app_d->game_style.line));
        SDL_Rect r = l.level_rects[i].sdl_rect();
        SDL_RenderCopy(app_d->gc.renderer, app_d->current_world()->lps.items[i].preview, NULL, &r);

        const auto vignette_colour = app_d->current_world()->lps.items[i].complete ? 
            gef_rgba(0, 0, 0, 0):
            gef_rgba(0, 0, 0, 128);

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
        } else if (down && app_d->world_idx < app_d->worlds.length - 1) {
            app_d->world_idx++;
            app_d->level_idx = 0;
            audio_play(&app_d->a, CS_MENU_MOVE);   
        }
        if (select) {
            app_d->current_scene = SCENE_GAME;
            audio_play(&app_d->a, CS_MENU_SELECT);
        }
    } else if (e.type == SDL_MOUSEMOTION) {
        const auto pane_rect = rect::centered(app_d->gc.xres/2, app_d->gc.yres/2, 0.8 * app_d->gc.xres, 0.8 * app_d->gc.yres);
        layout l = get_layout(pane_rect, app_d->current_world()->lps.length);

        for (int i = 0; i < l.num_levels; i++) {
            if (l.level_rects[i].contains(e.motion.x, e.motion.y)) {
                if (app_d->level_idx != i) {
                    audio_play(&app_d->a, CS_MENU_MOVE);
                    app_d->level_idx = i;
                }
            }
        }
    } else if (e.type == SDL_MOUSEBUTTONUP) {
        const auto pane_rect = rect::centered(app_d->gc.xres/2, app_d->gc.yres/2, 0.8 * app_d->gc.xres, 0.8 * app_d->gc.yres);
        layout l = get_layout(pane_rect, app_d->current_world()->lps.length);

        for (int i = 0; i < l.num_levels; i++) {
            if (l.level_rects[i].contains(e.motion.x, e.motion.y)) {
                audio_play(&app_d->a, CS_MENU_SELECT);
                app_d->level_idx = i;
                app_d->current_scene = SCENE_GAME;
            }
        }
    }
}