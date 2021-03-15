const char *levels[] = {
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



    // world2




    // dumb
    "flanking\n"
    "##########\n"
    "######  /#\n"
    "#  b p #/#\n"
    "#/####t#/#\n"
    "#/######/#\n"
    "#/######/#\n"
    "#////////#\n"
    "##########\n",







    "prettyIs\n"
    "###############\n"
    "#####     #####\n"
    "#####  t  #####\n"
    "#####     #####\n"
    "#####     #####\n"
    "#   #/////    #\n"
    "#    /////    #\n"
    "# b  //p//    #\n"
    "#    /////    #\n"
    "#    /////    #\n"
    "#####     #####\n"
    "#####     #####\n"
    "#####     #####\n"
    "#####     #####\n"
    "###############\n",



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


    "block puzzle 3\n"
    "###########\n"
    "# b    t  #\n"
    "#    /t # #\n"
    "#   #b#b# #\n"
    "#p     t  #\n"
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

    // obviously cool but also broken, its a commutativity problem

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







/*
                _          
  ___ _ __ __ _| |_ ___  ___ 
 / __| '__/ _` | __/ _ \/ __|
| (__| | | (_| | ||  __/\__ \
 \___|_|  \__,_|\__\___||___/
                             
*/


    "crate to meet you\n"
    "#########\n"
    "####t####\n"
    "#   c   #\n"
    "#    b  #\n"
    "#  p    #\n"
    "#########\n",

    // long chains of crates certainly has design space
    "blockchain\n"
    "#############\n"
    "######t #####\n"
    "# cccccccc  #\n"
    "#           #\n"
    "#    pb     #\n"
    "#           #\n"
    "#############\n",





    "lol\n"
    "################\n"
    "#    #    #    #\n"
    "#t   #T   #t   #\n"
    "#b   #b   #B   #\n"
    "#    #    #    #\n"
    "#p             #\n"
    "#              #\n"
    "#              #\n"
    "################\n",















        // might have been broken too
    "hole punch\n"
    "############\n"
    "#   #      #\n"
    "#tb #      #\n"
    "#//////////#\n"
    "#//////////#\n"
    "#//////////#\n"
    "#//////////#\n"
    "## ###c###c#\n"
    "#p         #\n"
    "############\n",


    // breakable, kinda hard
    "clear a path\n"
    "#############\n"
    "######T######\n"
    "###p  /   ###\n"
    "###   C   ###\n"
    "###   /  ####\n"
    "###   /   ###\n"
    "### B/C///T##\n"
    "###   /   ###\n"
    "###   /   ###\n"
    "###   C   ###\n"
    "###   /   ###\n"
    "###   C   ###\n"
    "###   B   ###\n"
    "###       ###\n"
    "#############\n",

    


    // how about the present shifta strikes back

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


    "a thing\n"
    "##############\n"
    "##////////####\n"
    "#//////////###\n"
    "#/##////////##\n"
    "#/#////////C/#\n"
    "#///B/////T#/#\n"
    "#/#////////#/#\n"
    "#////////###/#\n"
    "#////////###/#\n"
    "#P///////////#\n"
    "##############\n",



    // this would be 2 ez rn


    // build a crate transfer

    // just imagine a patch of ice and its ramifications

    // there has to be a sierpinski triangle level



    // take a block through 4 quadrants

    // try to make a hard level thats highly constrained, e.g. block and target are on the same wall


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
