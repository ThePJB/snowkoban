#include "application.h"
#include "levels.h"
#include "util.h"

void application_init(application *app, int xres, int yres, bool do_start_level, int start_level) {
    app->gc = gef_init("snowkoban", xres, yres);
    gef_load_atlas(&app->gc, "assets/snowkoban.png");

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("failed to open audio\n");
    }

    app->audio = audio_load_sounds();
    app->previous_scene = -1;
    app->shared_data = (shared_data) {
        .current_scene = SCENE_MAIN_MENU,
        .keep_going = true,
        .levels = levels,
        .num_levels = len(levels),
        .selected_level = 0,
        .completed = {false},
        .time = 0,
    };

    app->scenes[SCENE_MAIN_MENU] = malloc(sizeof(main_menu));
    *(main_menu*)app->scenes[SCENE_MAIN_MENU] = main_menu_init(&app->gc);
    app->scenes[SCENE_LEVEL_MENU] = malloc(sizeof(level_menu));
    *(level_menu*)app->scenes[SCENE_LEVEL_MENU] = level_menu_init(&app->gc, &app->shared_data);
    app->scenes[SCENE_GAME] = malloc(sizeof(game));
    *(game*)app->scenes[SCENE_GAME] = game_init(&app->audio, &app->shared_data);

    if (do_start_level) {
        app->shared_data.current_scene = SCENE_GAME;
        app->shared_data.selected_level = start_level;
    }
}

void application_update(application *app, double dt) {
    if (app->previous_scene != app->shared_data.current_scene) {
        app->previous_scene = app->shared_data.current_scene;
        scene_interface *cs = app->scenes[app->shared_data.current_scene];
        cs->on_focus(&app->shared_data, cs);
    }

    app->shared_data.time += dt;

    return; // probably dont do much here since its mostly input driven
}

void application_draw(application *app, double dt) {
    gef_clear(&app->gc);

    app->scenes[app->shared_data.current_scene]->draw(
        &app->shared_data, 
        app->scenes[app->shared_data.current_scene], 
        &app->gc, dt);

    gef_present(&app->gc);
}

void application_handle_input(application *app) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            app->shared_data.keep_going = false;
            return;
        } else {
            app->scenes[app->shared_data.current_scene]->handle_input(
                &app->shared_data, app->scenes[app->shared_data.current_scene], e);
        }
    }
}