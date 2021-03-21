#pragma once

#include <SDL2/SDL.h>
#include "gef.hpp"
#include "entity.hpp"
#include "tile.hpp"
#include "vla.hpp"
#include "grid.hpp"

struct level_prototype {
    vla<entity> starting_entities;
    grid<tile> tiles;

    bool complete = false;
    
    SDL_Texture *preview;
    char* title;

    level_prototype(const char* level_str, SDL_Renderer * r);
};

struct world {
    const char *name;
    vla<level_prototype> lps;
    int total_presents;
    int num_presents_collected = 0;

    world(){};

    world(const char *name, const char** level_strs, int n_levels, SDL_Renderer * r);
};

world make_world1(SDL_Renderer *r);
world make_world2(SDL_Renderer *r);
world make_world3(SDL_Renderer *r);
world make_world4(SDL_Renderer *r);
world make_world5(SDL_Renderer *r);