## Mechanics Ideas
 - Snowman balls where you push them and they grow
 - tnt boxes that take something out and make it ground
 - crumbly block no going back
 - multiple players (cloning machine?)
 - ice cube that slides no matter what its on
 - what if it was about building a snowman (getting the balls of snow to the right size where they go up by 1 every time you push on snow so you have to find a traversal of the correct distance, there would be bases and the rule is 3 high decreasing by 1)
  - have dirt, thin snow, deep snow

## Game Features
 - custom levels in main menu
 - in game menu (scene stack?)
 - reset, undo buttons that also hint 
 - write save to disk
 - mobile

## Bugs
 - no movt sound
 - i think high memory usage from fonts - maybe switch to a stylized bitmap font
 - record undo points from just running at a wall

## Required juice
 - snow wipe. maybe i can get there with 1 variable and some moddy tricks. Keep the multiple of xres and add xo as the mod part
    - wipiing is annoying lol print out wipe state
 - level number
 - 3d or 2d, autotile warcraft style would probably work
 - if 2d, how we doing scaling / size enforcement, have different zoom levels and pad with wall? or fix lvl sizes
 - maybe enforcing a certain level size / aspect ratio, or just center it for drawing and surround with WALL_TEXTURE?


## Non specific issues
 - whats with memory usage lol
 - fix sound design
 - fix art
   - todo mine celeste for inspo
 - main menu art
 - steam banner thing
 - tone and theme
  - needs to maybe be funny and christmasy but not really, pivot each time you unlock a mechanic?
  - fake rpg?

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


 --------

ok i fixed sliding and stuff i think
todo 
 - fix holes
 - input buffering
 - sound
 - animations etc
 - split target from ents and make it a tile? target/snow and target/ice? eh not until the need arises
 - split player from entities? eh
 - was tired for level design maybe tomorrow. I did put in some good hours today nice job me


 ok somethings fucked in history land
 at one point it tried to allocate like infinity memory and failed, idk why (after several big pauses from i guess the same thing presumably)
 could probably unit test
 idk which layer but probably history layer

 a sokoban rules overworld and like collectibles would be really cool

 conveyor belts

 barricades for N presents


diode and double diode

leaking font thing is that bad?

---------

 oo a fractal / procedural walking sim would be cool



wiping is fuggen annoying
-------------------------

in shared data have float wipe thats like incs whenever you go forward or decs whenever you go back. by 2 because 1 is for wipe out and 1 is for wipe in
then use that to calculate wipe offsets and snow offset

ok its fine now, maybe if i make the menus wipe later I will