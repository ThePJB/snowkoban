#include "application.hpp"
#include "levels.hpp"
#include "util.hpp"

application::application(int xres, int yres) : m_shared_data{shared_data(xres, yres, "Snowkoban!")} {
    scenes[SCENE_MAIN_MENU] = new main_menu();
    scenes[SCENE_SETTINGS_MENU] = new settings_menu();
    scenes[SCENE_LEVEL_MENU] = new level_menu();
    scenes[SCENE_GAME] = new game();
}

void application::update(double dt) {
    if (previous_scene != m_shared_data.current_scene) {
        previous_scene = m_shared_data.current_scene;
        scenes[m_shared_data.current_scene]->on_focus(&m_shared_data);
    }

    m_shared_data.time += dt;
    m_shared_data.abs_time += dt;
    m_shared_data.interp_time += dt;
    m_shared_data.interp_time += min(2*dt, 0.1 * (m_shared_data.time - m_shared_data.interp_time));
    
    scenes[m_shared_data.current_scene]->update(&m_shared_data, dt);

    return;
}

void application::draw(double dt) {
    gef_clear(&m_shared_data.gc);

    scenes[m_shared_data.current_scene]->draw(&m_shared_data, dt);

    gef_present(&m_shared_data.gc);
}

void application::handle_input() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            m_shared_data.keep_going = false;
            return;
        } else {
            scenes[m_shared_data.current_scene]->handle_input(&m_shared_data, e);
        }
    }
}