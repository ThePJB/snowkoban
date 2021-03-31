#include "world.hpp"

world make_world_mp(gef_context *gc) {
    const char *levels[] = {
        "multiplayer 1\n"
        "###############\n"
        "##w   pbpt   w#\n"
        "###############\n"
        "#     p       #\n"
        "#             #\n"
        "#             #\n"
        "#tb    #    bt#\n"
        "###############\n",

// no need to keep alive
R"(multiplayer 2
##############
#w # t       #
#w #     /   #
#w #     /   #
#  #         #
#/w#   /B//  #
#/w#   ////  #
#p #p        #
##############
)",

R"(multiplayer 3
##############
#T#T#        #
#B#B#        #
#/ /   //    #
#/#/#  //    #
#####  // c  #
#      //    #
#w # w  w    #
#w # w  #    #
# p#pw  w    #
#  #         #
##############
)",

R"(multiplayer 4
##################
#       #   ww  t#
#   cb  #        #
#w      #        #
#  ##   #  ##    #
#   # ww# b #    #
#pw # wt#p  #    #
##################
)",

R"(multiplayer 5
#################
#             #/#
#t            B/#
#             #/#
#              /#
##             /#
#             ###
#/////////////BT#
#  #          ###
#pp#            #
#################
)",
R"(march of thousands
########
#      #
#  t   #
#  b   #
#      #
#      #
#wwwwww#
#wwwwww#
#wwwwww#
#wwwwww#
#wwwwww#
#      #
#  #   #
# p    #
#  pp  #
# ppp  #
#      #
#      #
########
)",
R"(mirrorverse
###############
#p     #p     #
#    c #  w c #
#  b   #  b   #
#      #      #
# c  T # c  T #
#      #      #
###############
#p     #p     #
#    c #    c #
#  b   #  bw  #
#  w   #      #
# c  T # c  T #
#      #      #
###############
)",
R"(desync 2
###############
##     ##     #
#p     #p     #
#    c #  w c #
#  b   #  b   #
#      #      #
# c  T # c  T #
##     ##     #
###############
##     ##     #
#p     #p     #
#    c #    c #
#  b   #  bw  #
#  w   #      #
# c  T # c  T #
##     ##     #
###############
)",
R"(desync
##########################
#p     w#p     w##########
#    c ##  w c ###########
#  b   ##  b   ###########
#      ##      ###########
# c  T ## c  T ###########
#      ##      ###########
##########################
#p     w#p          bt####
#    c ##    c ###########
#  b   ##  bw  ###########
#  w   ##      ###########
# c  T ## c  T ###########
#      ##      ###########
##########################
)",
R"(pp world
################
#       wbt#####
#       ########
#   ############
#   #w    /    #
#w w#w         #
#w             #
#wpp           #
################
)",
    };   
    return world("da multiplayer world", levels, sizeof(levels) / sizeof(levels[0]), gc, gef_rgba(255, 0, 0, 100));
}