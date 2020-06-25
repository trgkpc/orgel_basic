#pragma once
#include <avr/io.h>

#define CLOCK_DIVISION8 2

void stop_buzzer();
void set_buzzer(uint16_t freq, uint8_t flags);

void initBuzzer();
