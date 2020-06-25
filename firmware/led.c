#include "led.h"
#include <avr/io.h>
#define LEDCBKCycle 5
#define LEDFlashTime 84
#define LEDFastFlashTime 42

enum LEDControlMode {
    LEDManualControl,
    LEDFlash,
    LEDFastFlash,
};

struct LEDControlData {
    enum LEDControlMode mode;
    uint8_t timer;
};

struct LEDControlData red_led_data;
struct LEDControlData green_led_data;

void setLed(enum LED led, bool onoff)
{
    if (led == RedLed) {
        PORTD = (PORTD & 0xfe) + ((uint8_t)onoff);
        red_led_data.mode = LEDManualControl;
        red_led_data.timer = 0;
    } else if (led == GreenLed) {
        PORTD = (PORTD & 0xfd) + ((uint8_t)onoff) * 2;
        green_led_data.mode = LEDManualControl;
        green_led_data.timer = 0;
    }
}

void toggleLed(enum LED led)
{
    if (led == RedLed) {
        PIND = 1;
    } else if (led == GreenLed) {
        PIND = 2;
    }
}

void flashLed(enum LED led, bool fast)
{
    enum LEDControlMode mode = fast ? LEDFastFlash : LEDFlash;
    if (led == RedLed) {
        red_led_data.mode = mode;
        red_led_data.timer = 0;
    } else if (led == GreenLed) {
        green_led_data.mode = mode;
        green_led_data.timer = 0;
    }
}


// callback
void updateEachLed(enum LED led, struct LEDControlData* data)
{
    if (data->mode == LEDManualControl) {
        return;
    } else if (data->timer == 0) {
        toggleLed(led);
        data->timer = (data->mode == LEDFlash) ? LEDFlashTime : LEDFastFlashTime;
    } else {
        (data->timer)--;
    }
}

void updateLed()
{
    static uint8_t led_cbk_counter = 0;
    if (led_cbk_counter >= LEDCBKCycle) {
        updateEachLed(RedLed, &red_led_data);
        updateEachLed(GreenLed, &green_led_data);
        led_cbk_counter = 0;
    }
    led_cbk_counter++;
}

// init
void initLed()
{
    setLed(RedLed, false);
    setLed(GreenLed, false);
}
