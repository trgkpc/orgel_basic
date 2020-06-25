#include "../firmware/buzzer.h"
#include "player.h"
#include "score.h"
#include <stdbool.h>

#define BPM 105
#define CUT_TIME 20

#define SKIP_TIME 5000
#define BACK_TIME 5000

enum MusicPlayerCommandKind mp_command;

enum MusicPlayerMode {
    Playing,
    Stop,
};

enum MusicPlayerMode mp_mode;


void initMusicPlayer()
{
    mp_command = none;
    mp_mode = Stop;
}

void commandMusicPlayer(enum MusicPlayerCommandKind cmd)
{
    mp_command = cmd;
}


struct SoundData {
    uint16_t freq;
    uint16_t len;
    uint8_t flag;
};

struct SoundData getSoundData(uint16_t pointer)
{
    uint16_t f = pgm_read_word_near(score_freq + pointer);
    uint8_t l = pgm_read_word_near(score_len + pointer);
    uint8_t c = pgm_read_word_near(score_cut + pointer);
    struct SoundData data = {f, ((uint16_t)l) * BPM, c};
    return data;
}

void setSound(uint16_t freq, uint8_t flag)
{
    static uint16_t last_freq = 0;
    static uint8_t last_flag = 0;
    if (last_freq != freq || last_flag != flag) {
        set_buzzer(freq, flag);
        last_freq = freq;
        last_flag = flag;
    }
}

void stopSound()
{
    setSound(0, 0);
}

void musicPlayerCallback()
{
    static uint16_t last_sound_time = 0;
    static uint16_t pointer = 0;
    static struct SoundData data = {0, 0, 0};
    static bool init_music = true;

    if (mp_command == none) {
    } else if (mp_command == start_stop) {
        if (mp_mode == Stop) {
            mp_mode = Playing;
        } else {
            mp_mode = Stop;
        }
    } else if (mp_command == start) {
        mp_mode = Playing;
    } else if (mp_command == stop) {
        mp_mode = Stop;
    } else if (mp_command == reset) {
        init_music = true;
        last_sound_time = 0;
    } else if (mp_command == back) {
        last_sound_time += BACK_TIME;
        while (last_sound_time > data.len) {
            if (pointer == 0) {
                init_music = true;
                last_sound_time = 0;
                break;
            }
            last_sound_time -= data.len;
            data = getSoundData(--pointer);
        }
    } else if (mp_command == skip) {
        uint16_t t = SKIP_TIME;
        while (t > last_sound_time) {
            t -= last_sound_time;
            pointer++;
            if (pointer == SCORE_LENGTH) {
                init_music = true;
                t = 0;
                last_sound_time = 0;
                break;
            }
            data = getSoundData(pointer);
            last_sound_time = data.len;
        }
        last_sound_time -= t;
    }

    if (mp_mode == Playing) {
        if (last_sound_time == 0) {
            if (!init_music) {
                pointer++;
            } else {
                pointer = 0;
                init_music = false;
            }
            if (pointer == SCORE_LENGTH) {
                pointer = 0;
            }
            data = getSoundData(pointer);
            last_sound_time = data.len;
        }

        if (last_sound_time < CUT_TIME && (data.flag & CUT) != 0) {
            stopSound();
        } else {
            setSound(data.freq, data.flag);
        }

        last_sound_time--;
    } else {
        stopSound();
    }
    mp_command = none;
}
