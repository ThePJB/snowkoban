#include "application.h"

void application_init(application *app, int xres, int yres) {
    app->gc = gef_init("snowkoban", xres, yres, 60);
    gef_load_atlas(&app->gc, "assets/snowkoban.png");
    app->main_menu = main_menu_init(&app->gc);
    app->game = game_init();

    app->ad = (application_data) {
        //.current_scene = &app->main_menu.s,
        .current_scene = &app->game.s,
        .keep_going = true,
    };
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