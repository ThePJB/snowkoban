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

## Required juice
 - nice undos
   - ok this was fun to make but i dont keep enough information about entity positions etc and undo logic is bad enough
 - nicer resets would be good though, pop through the undos quickly
 - better sounds
 - nicer font
 - steam banner
 - snow particles when u walk
 - tilt player when sliding
 - at the end presents hoover into ur total

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

 ----------------------

--------------------

ok doing the level prototype thing, i think thats pretty clean.
TODO tomoz is just worlds init, and the plumbing which hopefully isnt bad cause its still structured the same. game will just take level proto from world instead of char* to do an initialize

----------------------------

LEVEL MENU looking dope. todo:
better layout on the buttons
track
maybe different snow characteristics
wipe
travel LR
kick u back to where u were and u go right to the next levels
