#include "level_set.h"

(level_set) {
    .name = "Noice",
    .background = gef_rgb(30, 30, 200),
    .highlight = gef_rgb(30, 30, 255),
    .levels = {
        "first level\n"
        "#####\n"
        "## t#\n"
        "# b #\n"
        "#   #\n"
        "##p##\n"
        "#####\n",

        "second level\n"
        "######\n"
        "#t t##\n"
        "##   #\n"
        "##b  #\n"
        "## b #\n"
        "##   #\n"
        "###p##\n"
        "######\n",

        "one step forward, 2 steps back\n"
        "########\n"
        "# tb   #\n"
        "#p #   #\n"
        "# bt   #\n"
        "########\n",

        "sticky\n"
        "########\n"
        "#     p#\n"
        "#   bb #\n"
        "#  #####\n"
        "#     t#\n"
        "#     t#\n"
        "########\n",

        "sticky 2\n"
        "##########\n"
        "#        #\n" // clever its another trap
        "# t      #\n"
        "# #### # #\n"
        "#      # #\n"
        "# bp # # #\n"
        "#    #   #\n"
        "##########\n",

        "sticky 3\n" // basically if its on the wall respect it
        "######\n"
        "#    #\n"
        "# pbt#\n"
        "#    #\n"
        "#    #\n"
        "#    #\n"
        "#    #\n"
        "#   b#\n"
        "# t  #\n"
        "######\n",
    },
    .completed = {false},
    .num_levels = 6,
    .total_presents = 10,
    .collected_presents = 0,
}