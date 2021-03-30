#include "world.hpp"

world make_workshop(gef_context *gc) {
    const char *levels[] = {
R"(nice to sea you
#########
####t####
#### ####
#wwwwwww#
#wwwwwww#
#w     w#
#w cpc w#
#w  b  w#
#w     w#
#wwwwwww#
#########
)",
R"(quadrant
#############
#tt   w     #
#     w   c #
#     w   c #
#     w     #
#wwwwwwwwwww#
#     w     #
# b   w  p  #
#     w bc  #
#     w     #
#############
)",
R"(rescue mission
############
#wwwwwwwwww#
#wwwwbwwwww#
#wwwwwwwwww#
###//////###
# p//////  #
#  //////  #
#ww   cc   #
#ww  ccc   #
#ww  ccc   #
#ww  t     #
############
)",
R"(wet n wild
###########
#         #
#    t    #
#         #
##w########
#         #
#  c      #
#      #B #
#  p      #
#         #
###########
)",
R"(sequence 3
#################
#/////////w/Tw/T#
##   cc   #######
##   cc   #######
## p bb   #######
##        #######
#################
)"
    };
    return world("da workshop", levels, sizeof(levels)/sizeof(levels[0]), gc, gef_rgba(255, 255, 255, 100));
}