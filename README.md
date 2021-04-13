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
 - write save to disk

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