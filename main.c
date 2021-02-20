#include <stdio.h>
#include "application.h"

int main(int argc, char** argv) {

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
    
    while (app.ad.keep_going) {
        gef_start_frame(&app.gc);
        application_handle_input(&app);
        application_draw(&app);
        gef_end_frame(&app.gc);
    }

    return 0;
}