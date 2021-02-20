#pragma once

#include <SDL_mixer.h>

typedef enum {
    CS_SNOW_FOOTSTEP,
    CS_SLIP,
    CS_UNDO,
    CS_LOSE,
    CS_MOVEFINAL,
    CS_WIN,
    NUM_CS,
} channel_sound;

typedef struct {
    Mix_Music *bgm;
    Mix_Chunk *snow_footstep;
    Mix_Chunk *slip;
    Mix_Chunk *undo;
    Mix_Chunk *lose;
    Mix_Chunk *movefinal;
    Mix_Chunk *win;

} audio;

audio audio_load_sounds();