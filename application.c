#include "application.h"
#include "levels.h"
#include "util.h"
#include "level_menu_new.h"


void application_init(application *app, int xres, int yres, bool do_start_level, int start_level) {
    
    app->m_shared_data.gc = gef_init("snowkoban", xres, yres);

    app->m_shared_data.draw_snow = true;
    app->m_shared_data.max_scale = 8;

    app->m_shared_data.keep_going = true;
    app->m_shared_data.current_scene = SCENE_MAIN_MENU;

    app->m_shared_data.time = 0;

    app->m_shared_data.snow_offset_base = 0;
    app->m_shared_data.snow_offset_current = 0;
    
    app->m_shared_data.levels = levels;
    app->m_shared_data.num_levels = len(levels);
    app->m_shared_data.selected_level = 0;
    
    //app->m_shared_data.completed = {false};
    

    // new level stuff
    app->m_shared_data.worlds[0] = 
    #include "world_1.h"
    ;
    app->m_shared_data.worlds[1] = 
    #include "world_2.h"
    ;
    app->m_shared_data.worlds[2] = 
    #include "world_3.h"
    ;
    app->m_shared_data.worlds[3] = 
    #include "world_4.h"
    ;
    app->m_shared_data.worlds[4] = 
    #include "world_5.h"
    ;
    app->m_shared_data.world_idx = 0;
    app->m_shared_data.num_worlds = 5;
    app->m_shared_data.level_idx = 0;

    gef_load_atlas(&app->m_shared_data.gc, "assets/snowkoban.png");

    app->m_shared_data.a = audio_init();

    app->m_shared_data.menu_button_style = (button_style) {
        .bg = gef_rgb(80, 150, 220),
        .line = gef_rgb(100, 100, 100),
        .text = gef_rgb(255, 255, 255),
        .highlight = gef_rgb(255, 255, 0),
        .f = gef_load_font("assets/Hack-Regular.ttf", 48),

        .line_thickness = 6,
    };

    app->m_shared_data.title_font = gef_load_font("assets/Hack-Regular.ttf", 48);
    
    app->previous_scene = (scene_index)-1;

    app->scenes[SCENE_MAIN_MENU] = (scene_interface*)malloc(sizeof(main_menu));
    *(main_menu*)app->scenes[SCENE_MAIN_MENU] = main_menu_init(&app->m_shared_data.gc);
    //app->scenes[SCENE_LEVEL_MENU] = malloc(sizeof(level_menu));
    //*(level_menu*)app->scenes[SCENE_LEVEL_MENU] = level_menu_init(&app->m_shared_data.gc, &app->m_shared_data);
    app->scenes[SCENE_LEVEL_MENU] = (scene_interface*)malloc(sizeof(level_menu_new));
    *(level_menu_new*)app->scenes[SCENE_LEVEL_MENU] = level_menu_new_init(&app->m_shared_data.gc, &app->m_shared_data);
    app->scenes[SCENE_SETTINGS_MENU] = (scene_interface*)malloc(sizeof(settings_menu));
    *(settings_menu*)app->scenes[SCENE_SETTINGS_MENU] = settings_menu_init(&app->m_shared_data.gc);
    app->scenes[SCENE_GAME] = (scene_interface*)malloc(sizeof(game));
    *(game*)app->scenes[SCENE_GAME] = game_init(&app->m_shared_data);

    if (do_start_level) {
        app->m_shared_data.current_scene = SCENE_GAME;
        app->m_shared_data.selected_level = start_level;
    }
}

void application_update(application *app, double dt) {
    if (app->previous_scene != app->m_shared_data.current_scene) {
        app->previous_scene = app->m_shared_data.current_scene;
        scene_interface *cs = app->scenes[app->m_shared_data.current_scene];
        cs->on_focus(&app->m_shared_data, cs);
    }

    app->m_shared_data.time += dt;
    app->m_shared_data.interp_time += dt;
    app->m_shared_data.interp_time += min(2*dt, 0.1 * (app->m_shared_data.time - app->m_shared_data.interp_time));

    app->scenes[app->m_shared_data.current_scene]->update(
        &app->m_shared_data, 
        app->scenes[app->m_shared_data.current_scene],
        dt
    );

    return;
}

void application_draw(application *app, double dt) {
    gef_clear(&app->m_shared_data.gc);

    app->scenes[app->m_shared_data.current_scene]->draw(
        &app->m_shared_data, 
        app->scenes[app->m_shared_data.current_scene], 
        &app->m_shared_data.gc, dt);

    gef_present(&app->m_shared_data.gc);
}

void application_handle_input(application *app) {

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            app->m_shared_data.keep_going = false;
            return;
        } else {
            app->scenes[app->m_shared_data.current_scene]->handle_input(
                &app->m_shared_data, app->scenes[app->m_shared_data.current_scene], e);
        }
    }
}