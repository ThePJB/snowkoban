## Mechanics Ideas
 - Snowman balls where you push them and they grow
 - tnt boxes that take something out and make it ground
 - crumbly block no going back
 - multiple players (cloning machine?)
 - ice cube that slides no matter what its on

## Game Features
 - custom levels in main menu
 - you win screen
 - reset, undo buttons that also hint 
 - write save to disk
 - mobile

## Bugs
 - no hole sound if u slide into it
  - no ice sound if you go snowicesnow
  - needs to just call the sound thing unconditionally amongst the movement
 - is there memory leaks? or how to bring down memory usage
 - there is one phantom undo stack

## Required juice
 - print level name when u go into it
 - animated character and other stuff?
 - 3d or 2d, autotile warcraft style would probably work
 - if 2d, how we doing scaling / size enforcement, have different zoom levels and pad with wall? or fix lvl sizes
 - maybe enforcing a certain level size / aspect ratio, or just center it for drawing and surround with WALL_TEXTURE?
 - make snow effect smooth (quick) rewind will look even cooler
 - reflection in ice effect? with like an ice mask so u draw ice base, entity upside down, ice mask

 - might be weird that its when you made the move, not right before when you made the next move

 - maybe entities should actually just be entities instead of tiles
 - that would tidy up tile logic too

 - transition levels, then wind blows, snow blows, whoosh swipe to the next one


## Non specific issues
 - fix sound design
 - fix art
   - todo mine celeste for inspo
 - main menu art
 - steam banner thing
 - tone and theme
  - needs to maybe be funny and christmasy but not really, pivot each time you unlock a mechanic?
  - fake rpg?
 - bgm rewinds with undos

## Refactor
 - maybe tile prototypes:
  - moves
  - clip
  - something take care of the bit shifting
   - could have slides. lol if there were little ice skating penguins

 ## Level Concepts
 - got to have newtons cradling with one on snow in the middle of the ice!
 - 


 - crate knocks a block
  - need crate somewhere but it knocks a present so u have to move that first
 - crate gets in the way e.g. makes a corner so u cant push something
 like 
 - to get to area have to move through a crate that fucks a block unless i do something first

 one way ice: walking on it from the side then theres a recess when u come back in

 what about a whole level made of crates where you cant move any of them except like one to win

 push a chain of crates to unstick a present


 use ice to unstick 2 blocks stuck together


#######
  b tt#
b #####


# #####
 cb tt#
#b#####


##########
  b t///t#
b ########

or its ice, then open at the other end, and theres some use for it, but u had to use it to get first present in  location

fake receptacle, just a right angle with no space


hole puzzles:
you can push crates down to get rid of them
next to ice, you cant go that way on the ice but can still go across, AND no wall so you cant skate up to return to that row
so the hard part would be getting up to that part




 ## Aesthetic
Christmas is pretty fucking trash. What else is snow and icy?
 - i mean tongue in cheek christmas might be redeemable?
 - vikings
 - penguins
 - eskimos
 - yetis
 - you could be tibet santa
 - Siberia? escaped from russian prison.
