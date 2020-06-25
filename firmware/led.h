#pragma once
#include <stdbool.h>
enum LED {
    RedLed,
    GreenLed,
};

void setLed(enum LED led, bool onoff);
void toggleLed(enum LED led);
void flashLed(enum LED led, bool fast);

void updateLed();
void initLed();
