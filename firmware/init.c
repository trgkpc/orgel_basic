#include "init.h"
#include <avr/interrupt.h>
#include <avr/io.h>

void initFirmware()
{
    // ポート設定
    // ポートBはPORTB1のみ出力, それ以外は内蔵プルアップ
    DDRB = 2;
    PORTB = ~(2);
    // ポートCは0,1を入力に
    DDRC = 0;
    PORTC = 0xff;
    // ポートDは0,1を入力に
    DDRD = 0x3;
    PORTD = ~(2);

    initSwitch();
    initLed();
    initBuzzer();
    initTimer();
}

void firmwareCallback()
{
    updateSwitch();
    updateLed();
}
