#define F_CPU 1000000UL  // 1 MHz

#include <avr/io.h>
#include <avr/delay.h>

#define FRAME_TIME 100;

// engine TL - PD0;
// engine TR - PD1;
// engine BR - PD2;
// engine BL - PD3;

void move_reset() {
    PORTD &= ~((1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3));
}

void move_forward() {
    PORTD |= (1 << PD0) | (1 << PD1);
}

void move_backward() {
    PORTD |= (1 << PD2) | (1 << PD3);
}

int main(void) {
    int delay = 1000;

    DDRC = 0x00; // set all C-ports as enters
    DDRD = 0xff; // set all D-ports as exists

    PORTC = 0xff;

    while (1) {

        move_reset();

        /*if ((PINC & (1 << PINC1))) {
            move_forward();
        } else {
            move_backward();
        }*/

        if (
                ( PINC & ((1 << PINC0) | (1 << PINC1)) )
                ) {
            move_forward();
        }

        /*if (
                (PINC & (1 << PINC2))
                || (PINC & (1 << PINC3))
                ) {
            move_backward();
        }*/

        move_reset();
    }
}