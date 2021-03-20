#include "level_set.hpp"

(level_set) {
    .name = "Da Ice Zone",
    .background = gef_rgb(130, 130, 200),
    .highlight = gef_rgb(130, 130, 255),
    .levels = {
        "ice to meet you\n"
        "#########\n"
        "######t##\n"
        "###////##\n"
        "#p ///B/#\n"
        "# #////##\n"
        "# #////##\n"
        "#      ##\n"
        "#########\n",

        "insert ice pun\n"
        "########\n"
        "####t###\n"
        "#//////#\n"
        "#pb//#/#\n"
        "#//////#\n"
        "#////#/#\n"
        "#/#////#\n"
        "#//////#\n"
        "########\n",
        
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

        // broken and also kinda shitty
        "idk\n"
        "###########\n"
        "# b t######\n"
        "# #/////BT#\n"
        "#  /####/##\n"
        "#  /####/##\n"
        "#  P####/##\n"
        "#  //////##\n"
        "###/#######\n"
        "###########\n"
        "###########\n",

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

        // this is kinda nasty, maybe the version where theres one in the corne would be easier
        "dont mess up\n"
        "########\n"
        "##T#####\n"
        "#T/////#\n"
        "##/ bb/#\n"
        "##/   T#\n"
        "##/ b /#\n"
        "##/ p /#\n"
        "##/////#\n"
        "########\n",

        // maybe its not obvious you cant get back to the winning state of the start
        "rotational symmetry\n"
        "#############\n"
        "#########//##\n"
        "#########/B##\n"
        "#/T/////////#\n"
        "#/B///B///BT#\n"
        "#///////////#\n"
        "#///////////#\n"
        "#T///////// #\n"
        "#////////PT/#\n"
        "#############\n",

    },
    .completed = {false},
    .num_levels = 10,
    .total_presents = 21,
    .collected_presents = 0,
}