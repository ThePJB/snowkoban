## Mechanics Ideas
 - Snowman balls where you push them and they grow (its a different puzzle game)
 - snow -> water tile
 - 2x1 presents
 - water -> ice?
 - evil twin (purple and does opposite movt to you)
   - what if u move into the same square lol

## Juice List (unimplemented)
 - movt particles
 - extra dopamine for level finishing e.g. fireworks
 - bonus levels maybe
 
## Juice Problems
 - shit sounds
 - shit font
 - number of presents? who cares really. could do a world of goo thing and have a big present room
 - art style - something better for lo fi? like baba is you does
 - waiting for james music
 - maybe dont rewind music its buggy and annoying
 - waiting for wipe / motion sickness, maybe do something else like cool diagonal colour wipe thing
 - rewind looks cool, maybe do make undo sound or maybe make own sound?

## QoL
 - make inputs only buffer a little bit maybe
 - better way to hint undo. maybe take control away on the first level and make them press u
 - undo history trimming

## The Question of the Overworld
 - could copy baba is you
 - could have multiple layers maybe for da chunking
 - could have a few movement puzzles and stuff
 - how to arrange levels: maybe go back to a big flat list of them
 - overworld is just normal levels, with embedded links to other levels (ref index or name, probably name)
 - level format then, would have 4,4,prison break etc
 - level format should check validity
 - also for game testing just have 
 - hover a preview somewhere? maybe mouse rollover
 - could sneak little side levels into other levels
 - measure completeness - how many levels finished, could have a little tracker in the overworld with ur little elf dude
 - overworld appearance - tastefully downsample tiles? for that matter just make mipmaps for scaled levels?

## Game Features
 - custom levels in main menu
 - in game menu (scene stack?)
 - reset, undo buttons that also hint     <<<<< this is a good idea
 - reset not a button, move to menu
 - settings
   - write to disk
   - res - text input (also depend scene stack)
   - slider
   - full screen

double play of present dopamine sound is really annoying to fix
be good if the sounds played off each other well and double play was good
e.g. an octave

--------------
da chris zone:
loot box presents
it can look different and be a meme

both ends is dumb

snow rewind annoying?

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
--------
can u make a very expressive state machine object?

template of enum
get state, state_t, maybe different_state_t

maybe a callback or maybe an interface with virtual methods...

 - pure state transition function: state, t -> state
 - on_enter function: new_state, old_state -> ()
  - basically switch (new_state) and switch(old_state), probably looks like old update function

should make transitions really explicit though

or

 - table of callbacks for each state variable. could return new state
  - callbacks[STATE_REWIND] = [&game](){return state_t > 1 ? STATE_REWIND : STATE_NONE}
    kind of nice

maybe very very simple one that just has an update function and a set state function
maybe with table dispatch?

----------------------
new level menu
 - if it animated more smoothly
 - prob no kick back every level
 - smooth transitions
    - maybe just save a texture and do transitions in application or something. e.g. save texture and do a cool wipe. or like the screen shoots off spinning
 - bigger levels?
 - get rid of side pane and just float title above the level

thing is theres heaps of small levels so youve got a lot to organize. You can make a lot nicer ones if you just show off one (or a handful) at a time

good isnt it amazing what you can get done if you actually sit down and focus

res scaling

the ones with just 4 or 5 levels look pleasing

could maybe indicate difficulty with skulls or something and let em ramp, do it parallel like. if they can understand the difficulty increasing

-------------------------------------

Design stuff:
 - gradients potentially good eg https://uigradients.com/#DarkOcean
 - HSV good
 - probably not hard to visualize or make a colour scheme picker etc
 - some palette techniques
  - greyscale with one accent colour
  - 5colours, brightest 95% and darkest 20%
  - slight blue to greys if u like
  - analagous schemes - primary, secondary and some dark boys (dark boys same hue as secondary)
    - primary, secondary not that different. alternet saturation and brightness
  - complementary - as analagous but opposites like ikea - kinda cringe
  - triad hard lol
OK well I jacked a scheme from that post. its not like the closest match up ever but at least it looks nice lol.
a bit dark and spooky

probably wouldnt kill to match it up with the assets. 
the bow on the present kind of matches
blue of ice?
grey of wall? defs make cooler
green with his suit
also the wheat colour of the crate

definitely chilling out the wall looks good


-------------
maybe dont allow key repeats over scene transitions hey

rounding errors in some of the level previews? what can be done

>> stop double exiting

paint background

pulse animate all the levels in the level menu when you finish a world, and play a sound
animate worlds turning to done would be a good idea

paint bg in main menu
stop kicking out of levels. that will be a big polish. its getting pretty polished atm mmmm

im pretty glad i made this bitmap font for my tower defence game lol. how many px is super crate box?

can you get an SDL context faster or wot?
i do decide what to do in a kind of haphazard way lol

snow state - I could just make it 

animate on rollover/highlight. that would look awesome lol

levels arent completed

flash title of next level
maybe wrong next level at some point

bugs make you hate your game. like jbp on that dont let your children resent you

recolouring assets
- better green on the dude
- colder walls
- redo water

---------------------------

what now?

 - prune shitty levels - i can do that
 - game->game transitions - double buffering games i guess lol. maybe that fix the title flashing thing as well.
 - just have another scene and update carefully
  - should i refactor some shit to be like progress_mgr? nah that sounds gay
 - flash title of next level - idk how to fix really, maybe just stop displaying title instantly
 - keys over scene boundaries - idk
 - let player move, speed of light? sounds hard tho


 ---------------
 make level pop out, unvignette on highlight

 victory state
 "YOU WIN!"
 "WINNER!
 spin in lol
 * probably should have it fade out by the end of CELEBRATE

 Do like a victory screen roundup lol