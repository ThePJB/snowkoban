#include "application.h"

void application_init(application *app, int xres, int yres, bool do_start_level, int start_level) {
    app->gc = gef_init("snowkoban", xres, yres, 60);
    gef_load_atlas(&app->gc, "assets/snowkoban.png");

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("failed to open audio\n");
    }

    app->audio = audio_load_sounds();


    app->main_menu = main_menu_init(&app->gc);
    app->game = game_init(start_level, &app->audio);

    app->ad = (application_data) {
        .current_scene = &app->main_menu.s,
        .keep_going = true,
    };

    if (do_start_level) {
        app->ad.current_scene = &app->game.s;
    }
}

void application_update(application *app) {
    return; // probably dont do much here since its mostly input driven
}

void application_draw(application *app) {
    gef_clear(&app->gc);

    app->ad.current_scene->draw(&app->ad, app->ad.current_scene, &app->gc);

    gef_present(&app->gc);
}

void application_handle_input(application *app) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            app->ad.keep_going = false;
            return;
        } else {
            app->ad.current_scene->handle_input(&app->ad, app->ad.current_scene, e);
        }
    }
}