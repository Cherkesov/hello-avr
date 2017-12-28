#define F_CPU 1000000UL  // 1 MHz

#include <avr/io.h>
#include <avr/delay.h>

#define FRAME_TIME 100;

// engine TL - ^ PD0 v PD1;
// engine TR - ^ PD2 v PD3;
// engine BR - ^ PD4 v PD5;
// engine BL - ^ PD6 v PD7;

void move_reset() {
    PORTD = 0x00;
}

void move_forward() {
    PORTD = 0x00;
    PORTD |= (1 << PD0);
    PORTD |= (1 << PD2);
    PORTD |= (1 << PD4);
    PORTD |= (1 << PD6);
}

void move_backward() {
    PORTD = 0x00;
    PORTD |= (1 << PD1);
    PORTD |= (1 << PD3);
    PORTD |= (1 << PD5);
    PORTD |= (1 << PD7);
}

void rotate_left() {
    PORTD = 0x00;
    
    // right side move forward
    PORTD |= (1 << PD2);
    PORTD |= (1 << PD4);
    
    // left side move backward
    PORTD |= (1 << PD1);
    PORTD |= (1 << PD7);
}

void rotate_right() {
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
    DDRD = 0xff; // set all D-ports as exists

    PORTC = 0xff;

    while (1) {

        move_reset();

        // TODO: just do it!!
    }
}
