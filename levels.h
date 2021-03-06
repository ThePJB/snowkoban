const char *levels[] = {
    "test level\n"
    "#####################\n"
    "#                   #\n"
    "# p  b////////      #\n"
    "#    b/b//////      #\n"
    "#                   #\n"
    "#     ///BBB//      #\n"
    "#                   #\n"
    "#                   #\n"
    "#   t ttt           #\n"
    "#####################\n",

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

    "constraints\n"
    "########\n"
    "#     p#\n"
    "#   bb #\n"
    "#  #####\n"
    "#     t#\n"
    "#     t#\n"
    "########\n",


    "constraints 2\n"
    "##########\n"
    "#        #\n" // clever its another trap
    "# t      #\n"
    "# #### # #\n"
    "#      # #\n"
    "# bp # # #\n"
    "#    #   #\n"
    "##########\n",

    "constraints 3\n" // basically if its on the wall respect it
    "#########\n"
    "#       #\n"
    "#    pbt#\n"
    "#       #\n"
    "#       #\n"
    "#       #\n"
    "#       #\n"
    "#      b#\n"
    "#    t  #\n"
    "#########\n",



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

    "flanking\n"
    "##########\n"
    "######  /#\n"
    "#  b p #/#\n"
    "#/####t#/#\n"
    "#/######/#\n"
    "#/######/#\n"
    "#////////#\n"
    "##########\n",

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

    "dont mess up\n"
    "########\n"
    "##T#####\n"
    "#T/////#\n"
    "##/ bb/#\n"
    "##/ b T#\n"
    "##/ b /#\n"
    "##/ p /#\n"
    "##T////#\n"
    "########\n",

    // cant put it back square of squares
    "can't put it back\n"
    "###########\n"
    "#    t    #\n"
    "# ### ### #\n"
    "# ### ### #\n"
    "# ### ### #\n"
    "#  p B    #\n"
    "# ### ### #\n"
    "# ### ### #\n"
    "# ### ### #\n"
    "#         #\n"
    "###########\n",

    // there has to be a sierpinski triangle level

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


    "block puzzle\n"
    "###########\n"
    "#  b     t#\n"
    "#     /  t#\n"
    "#    #b#  #\n"
    "#p        #\n"
    "###########\n",

    
    "block puzzle 3\n"
    "###########\n"
    "# b    t  #\n"
    "#    /t # #\n"
    "#   #b#b# #\n"
    "#p     t  #\n"
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

    "railgun\n"
    "#########\n"
    "####t####\n"
    "####t####\n"
    "####t####\n"
    "#t  /   #\n"
    "# #b/ b #\n"
    "# # /   #\n"
    "# # /b  #\n"
    "#p# /   #\n"
    "####/## #\n"
    "####B   #\n"
    "####/ ###\n"
    "####/   #\n"
    "####/####\n"
    "#########\n",

    "loom\n"
    "#############\n"
    "#  ///#///###\n"
    "#  ///////tt#\n"
    "# b//#////###\n"
    "# b///////tt#\n"
    "# b/////#/###\n"
    "#  ///////tt#\n"
    "# #///////###\n"
    "#  bbb      #\n"
    "#p          #\n"
    "#############\n",

    "crate to meet you\n"
    "#########\n"
    "####t####\n"
    "#   c   #\n"
    "#    b  #\n"
    "#  p    #\n"
    "#########\n",

    "assist\n"
    "########\n"
    "#####t##\n"
    "#pb////#\n"
    "#  ////#\n"
    "#  ////#\n"
    "#     c#\n"
    "#      #\n"
    "########\n",

    "sequence 2\n"
    "###################\n"
    "#/////////////TT/T#\n"
    "## #    ###########\n"
    "##  c  b###########\n"
    "##   bb ###########\n"
    "##p     ###########\n"
    "###################\n",

    "2 roads\n"
    "##############\n"
    "#tc      #   #\n"
    "#///////////T#\n"
    "#        #   #\n"
    "# ###### #####\n"
    "# ###### #####\n"
    "# ###### #####\n"
    "#   bb   #####\n"
    "#        #####\n"
    "#   p    #####\n"
    "#        #####\n"
    "##############\n",

    "long way\n"
    "###############\n"
    "#######T#######\n"
    "#      /     t#\n"
    "#  p   B      #\n"
    "#      /      #\n"
    "#######/      #\n"
    "#######/      #\n"
    "#######/      #\n"
    "#   b  /      #\n"
    "#      /      #\n"
    "#      /#######\n"
    "#      /#######\n"
    "###############\n",

    "scaffolding\n"
    "###########\n"
    "#        t#\n"
    "#  p      #\n"
    "# bb /   t#\n"
    "# ### #####\n"
    "#   c c   #\n"
    "#  c c    #\n"
    "#         #\n"
    "###########\n",

    "I swear we had this before\n"
    "###############\n"
    "#      t      #\n"
    "#             #\n"
    "#######/#######\n"
    "#   ###/###   #\n"
    "# b ###/###   #\n"
    "#   ///P///   #\n"
    "#   ###/###   #\n"
    "#   ###/###  t#\n"
    "#######/#######\n"
    "# b      cccc #\n"
    "#             #\n"
    "###############\n",

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

    "icey nicey\n"
    "#############\n"
    "#  /  #  /  #\n"
    "# c/p # c/b #\n"
    "#///////////#\n"
    "# c/  # c/  #\n"
    "#  /  #  /  #\n"
    "###/#####/###\n"
    "#  /  #  /  #\n"
    "#  /  #  /c #\n"
    "#T/B////////#\n"
    "#  /  #  /  #\n"
    "#  /  #  / t#\n"
    "#############\n",


    // idk how to make the crossing thing useful really
    "voyage\n"
    "###################\n"
    "#          ####T###\n"
    "#          #   / ##\n"
    "#///B////////////T#\n"
    "#          #   /  #\n"
    "#          ####/###\n"
    "#          ####/###\n"
    "#          ####/###\n"
    "#          #   /  #\n"
    "#          #   /  #\n"
    "#          #   B  #\n"
    "#          #   /  #\n"
    "#          ####/###\n"
    "###################\n",

    // what about a bistable latch, states A and B
    // and ur in 1 or 2
    // 1 B you can use it but problem is its in 1A and 2B until you break it
    // can that exist?

    "hole punch\n"
    "############\n"
    "#   #      #\n"
    "#tb #      #\n"
    "#//////////#\n"
    "#//////////#\n"
    "#//////////#\n"
    "#//////////#\n"
    "##c###c###c#\n"
    "#p         #\n"
    "############\n",

    "one way street\n"
    "#################\n"
    "#    # /////////#\n"
    "#      /////////#\n"
    "#    / /////////#\n"
    "#ttt#/##/#//////#\n"
    "#####/#////////##\n"
    "#    /#////////##\n"
    "# bb /b////////##\n"
    "# p  /#///////###\n"
    "#    /#///#/////#\n"
    "#####/###########\n"
    "#################\n",
    
    "hole to meet you\n"
    "##########\n"
    "#####t ###\n"
    "# cccccch#\n"
    "#        #\n"
    "#   p    #\n"
    "##########\n",

    "drop off\n"
    "##hhhhhhhhhhh#\n"
    "##hhhhhhhhhhh#\n"
    "# /////B////T#\n"
    "# #////P/////#\n"
    "# #//////////#\n"
    "# #//////////#\n"
    "#////////////#\n"
    "##############\n",

    "winner\n"
    "ttttttttttttttt\n"
    "t     ###     t\n"
    "t     #P#     t\n"
    "t     ###     t\n"
    "t             t\n"
    "t#   # # ##  #t\n"
    "t# # # # # # #t\n"
    "t##### # #  ##t\n"
    "ttttttttttttttt\n",
};