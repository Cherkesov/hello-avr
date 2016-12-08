#define F_CPU 1000000UL  // 1 MHz
#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{
    int period = 5;

    period = period * 1000;

    DDRD = 0xff;
    while (1) {
        PORTD = 0xff;
        _delay_ms(200);
        PORTD = 0x00;
        _delay_ms(period);
    }
}