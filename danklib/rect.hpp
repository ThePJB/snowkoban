#pragma once

#include <SDL2/SDL.h>

struct rect {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    rect(){};

    rect(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    static rect centered(int x, int y, int w, int h) {
        return rect(x - w/2, y - h/2, w, h);
    }

    rect dilate(int s) {
        return rect(x - s, y - s, w + 2*s, h + 2*s);
    }

    SDL_Rect sdl_rect() {
        return (SDL_Rect) {.x = x, .y = y, .w = w, .h = h};
    };

    bool contains(int p_x, int p_y) {
        return p_x >= x && p_x <= x + w && p_y >= y && p_y <= y + h;
    };
};