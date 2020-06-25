#include "timer.h"
#include <avr/interrupt.h>
void initTimer()
{
    // 1cycle: 20,000clock
    TCCR0A = 0b00000010;
    TCCR0B = 0b00000100;
    TIMSK0 = 0b00000010;
    OCR0A = 78;
}

ISR(TIMER0_COMPA_vect)
{
    cycleCallback();
}
