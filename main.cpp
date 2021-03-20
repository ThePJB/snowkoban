#include <stdio.h>
#include <unistd.h>
#include "application.hpp"
#include "util.hpp"
#include "dankstrings.hpp"

//#define PROFILE

int main(int argc, char** argv) {
    const int frame_cap = 60;
    const int frame_us = 1000000 / frame_cap;

    int level_num = 0;
    bool do_level_start = false;

    for (int i = 0; i < argc; i++) {
        if (strings_equal(argv[i], "--level")) {
            do_level_start = true;
            i++;
            if (i < argc && strings_is_dec_int(argv[i])) {
                level_num = atoi(argv[i]);
            } else {
                printf("need an integer argument for level\n");
                return 1;
            }
        }
    }

    application app = application(1600, 900);
    
    double dt = 0.001;
    while (app.m_shared_data.keep_going) {
        int64_t tstart = get_us();

        app.handle_input();
        int64_t t_hi = get_us();

        app.update(dt);
        int64_t t_update = get_us();

        app.draw(dt);
        int64_t t_draw = get_us();

        #ifdef PROFILE
        printf("t_hi = %f, t_update = %f, t_draw = %f\n", (t_hi - tstart) / 1000.0, (t_update - t_hi) / 1000.0, (t_draw - t_update) / 1000.0);
        #endif
        int64_t t_now = get_us();
        
        int64_t dt_us = t_now - tstart;
        if (dt_us < frame_us) {
            int64_t remaining_us = frame_us - dt_us;
            usleep(remaining_us);
        }


        dt_us = get_us() - tstart;
        dt = (double)dt_us / 1000000.0;
    }

    return 0;
}