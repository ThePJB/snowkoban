#include "button.h"
#include <stdio.h>

void button_generic_draw(gef_context *gc, button_style bs, SDL_Rect r, char *text, bool rollover) {
    SDL_Rect line_rect = (SDL_Rect) {
        r.x - bs.line_thickness,
        r.y - bs.line_thickness,
        r.w + bs.line_thickness * 2,
        r.h + bs.line_thickness * 2,
    };

    if (rollover) {
        gef_draw_rect(gc, bs.highlight, line_rect.x, line_rect.y, line_rect.w, line_rect.h);
    } else {
        gef_draw_rect(gc, bs.line, line_rect.x, line_rect.y, line_rect.w, line_rect.h);
    }

    gef_draw_rect(gc, bs.bg, r.x, r.y, r.w, r.h);

    text_handle t = gef_make_text(gc, bs.f, text, bs.text.r, bs.text.g, bs.text.b);
    gef_draw_text(gc, t, r.x + r.w/2 - t.w/2, r.y + r.h/2 - t.h/2);
    gef_destroy_text(t);
}

void button_bool_draw(gef_context *gc, button_style bs, SDL_Rect r, char *text, bool value, bool rollover) {
    char buffer[256] = {0};
    sprintf(buffer, "%s: %s", text, value ? "ON" : "OFF");
    button_generic_draw(gc, bs, r, buffer, rollover);
}

void button_percent_draw(gef_context *gc, button_style bs, SDL_Rect r, char *text, float value, bool rollover) {
    char buffer[256] = {0};
    sprintf(buffer, "%s: %d%%", text, (int)(value * 100));
    button_generic_draw(gc, bs, r, buffer, rollover);
}

void button_int_draw(gef_context *gc, button_style bs, SDL_Rect r, char *text, int value, bool rollover) {
    char buffer[256] = {0};
    sprintf(buffer, "%s: %d", text, value);
    button_generic_draw(gc, bs, r, buffer, rollover);
}


