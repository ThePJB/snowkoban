#include "gef.hpp"
#include "util.hpp"
#include <math.h>
#include "coolmath.hpp"

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

void snowflakes_draw(gef_context *gc, float t, int xo) {
    const int xres = gc->xres;
    const int yres = gc->yres;
    
    const int pixel_size = 4;
    const int spacing = pixel_size * 3;
    const int starting_y = -spacing;

    const float slowness = 10;

    const float frequency = 10;
    const float sin_magnitude = 10;

    const int vxh_magnitude = 200;

    const int potential_xmax = sin_magnitude + vxh_magnitude + abs(xo);
    
    int start_seed = -(potential_xmax/spacing) - 1;
    int end_seed = (potential_xmax + xres)/spacing + 1;
    
    for (int seed = start_seed; seed < end_seed; seed++) {

        float phase = hash_floatn(seed, 0, slowness);
        
        float tprime = (t + phase) / slowness;
        float h = cm_frac(tprime);
        float iteration = cm_floor(tprime);

        int vxh = hash_intn(seed + 9853*iteration, -vxh_magnitude, vxh_magnitude);
        float sin_phase = hash_floatn(seed + 1209*iteration, 0, 2*M_PI);

        int starting_x = seed * spacing;

        int x = starting_x + h * vxh + sin_magnitude * sin(h * frequency + sin_phase) - xo;
        int y = h * (yres - starting_y) + starting_y;

        snowflake_draw(gc, x, y, pixel_size);
    }
}

