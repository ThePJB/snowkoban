#include "button.h"
#include "dankstrings.h"
#include <stdio.h>

void button_generic_draw(gef_context *gc, style *s, SDL_Rect r, const char *text, bool rollover) {
    SDL_Rect line_rect = (SDL_Rect) {
        r.x - s->line,
        r.y - s->line,
        r.w + s->line * 2,
        r.h + s->line * 2,
    };

    if (rollover) {
        gef_draw_rect(gc, s->highlight, line_rect.x, line_rect.y, line_rect.w, line_rect.h);
    } else {
        gef_draw_rect(gc, s->btn_line_colour, line_rect.x, line_rect.y, line_rect.w, line_rect.h);
    }

    gef_draw_rect(gc, s->btn_colour, r.x, r.y, r.w, r.h);
    
    gef_draw_bmp_text_centered(gc, s->game_font, s->big, text, r.x + r.w/2, r.y + r.h/2);
}

void button_bool_draw(gef_context *gc, style *s, SDL_Rect r, const char *text, bool value, bool rollover) {
    char buffer[256] = {0};
    sprintf(buffer, "%s: %s", text, value ? "ON" : "OFF");
    button_generic_draw(gc, s, r, buffer, rollover);
}

void button_percent_draw(gef_context *gc, style *s, SDL_Rect r, const char *text, float value, bool rollover) {
    char buffer[256] = {0};
    sprintf(buffer, "%s: %d%%", text, (int)(value * 100));
    button_generic_draw(gc, s, r, buffer, rollover);
}

void button_int_draw(gef_context *gc, style *s, SDL_Rect r, const char *text, int value, bool rollover) {
    char buffer[256] = {0};
    sprintf(buffer, "%s: %d", text, value);
    button_generic_draw(gc, s, r, buffer, rollover);
}


