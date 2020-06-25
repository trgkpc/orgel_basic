#include "buzzer.h"

void stop_buzzer()
{
    TCCR1A = 0;
    TCCR1B = 0;
    PORTB &= ~(1 << 1);
}

void set_buzzer(uint16_t freq, uint8_t flags)
{
    if (freq == 0) {
        stop_buzzer();
    } else {
        TCCR1A = 0x82;
        TCCR1B = 0b11001;
        if ((flags & CLOCK_DIVISION8) != 0) {
            TCCR1B = 0b11010;
        }
        ICR1 = freq;
        OCR1A = freq / 4;
    }
}


void initBuzzer()
{
    stop_buzzer();
}
