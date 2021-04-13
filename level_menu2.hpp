#pragma once

#include "scene.hpp"



struct level_menu2 : scene {
    void draw(shared_data *app_d, double dt);
    void update(shared_data *app_d, double dt);
    void on_focus(shared_data *app_d);
    void handle_input(shared_data *app_d, SDL_Event e);

    void set_state(shared_data *app_d, level_menu_state s);
};