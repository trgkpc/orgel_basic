avr-gcc -g -O2 -mmcu=atmega168p music/*c firmware/*.c main.c 
avr-objcopy -j .text -j .data -O ihex a.out binary.hex
echo "avrdude -e -c avrisp2 -p m168p -U flash:w:binary.hex -F"

