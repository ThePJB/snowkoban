#include "audio.h"

audio audio_load_sounds() {
    audio a = (audio) {
        .bgm = Mix_LoadMUS("assets/shades-of-spring.mp3"),
        .snow_footstep = Mix_LoadWAV("assets/snow-footstep-01.wav"),
        .slip = Mix_LoadWAV("assets/slide.wav"),
        .undo = Mix_LoadWAV("assets/undo.ogg"),
        .win = Mix_LoadWAV("assets/win.ogg"),
        .movefinal = Mix_LoadWAV("assets/movefinal.ogg"),
        .lose = Mix_LoadWAV("assets/lose.ogg"),
    };

    Mix_AllocateChannels(NUM_CS);

    Mix_PlayMusic(a.bgm, -1);
    Mix_VolumeMusic(64);
    Mix_VolumeChunk(a.movefinal, 64);
    Mix_VolumeChunk(a.win, 70);
    //Mix_FadeInMusic(app->bgm, -1, 2000);

    return a;
}