#include "gef.h"
#include "util.h"
#include "math.h"
#include "coolmath.h"

/*

How to make procedural snowflakes

takes time variable
and it needs to make them, have them move down, and delete them
so they exist over some interval made of noise etc


*/

void snowflake_draw(gef_context *gc, int x, int y, int s) {
    const colour snowflake_colour = gef_rgb(255,255,255);
    gef_draw_rect(gc, snowflake_colour, x + s, y, s, s);
    gef_draw_rect(gc, snowflake_colour, x, y + s, s, s);
    gef_draw_rect(gc, snowflake_colour, x + s, y + s + s, s, s);
    gef_draw_rect(gc, snowflake_colour, x + s + s, y + s, s, s);
}

void snowflakes_draw(gef_context *gc, int xres, int yres, float t) {
    const int pixel_size = 4;
    const int spacing = pixel_size * 3;
    const int starting_y = -spacing;

    const float slowness = 10;

    const float frequency = 10;
    const float sin_magnitude = 10;

    const int vxh_magnitude = 200;

    const int potential_xmax = sin_magnitude + vxh_magnitude;
    

    for (int starting_x = -potential_xmax; starting_x < xres + potential_xmax; starting_x += spacing) {
        float phase = hash_floatn(starting_x, 0, slowness);
        
        float tprime = (t + phase) / slowness;
        float h = cm_frac(tprime);
        float iteration = cm_floor(tprime);

        int vxh = hash_intn(starting_x + 9853*iteration, -vxh_magnitude, vxh_magnitude);
        float sin_phase = hash_floatn(starting_x + 1209*iteration, 0, 2*M_PI);

        int x = starting_x + h * vxh + sin_magnitude * sin(h * frequency + sin_phase);
        int y = h * (yres - starting_y) + starting_y;

        snowflake_draw(gc, x, y, pixel_size);
    }
}

