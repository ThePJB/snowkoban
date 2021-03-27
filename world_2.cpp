#include "world.hpp"

world make_world2(gef_context *gc) {
    const char *levels[] = {
        "ice to meet you\n"
        "#########\n"
        "######T##\n"
        "#TB/////#\n"
        "#P////B/#\n"
        "#///////#\n"
        "#///////#\n"
        "#//////##\n"
        "#########\n",
        
        "maze 1\n"
        "##############\n"
        "#########/####\n"
        "#########/T###\n"
        "##/////////###\n"
        "#//PB//////###\n"
        "###//////////#\n"
        "###//////////#\n"
        "###//#//#/#//#\n"
        "##############\n",

        "Patience\n"
        "##########\n"
        "#tb  #####\n"
        "# #////BT#\n"
        "#  //#####\n"
        "#  //#####\n"
        "# p/######\n"
        "##########\n",

        "block puzzle\n"
        "###########\n"
        "#  b     t#\n"
        "#     /  t#\n"
        "#    #b#  #\n"
        "#p        #\n"
        "###########\n",

        "cross\n"
        "#########\n"
        "####t##t#\n"
        "#  #/## #\n"
        "#  /B/t #\n"
        "#  #/## #\n"
        "#  #b# b#\n"
        "#   p   #\n"
        "#########\n",

        "diode\n"
        "#########\n"
        "#tb     #\n"
        "####### #\n"
        "#tb   # #\n"
        "#  #  # #\n"
        "#  /  # #\n"
        "###/### #\n"
        "#p      #\n"
        "#########\n",

        // people get really stuck on this, i wonder if its presentation
        /*
        "I\n"
        "#############\n"
        "#          t#\n"
        "#           #\n"
        "######/######\n"
        "#   ##/##   #\n"
        "# b ##/##   #\n"
        "#   //P//   #\n"
        "# # ##/##   #\n"
        "# / ##/##  t#\n"
        "##/###/######\n"
        "# b         #\n"
        "#           #\n"
        "#############\n",
*/
        "I\n"
        "#############\n"
        "######t######\n"
        "###### ######\n"
        "######/######\n"
        "######/######\n"
        "#   ##/######\n"
        "# b //P//  t#\n"
        "# # ##/######\n"
        "# / ##/######\n"
        "##/###/######\n"
        "#       #####\n"
        "#     b #####\n"
        "#       #####\n"
        "#############\n",

        "ringy ding\n"
        "########\n"
        "#T/////#\n"
        "##/ bb/#\n"
        "##/   T#\n"
        "##/ b /#\n"
        "##/ p /#\n"
        "##/////#\n"
        "##T#####\n"
        "########\n",

        // this is kinda nasty, maybe the version where theres one in the corne would be easier
        "ringy dingy\n"
        "########\n"
        "##T#####\n"
        "#T/////#\n"
        "##/ bb/#\n"
        "##/   T#\n"
        "##/ b /#\n"
        "##/ p /#\n"
        "##/////#\n"
        "########\n",

        "one way street\n"
        "#################\n"
        "#    # /////////#\n"
        "#      /////////#\n"
        "#    / /////////#\n"
        "#tt##/##/#//////#\n"
        "#####/#////////##\n"
        "#    /#////////##\n"
        "#  b /b////////##\n"
        "# p  /#///////###\n"
        "#    /#///#/////#\n"
        "#####/###########\n"
        "#################\n",
    };
    return world("icetown", levels, 10, gc, gef_rgba(100, 100, 255, 100));
}