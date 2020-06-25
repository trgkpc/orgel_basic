#define F_CPU 20000000
#include "firmware/init.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "music/player.h"

// bpm:143
//  -> 60 * 1e3 / (4*143)

#define CUT_TIME 20
#define BPM 105
#define BPM_MINUS_CUTTIME 85

volatile uint16_t last_sound_time = 0;

void b(uint16_t freq, uint8_t len, uint8_t flags)
{
    toggleLed(GreenLed);
    set_buzzer(freq, flags);

    last_sound_time = ((uint16_t)BPM) * ((uint16_t)len);
}


int main(void)
{
    initFirmware();
    initMusicPlayer();

    sei();

    commandMusicPlayer(start);
    setLed(GreenLed, true);
    flashLed(GreenLed, true);

    set_sleep_mode(SLEEP_MODE_IDLE);
    for (;;) {
        sleep_mode();
    }
    return 0;
}

void cycleCallback()
{
    // input
    firmwareCallback();

    if (green_switch.toggle) {
        commandMusicPlayer(back);
    }
    if (red_switch.toggle) {
        commandMusicPlayer(start_stop);
        toggleLed(RedLed);
    }
    if (blue_switch.toggle) {
        commandMusicPlayer(skip);
    }

    // outpute
    musicPlayerCallback();
}
