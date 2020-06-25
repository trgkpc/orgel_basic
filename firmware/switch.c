#include "switch.h"
#include <avr/io.h>
#define SWITCH_THRESH_TIME 5
#define SWITCH_MAX_PUSHED_TIME 2000

struct Switch red_switch;
struct Switch green_switch;
struct Switch blue_switch;

void updateEachSwitch(struct Switch* sw, bool raw_data)
{
    bool read_data = !(raw_data);
    sw->toggle = false;
    sw->fall_edge = false;
    if (sw->lock_time > 0) {
        (sw->lock_time)--;
    } else {
        if ((sw->status) ^ read_data) {
            sw->toggle = read_data;
            sw->fall_edge = (!read_data);
            sw->lock_time = SWITCH_THRESH_TIME;
        }
        sw->status = read_data;
    }

    if (sw->status) {
        if (sw->pushed_time < SWITCH_MAX_PUSHED_TIME) {
            sw->pushed_time++;
        }
    } else {
        sw->pushed_time = 0;
    }
}

void updateSwitch()
{
    updateEachSwitch(&blue_switch, (PINC & 8));
    updateEachSwitch(&green_switch, (PINC & 2));
    updateEachSwitch(&red_switch, (PINC & 1));
}

struct Switch constructSwitch()
{
    struct Switch sw = {false, 0, false, false, 0};
    return sw;
}

void initSwitch()
{
    red_switch = constructSwitch();
    green_switch = constructSwitch();
    blue_switch = constructSwitch();
    updateSwitch();
}
