#define F_CPU 1000000UL  // 1 MHz

#include <avr/io.h>
#include <avr/delay.h>

#define FRAME_TIME 100;

// engine TL - ^ PD0 v PD1;
// engine TR - ^ PD2 v PD3;
// engine BR - ^ PD4 v PD5;
// engine BL - ^ PD6 v PD7;

void move_reset(void) {
    PORTD = 0x00;
}

void move_forward(void) {
    PORTD = 0x00;
    PORTD |= (1 << PD0);
    PORTD |= (1 << PD2);
//    PORTD |= (1 << PD4);
//    PORTD |= (1 << PD6);
}

void move_backward(void) {
    PORTD = 0x00;
//    PORTD |= (1 << PD1);
//    PORTD |= (1 << PD3);
    PORTD |= (1 << PD5);
    PORTD |= (1 << PD7);
}

void rotate_left(void) {
    PORTD = 0x00;

    // right side move forward
    PORTD |= (1 << PD2);
    PORTD |= (1 << PD4);

    // left side move backward
    PORTD |= (1 << PD1);
    PORTD |= (1 << PD7);
}

void rotate_right(void) {
    PORTD = 0x00;

    // right side move forward
    PORTD |= (1 << PD3);
    PORTD |= (1 << PD5);

    // left side move backward
    PORTD |= (1 << PD0);
    PORTD |= (1 << PD6);
}

int main(void) {
    int delay = 1000;

    DDRC = 0x00; // set all C-ports as enters
    PORTC = 0b11111111;
    DDRD = 0xFF; // set all D-ports as exists
    PORTD = 0xFF;

    while (1) {
        PORTD = 0x00;

        if ((PINC & (1 << PC0)) == 0) {
            move_forward();
        }

        _delay_ms(delay);
    }

}
