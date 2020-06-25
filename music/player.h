#pragma once
#define CUT 1

enum MusicPlayerCommandKind {
    start,
    stop,
    start_stop,
    reset,
    skip,
    back,
    none,
};

void commandMusicPlayer(enum MusicPlayerCommandKind cmd);

void initMusicPlayer();
void musicPlayerCallback();
