## Mechanics Ideas
 - Snowman balls where you push them and they grow
 - tnt boxes that take something out and make it ground
 - crumbly block no going back
 - multiple players (cloning machine?)
 - ice cube that slides no matter what its on
 - what if it was about building a snowman (getting the balls of snow to the right size where they go up by 1 every time you push on snow so you have to find a traversal of the correct distance, there would be bases and the rule is 3 high decreasing by 1)
  - have dirt, thin snow, deep snow
 - water is better flavour than hole lol and you can play a splash animation
 - tree you can walk through but not push blocks through
 - sleigh that can only be pulled lol
 - conveyor belts
 - barricades for N presents
 - wrapping machine: boxes into presents

## Game Features
 - maybe a screen when u start a new game thats like press u to undo
 - maybe more dopamine when you finish a level like fireworks particles and it says winner lol
 - present collect, ok that would be easy to add
 - more cohesion would be nice. like maybe track the total number of presents so theres a number that goes up.
 - bonus levels where u just pump
 - extra hard collectibles like coloured santa hats would be cool
 - immersive overworld would be cool but maybe a ceebs, idk how undos would work if you had actual puzzles in there, and also camera panning. maybe divide into stages. could be cool to chunk levels that way
 - custom levels in main menu
 - in game menu (scene stack?)
 - reset, undo buttons that also hint     <<<<< this is a good idea
 - settings - write to disk
 - res - text input
 - write save to disk
 - mobile
 - full screen
 - level select preview
 - tutorial
 - seek first incomplete level

 // how to communicate state? maybe just write it to shared data tbh. simplest solution


## Menu annoing
 - sound shrill
 - sliders



## Bugs
 - record undo points from just running at a wall
 - music rewinds to zero sometimes
 - wipe is like really sickness inducing at high res lol
   - maybe i could do a blizzard wipe or something that wouldn't be so chunderific
   - or wipe could be slower
 - kick em to the menu every level maybe?
 - mouse for menus
 - maybe further back music skip would be good
 - audio is a bit quiet on eg sliding ice
 - snow scramble just the first time u change level, weird (I think fixed)
 - buffer input only for taps

## Required juice
 - nice reset
 - better sounds
 - jame music
 - nicer font
 - big font for titles too
 - steam banner
 - snow particles when u walk
 - tilt player when sliding?
 - at the end presents hoover into ur total
 - nice transitions
 - maybe just back to level menu after each level
 - nicer level done - green border, tick sprite

 ## Level menu stuff
 - maybe just one world at a time on screen, like how super meat boy does
 - then u can have bigger chunks, nice previews, progressive disclosure etc
 - level, world numbers for presentation
 - nicer font plus text drawing hole fuark

 ## Level Concepts
 - ice rings
 - ice lines
 - one way corner
 - newtons cradle (kinda doesnt work, too commutative and blocks and player are interchangeable)
 - crates in the way
 - diode
 - ice to unstick
 - break the crate foundation
 - use present to block before taking it all the way
 - multi stage levels
 - any interesting ice partitioning levels stuff

# Level groupings
 - basic sokoban shit, sticky etc
 - welcome to ice
 - welcome to boxes
 - pushing things onto ice

----------------------------

LEVEL MENU looking dope. todo:
better layout on the buttons
track
maybe different snow characteristics
wipe
travel LR
kick u back to where u were and u go right to the next levels


---------------

fucking level menu getting entered every time u go left or right. fix

ok level menu working, a bit slow for going between worlds
could make it have level textures, would that look good? could have a drawing of a level, backgrounds

---------------------

sound design continues to be hard, and menus hard. well we will persevere. always ingame menus holy shit

double play of present dopamine sound is really annoying to fix lol because we play in movt logic but win sound comes at a different point and doesnt have the information of what presents are in place, so i can add state to keep track of that.....
be good if the sounds played off each other well and double play was good


--------------

loot box presents
it can look different and be a meme

both ends is dumb

snow rewind annoying

level menu wipe too slow
level menu kinda pointless
level menu left/right bug

-------------------

cool transition: render to texture and replace
plus it need not know about the underlayer
so it could be like
```
scene_mgr.switch_to_level(10);

and scene_mgr calls 
scene_stack.push(level(10))
scene_stack.push(transition(renderfunc))

or just call em yourself I guess

could also add some new mechanics like the water one
