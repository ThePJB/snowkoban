#include <stdio.h>
#include <unistd.h>
#include "application.h"
#include "util.h"


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

    application app = {0};
    application_init(&app, 1600, 900, do_level_start, level_num);
    
    double dt = 0;
    while (app.shared_data.keep_going) {
        int64_t tstart = get_us();

        application_handle_input(&app);
        application_update(&app, dt);
        application_draw(&app, dt);
        
        int64_t tnow = get_us();
        int64_t dt_us = tnow - tstart;
        if (dt_us < frame_us) {
            int64_t remaining_us = frame_us - dt_us;
            usleep(remaining_us);
        }


        dt_us = get_us() - tstart;
        dt = (double)dt_us / 1000000.0;
    }

    return 0;
}