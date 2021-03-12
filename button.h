#pragma once

#include <stdbool.h>
#include "gef.h"

typedef struct {
    colour bg;
    colour line;
    colour text;
    colour highlight;
    font_handle f;

    int line_thickness;
} button_style;

void button_generic_draw(gef_context *gc, button_style bs, SDL_Rect r, char *text, bool rollover);

void button_bool_draw(gef_context *gc, button_style bs, SDL_Rect r, char *text, bool value, bool rollover);

void button_percent_draw(gef_context *gc, button_style bs, SDL_Rect r, char *text, float value, bool rollover);

void button_int_draw(gef_context *gc, button_style bs, SDL_Rect r, char *text, int value, bool rollover);


