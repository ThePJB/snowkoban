#include <stdio.h>
#include "application.h"

int main(int argc, char** argv) {
    application app = {0};
    application_init(&app, 1000, 1000);
    
    while (app.ad.keep_going) {
        gef_start_frame(&app.gc);
        application_handle_input(&app);
        application_draw(&app);
        gef_end_frame(&app.gc);
    }

    return 0;
}