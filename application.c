#include "application.h"
#include "levels.h"
#include "util.h"
#include "level_menu_new.h"


void application_init(application *app, int xres, int yres, bool do_start_level, int start_level) {
    
    app->shared_data = (shared_data) {
        .current_scene = SCENE_MAIN_MENU,
        .keep_going = true,
        .levels = levels,
        .num_levels = len(levels),
        .selected_level = 0,
        .completed = {false},
        .time = 0,
        .draw_snow = true,
        .snow_offset_base = 0,
        .snow_offset_current = 0,

        .max_scale = 8,


        // new level stuff
        .worlds = {
            #include "world_1.h"
            ,
            #include "world_2.h"
            ,
            #include "world_3.h"
            ,
            #include "world_4.h"
            ,
            #include "world_5.h"
            ,
        },
        .world_idx = 0,
        .level_idx = 0,
        .num_worlds = 5,
    };

    app->shared_data.gc = gef_init("snowkoban", xres, yres);
    gef_load_atlas(&app->shared_data.gc, "assets/snowkoban.png");

    app->shared_data.a = audio_init();

    app->shared_data.menu_button_style = (button_style) {
        .bg = gef_rgb(80, 150, 220),
        .line = gef_rgb(100, 100, 100),
        .highlight = gef_rgb(255, 255, 0),
        .text = gef_rgb(255, 255, 255),
        .f = gef_load_font("assets/Hack-Regular.ttf", 48),

        .line_thickness = 6,
    };

    app->shared_data.title_font = gef_load_font("assets/Hack-Regular.ttf", 48);
    
    app->previous_scene = -1;

    app->scenes[SCENE_MAIN_MENU] = malloc(sizeof(main_menu));
    *(main_menu*)app->scenes[SCENE_MAIN_MENU] = main_menu_init(&app->shared_data.gc);
    //app->scenes[SCENE_LEVEL_MENU] = malloc(sizeof(level_menu));
    //*(level_menu*)app->scenes[SCENE_LEVEL_MENU] = level_menu_init(&app->shared_data.gc, &app->shared_data);
    app->scenes[SCENE_LEVEL_MENU] = malloc(sizeof(level_menu_new));
    *(level_menu_new*)app->scenes[SCENE_LEVEL_MENU] = level_menu_new_init(&app->shared_data.gc, &app->shared_data);
    app->scenes[SCENE_SETTINGS_MENU] = malloc(sizeof(settings_menu));
    *(settings_menu*)app->scenes[SCENE_SETTINGS_MENU] = settings_menu_init(&app->shared_data.gc);
    app->scenes[SCENE_GAME] = malloc(sizeof(game));
    *(game*)app->scenes[SCENE_GAME] = game_init(&app->shared_data);

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
    app->shared_data.interp_time += dt;
    app->shared_data.interp_time += min(2*dt, 0.1 * (app->shared_data.time - app->shared_data.interp_time));

    app->scenes[app->shared_data.current_scene]->update(
        &app->shared_data, 
        app->scenes[app->shared_data.current_scene],
        dt
    );

    return;
}

void application_draw(application *app, double dt) {
    gef_clear(&app->shared_data.gc);

    app->scenes[app->shared_data.current_scene]->draw(
        &app->shared_data, 
        app->scenes[app->shared_data.current_scene], 
        &app->shared_data.gc, dt);

    gef_present(&app->shared_data.gc);
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