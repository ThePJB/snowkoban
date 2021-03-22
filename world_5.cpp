#include "world.hpp"

world make_world5(gef_context *gc) {
    const char *levels[] = {
        "Flintlock\n"
        "##############\n"
        "# p     ######\n"
        "#    c  ######\n"
        "#   c   ######\n"
        "### #/###/####\n"
        "#///C///T////#\n"
        "#####/#/#/####\n"
        "#     c  /  ##\n"
        "#   c b     ##\n"
        "#           ##\n"
        "##############\n",

        "Eeek\n" 
        "################\n"
        "################\n"
        "##########///###\n"
        "##///P///////#/#\n"
        "##/##//////C/#/#\n"
        "##/#/B/////////#\n"
        "##///////////#T#\n"
        "####//C//////#/#\n"
        "####//// c /   #\n"
        "####//// c / c #\n"
        "####/######/   #\n"
        "####////////#  #\n"
        "###########/####\n"
        "################\n",

        "squeeze\n"
        "#t######\n"
        "#      #\n"
        "# #b # #\n"
        "# //c/t#\n"
        "## bp b#\n"
        "####t  #\n"
        "#### c #\n"
        "####   #\n"
        "########\n",
    };
    return world("nightmare zone", levels, sizeof(levels)/sizeof(levels[0]), gc);
}