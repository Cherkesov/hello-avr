#define F_CPU 1000000UL  // 1 MHz
#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{
    DDRD = 0xff;
    while (1) {
        PORTD = 0xff;
        _delay_ms(10);
        PORTD = 0x00;
        _delay_ms(2500);
    }
}