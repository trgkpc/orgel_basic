#pragma once
#include <avr/io.h>
#include <stdbool.h>
struct Switch {
    bool status;
    uint16_t pushed_time;
    bool toggle;
    bool fall_edge;
    uint8_t lock_time;
};

extern struct Switch red_switch;
extern struct Switch green_switch;
extern struct Switch blue_switch;

void updateSwitch();
void initSwitch();
