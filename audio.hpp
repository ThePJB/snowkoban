#pragma once

#include <SDL_mixer.h>

typedef enum {
    CS_NONE,
    CS_SNOW_FOOTSTEP,
    CS_SLIP,
    CS_UNDO,
    CS_LOSE,
    CS_MOVEFINAL,
    CS_WIN,
    CS_MENU_MOVE,
    CS_MENU_SELECT,
    NUM_CS,
} channel_sound;

typedef struct {
    Mix_Music *bgm;
    Mix_Chunk *sounds[NUM_CS];
    float relative_volume[NUM_CS];
    float sfx_volume;
    float bgm_volume;
} audio;

audio audio_init();
void audio_play(audio *a, channel_sound s);
void audio_set_sfx_volume(audio *a, float v);
void audio_set_bgm_volume(audio *a, float v);
