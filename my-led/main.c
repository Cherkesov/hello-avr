#define F_CPU 1000000UL  // 1 MHz
//#define F_CPU 8000000UL  // 8 MHz
//#define F_CPU 16000000UL  // 16 MHz
#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{
    int delay = 1000;
    DDRD = 0xff;
    while (1) {
        PORTD = 0xff;
        _delay_ms(delay);
        PORTD = 0x00;
        _delay_ms(delay);
    }
}