#pragma once

#include <stdbool.h>
#include "gef.hpp"
#include "scene.hpp"

void button_generic_draw(gef_context *gc, style *s, SDL_Rect r, const char *text, bool rollover);

void button_bool_draw(gef_context *gc, style *s, SDL_Rect r, const char *text, bool value, bool rollover);

void button_percent_draw(gef_context *gc, style *s, SDL_Rect r, const char *text, float value, bool rollover);

void button_int_draw(gef_context *gc, style *s, SDL_Rect r, const char *text, int value, bool rollover);
