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
#ww    c   #
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
)",
R"(iceberg
###########
#t        #
#    c    #
#/wwwwwwww#
#/w/////ww#
#/w//B//ww#
#/w/////ww#
#/wwwwwwww#
#    c    #
#        p#
###########
)",
R"(penis
##############
#wwwwwwwwwwww#
#wp    ww   w#
#w  c  ww b w#
#w  c  ww   w#
#wwwwwwwwwwww#
#w  //#  c  w#
#wtt/w      w#
#w  //#  c  w#
#wwwwwwwwwwww#
#w   w ww   w#
#w   w ww b w#
#w   w ww   w#
#wwwwwwwwwwww#
##############
)",
R"(epic chris level
#########################
###########T#############
###########T#############
###########T#############
######### B//############
######### B//############
#########/C//############
#########/C//############
#########/C//############
#########/C//############
#########/// ############
#########//BC############
#########///C############
###/CCCCCCC/////////CCC##
#TT/////B//P//B////////T#
###//////////bcccccccc/##
###/////// /           ##
##########/BB ###########
##########//B ###########
##########//B ###########
##########/// ###########
###########T#############
###########T#############
###########T#############
###########T#############
#########################
)",
R"(level name
#######
###T###
###/###
###/###
##//  #
##//C #
##//B #
##w/B #
# /p  #
#  /w##
# c//##
# c//##
#  //##
###/###
###/###
###T###
#######
)",
R"(water gay
#######
###T###
###/###
###/###
##//  #
##/// #
##//B #
## /B #
# /p  #
#  / ##
#  //##
# c//##
#  //##
###/###
###/###
###T###
#######
)"
// # wall / ice ' ' snow p player b present c crate w water (lowercase snow underneath uppercase ice underneath)

    };
    return world("da workshop", levels, sizeof(levels)/sizeof(levels[0]), gc, gef_rgba(255, 255, 255, 100));
}