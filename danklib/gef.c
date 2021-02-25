#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <unistd.h>

#include "util.h"
#include "stdint.h"
#include "gef.h"



gef_context gef_init(char *name, int xres, int yres) {
    printf("initializing graphics...\n");
    gef_context gc;

    gc.xres = xres;
    gc.yres = yres;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) gef_die(&gc, "couldn't init sdl");

    gc.window = SDL_CreateWindow(name, 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        gc.xres, 
        gc.yres,
        SDL_WINDOW_SHOWN);

    if (gc.window == NULL) gef_die(&gc, "couldn't create window");

    gc.renderer = SDL_CreateRenderer(gc.window, -1, SDL_RENDERER_ACCELERATED);
    if (gc.renderer == NULL) gef_die(&gc, "couldn't create renderer");

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) gef_die(&gc, "couldn't init SDL_img");
    if (TTF_Init() == -1) gef_die(&gc, "couldn't init SDL_ttf");
    
    return gc;
}

void gef_load_atlas(gef_context *gc, char *path) {
    SDL_Surface* loaded_surface = IMG_Load(path);
    gc->atlas = SDL_CreateTextureFromSurface(gc->renderer, loaded_surface);
    if (gc->atlas == NULL) gef_die(gc, "couldn't create texture");
    SDL_FreeSurface(loaded_surface);
}

font_handle gef_load_font(char *path, int size) {
    TTF_Font *f = TTF_OpenFont(path, size);
    return (font_handle) {
        .gfont = f,
    };
}

text_handle gef_make_text(gef_context *gc, font_handle f, char *text, int r, int g, int b) {
    SDL_Surface *text_surface = TTF_RenderText_Blended(f.gfont, text, (SDL_Color){.a=255, .r=r, .g=g, .b=b});
    SDL_Surface *text_shadow_surface = TTF_RenderText_Blended(f.gfont, text, (SDL_Color){.a=255, .r=0, .g=0, .b=0});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gc->renderer, text_surface);
    SDL_Texture *texture_shadow = SDL_CreateTextureFromSurface(gc->renderer, text_shadow_surface);
    text_handle t = (text_handle) {
        .texture = texture,
        .texture_shadow = texture_shadow,
        .w = text_surface->w,
        .h = text_surface->h,
    };

    SDL_FreeSurface(text_surface);
    

    return t;
}

void gef_draw_text(gef_context *gc, text_handle text, int x, int y) {
    SDL_RenderCopy(gc->renderer, text.texture_shadow, NULL, &(SDL_Rect) {x+2, y+2, text.w, text.h});
    SDL_RenderCopy(gc->renderer, text.texture, NULL, &(SDL_Rect) {x, y, text.w, text.h});
};

void gef_destroy_text(text_handle text) {
    SDL_DestroyTexture(text.texture);
    SDL_DestroyTexture(text.texture_shadow);
}

void gef_draw_sprite(gef_context *gc, SDL_Rect clip, SDL_Rect to_rect) {
    SDL_RenderCopy(gc->renderer, gc->atlas, &clip, &to_rect);
}

void gef_draw_sprite_ex(gef_context *gc, SDL_Rect clip, SDL_Rect to_rect, double angle, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(gc->renderer, gc->atlas, &clip, &to_rect, angle, NULL, flip);
}

void gef_clear(gef_context *gc) {
    SDL_SetRenderDrawColor(gc->renderer, 0, 0, 0, 255);
    SDL_RenderClear(gc->renderer);
}

void gef_draw_pixel(gef_context *gc, colour c, int x, int y) {
    SDL_SetRenderDrawColor(gc->renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawPoint(gc->renderer, x, y);
}

void gef_draw_square(gef_context *gc, colour c, int x, int y, int s) {
    SDL_SetRenderDrawColor(gc->renderer, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(gc->renderer, &(SDL_Rect){x, y, s, s});
}

void gef_draw_rect(gef_context *gc, colour c, int x, int y, int w, int h) {
    SDL_SetRenderDrawColor(gc->renderer, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(gc->renderer, &(SDL_Rect){x, y, w, h});
}

void gef_present(gef_context *gc) {
    SDL_RenderPresent(gc->renderer);
}


colour gef_rgb(int r, int g, int b) {
    return (colour) {r, g, b, 255};
}

colour gef_rgba(int r, int g, int b, int a) {
    return (colour) {r, g, b, a};
}


void gef_teardown(gef_context *gc) {
    SDL_DestroyRenderer(gc->renderer);
    SDL_DestroyWindow(gc->window);
    IMG_Quit();
    SDL_Quit();
}
