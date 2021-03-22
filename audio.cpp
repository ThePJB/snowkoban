#include "audio.hpp"
#include <stdio.h>

audio audio_init() {
    printf("initializing audio...\n");
    audio a = (audio) {0};

    for (int i = 0; i < NUM_CS; i++) {
        a.relative_volume[i] = 1.0;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("failed to open audio\n");
        exit(1);
    }

    a.bgm = Mix_LoadMUS("assets/shades-of-spring.mp3");
    a.sounds[CS_LOSE] = Mix_LoadWAV("assets/lose.ogg");
    a.sounds[CS_SLIP] = Mix_LoadWAV("assets/slide.wav");
    a.sounds[CS_SNOW_FOOTSTEP] = Mix_LoadWAV("assets/snow-footstep-01.wav");
    a.sounds[CS_UNDO] = Mix_LoadWAV("assets/undo.ogg");
    a.sounds[CS_WIN] = Mix_LoadWAV("assets/win.ogg");
    a.sounds[CS_MOVEFINAL] = Mix_LoadWAV("assets/movefinal.ogg");
    a.sounds[CS_MENU_MOVE] = Mix_LoadWAV("assets/menu-move.wav");
    a.sounds[CS_MENU_SELECT] = Mix_LoadWAV("assets/menu-select.wav");

    a.relative_volume[CS_WIN] = 0.5;
    a.relative_volume[CS_MOVEFINAL] = 0.4;
    a.relative_volume[CS_MENU_MOVE] = 0.4;
    a.relative_volume[CS_MENU_SELECT] = 0.4;
    a.relative_volume[CS_LOSE] = 0.4;
    a.relative_volume[CS_SNOW_FOOTSTEP] = 0.8;
    
    Mix_AllocateChannels(NUM_CS);

    Mix_PlayMusic(a.bgm, -1);
    Mix_VolumeMusic(64);

    audio_set_sfx_volume(&a, 0.7);
    audio_set_bgm_volume(&a, 0.0);

    return a;
}

void audio_play(audio *a, channel_sound s) {
    if (s == CS_NONE) return;
    int volume = a->relative_volume[s] * 0.5 * (float)SDL_MIX_MAXVOLUME;
    Mix_PlayChannel(s, a->sounds[s], 0);
}

void audio_set_sfx_volume(audio *a, float v) {
    a->sfx_volume = v;
    for (int i = 0; i < NUM_CS; i++) {
        Mix_Volume(i, a->relative_volume[i] * v * SDL_MIX_MAXVOLUME);
    }
}

void audio_set_bgm_volume(audio *a, float v) {
    a->bgm_volume = v;
    Mix_VolumeMusic(SDL_MIX_MAXVOLUME * v);
}