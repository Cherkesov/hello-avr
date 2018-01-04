#define F_CPU 16000000UL  // 1 MHz

// Тестовая программа для LCD, шина 8 бит
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


#include <inttypes.h>
#include <avr/pgmspace.h>


#define LCD_RS    0    //define MCU pin connected to LCD RS
#define LCD_RW    2    //define MCU pin connected to LCD R/W
#define LCD_E    1    //define MCU pin connected to LCD E
#define LCD_D0    0    //define MCU pin connected to LCD D0
#define LCD_D1    1    //define MCU pin connected to LCD D1
#define LCD_D2    2    //define MCU pin connected to LCD D1
#define LCD_D3    3    //define MCU pin connected to LCD D2
#define LCD_D4    4    //define MCU pin connected to LCD D3
#define LCD_D5    5    //define MCU pin connected to LCD D4
#define LCD_D6    6    //define MCU pin connected to LCD D5
#define LCD_D7    7    //define MCU pin connected to LCD D6
#define LDP PORTD    //define MCU port connected to LCD data pins
#define LCP PORTB    //define MCU port connected to LCD control pins
#define LDDR DDRD    //define MCU direction register for port connected to LCD data pins
#define LCDR DDRB    //define MCU direction register for port connected to LCD control pins


#define LCD_CLR             0    //DB0: clear display
#define LCD_HOME            1    //DB1: return to home position
#define LCD_ENTRY_MODE      2    //DB2: set entry mode
#define LCD_ENTRY_INC       1    //DB1: increment
#define LCD_ENTRY_SHIFT     0    //DB2: shift
#define LCD_ON_CTRL         3    //DB3: turn lcd/cursor on
#define LCD_ON_DISPLAY      2    //DB2: turn display on
#define LCD_ON_CURSOR       1    //DB1: turn cursor on
#define LCD_ON_BLINK        0    //DB0: blinking cursor
#define LCD_MOVE            4    //DB4: move cursor/display
#define LCD_MOVE_DISP       3    //DB3: move display (0-> move cursor)
#define LCD_MOVE_RIGHT      2    //DB2: move right (0-> left)
#define LCD_FUNCTION        5    //DB5: function set
#define LCD_FUNCTION_8BIT   4    //DB4: set 8BIT mode (0->4BIT mode)
#define LCD_FUNCTION_2LINES 3    //DB3: two lines (0->one line)
#define LCD_FUNCTION_10DOTS 2    //DB2: 5x10 font (0->5x7 font)
#define LCD_CGRAM           6    //DB6: set CG RAM address
#define LCD_DDRAM           7    //DB7: set DD RAM address
// reading:
#define LCD_BUSY            7    //DB7: LCD is busy
#define LCD_LINES            2    //visible lines
#define LCD_LINE_LENGTH        16    //line length (in characters)
// cursor position to DDRAM mapping
#define LCD_LINE0_DDRAMADDR        0x00
#define LCD_LINE1_DDRAMADDR        0x40
#define LCD_LINE2_DDRAMADDR        0x14
#define LCD_LINE3_DDRAMADDR        0x54
// progress bar defines
#define PROGRESSPIXELS_PER_CHAR    6

const uint8_t* LcdCustomChar = 0;


void LCDsendChar(uint8_t);        //forms data ready to send to 74HC164
void LCDsendCommand(uint8_t);    //forms data ready to send to 74HC164
void LCDinit(void);            //Initializes LCD
void LCDclr(void);                //Clears LCD
void LCDhome(void);            //LCD cursor home
void LCDstring(uint8_t *, uint8_t);    //Outputs string to LCD
void LCDGotoXY(uint8_t, uint8_t);    //Cursor to X Y position
void CopyStringtoLCD(const uint8_t *, uint8_t, uint8_t);//copies flash string to LCD at x,y
void LCDdefinechar(const uint8_t *, uint8_t);//write char to LCD CGRAM
void LCDshiftRight(uint8_t);    //shift by n characters Right
void LCDshiftLeft(uint8_t);    //shift by n characters Left
void LCDcursorOn(void);        //Underline cursor ON
void LCDcursorOnBlink(void);    //Underline blinking cursor ON
void LCDcursorOFF(void);        //Cursor OFF
void LCDblank(void);            //LCD blank but not cleared
void LCDvisible(void);            //LCD visible
void LCDcursorLeft(uint8_t);    //Shift cursor left by n
void LCDcursorRight(uint8_t);    //shif cursor right by n
// displays a horizontal progress bar at the current cursor location
// <progress> is the value the bargraph should indicate
// <maxprogress> is the value at the end of the bargraph
// <length> is the number of LCD characters that the bargraph should cover
//adapted from AVRLIB - displays progress only for 8 bit variables
void LCDprogressBar(uint8_t progress, uint8_t maxprogress, uint8_t length);


#define RS PC0 // RS подключаем к PC0 микроконтроллера
#define EN PC2 // EN подключаем к PC2 микроконтроллера

// Основная программа
int main(void) {
//    DDRC = 0xFF;
    PORTD = 0x00;
    LCDinit();

    while (1) {
//        LCD_CLR();
//        LCDsendCommand(LCD_CLR);
//        LCDGotoXY(0, 0);

        const uint8_t letterA = 'a';

        LCDsendChar(letterA);
        LCDsendChar(letterA);
        LCDsendChar(letterA);
        LCDsendChar(letterA);
        LCDsendChar(letterA);
        LCDsendChar(letterA);
        LCDsendChar(letterA);
        LCDsendChar(letterA);

        _delay_ms(500);
    }
}






//const uint8_t LcdCustomChar[] PROGMEM=//define 8 custom LCD chars
//        {
//                0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, // 0. 0/5 full progress block
//                0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00, // 1. 1/5 full progress block
//                0x00, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, // 2. 2/5 full progress block
//                0x00, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, 0x00, // 3. 3/5 full progress block
//                0x00, 0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x00, // 4. 4/5 full progress block
//                0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // 5. 5/5 full progress block
//                0x03, 0x07, 0x0F, 0x1F, 0x0F, 0x07, 0x03, 0x00, // 6. rewind arrow
//                0x18, 0x1C, 0x1E, 0x1F, 0x1E, 0x1C, 0x18, 0x00  // 7. fast-forward arrow
//        };


void LCDsendChar(uint8_t ch)        //Sends Char to LCD
{

#ifdef LCD_4bit
    //4 bit part
    LDP=(ch&0b11110000);
    LCP|=1<<LCD_RS;
    LCP|=1<<LCD_E;
    _delay_us(100);
    LCP&=~(1<<LCD_E);
    LCP&=~(1<<LCD_RS);
    _delay_us(100);
    LDP=((ch&0b00001111)<<4);
    LCP|=1<<LCD_RS;
    LCP|=1<<LCD_E;
    _delay_us(100);
    LCP&=~(1<<LCD_E);
    LCP&=~(1<<LCD_RS);
    _delay_us(100);
#else
    //8 bit part
    LDP = ch;
    LCP |= 1 << LCD_RS;
    LCP |= 1 << LCD_E;
    _delay_us(100);
    LCP &= ~(1 << LCD_E);
    LCP &= ~(1 << LCD_RS);
    _delay_us(100);
#endif
}

void LCDsendCommand(uint8_t cmd)    //Sends Command to LCD
{
#ifdef LCD_4bit
    //4 bit part
    LDP=(cmd&0b11110000);
    LCP|=1<<LCD_E;
    _delay_us(100);;
    LCP&=~(1<<LCD_E);
    _delay_us(100);
    LDP=((cmd&0b00001111)<<4);
    LCP|=1<<LCD_E;
    _delay_us(100);
    LCP&=~(1<<LCD_E);
    _delay_us(100);
#else
    //8 bit part
    LDP = cmd;
    LCP |= 1 << LCD_E;
    _delay_us(100);
    LCP &= ~(1 << LCD_E);
    _delay_us(100);
#endif
}

void LCDinit(void)//Initializes LCD
{
#ifdef LCD_4bit
    //4 bit part
    _delay_us(100);
    LDP=0x00;
    LCP=0x00;
    LDDR|=1<<LCD_D7|1<<LCD_D6|1<<LCD_D5|1<<LCD_D4;
    LCDR|=1<<LCD_E|1<<LCD_RW|1<<LCD_RS;
    //---------one------
    LDP=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|1<<LCD_D4; //4 bit mode
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_us(100);
    LCP&=~(1<<LCD_E);
    _delay_us(100);
    //-----------two-----------
    LDP=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|1<<LCD_D4; //4 bit mode
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_us(100);
    LCP&=~(1<<LCD_E);
    _delay_us(100);
    //-------three-------------
    LDP=0<<LCD_D7|0<<LCD_D6|1<<LCD_D5|0<<LCD_D4; //4 bit mode
    LCP|=1<<LCD_E|0<<LCD_RW|0<<LCD_RS;
    _delay_us(100);
    LCP&=~(1<<LCD_E);
    _delay_us(100);
    //--------4 bit--dual line---------------
    LCDsendCommand(0b00101000);
    //-----increment address, invisible cursor shift------
    LCDsendCommand(0b00001100);
    //init 8 custom chars
    uint8_t ch=0, chn=0;
    while(ch<64)
    {
        LCDdefinechar((LcdCustomChar+ch),chn++);
        ch=ch+8;
    }


#else
    //8 bit part
    _delay_us(100);
    LDP = 0x00;
    LCP = 0x00;
    LDDR |= 1 << LCD_D7 | 1 << LCD_D6 | 1 << LCD_D5 | 1 << LCD_D4 | 1 << LCD_D3
            | 1 << LCD_D2 | 1 << LCD_D1 | 1 << LCD_D0;
    LCDR |= 1 << LCD_E | 1 << LCD_RW | 1 << LCD_RS;
    //---------one------
    LDP = 0 << LCD_D7 | 0 << LCD_D6 | 1 << LCD_D5 | 1 << LCD_D4 | 0 << LCD_D3
          | 0 << LCD_D2 | 0 << LCD_D1 | 0 << LCD_D0; //8 it mode
    LCP |= 1 << LCD_E | 0 << LCD_RW | 0 << LCD_RS;
    _delay_us(100);
    LCP &= ~(1 << LCD_E);
    _delay_us(100);
    //-----------two-----------
    LDP = 0 << LCD_D7 | 0 << LCD_D6 | 1 << LCD_D5 | 1 << LCD_D4 | 0 << LCD_D3
          | 0 << LCD_D2 | 0 << LCD_D1 | 0 << LCD_D0; //8 it mode
    LCP |= 1 << LCD_E | 0 << LCD_RW | 0 << LCD_RS;
    _delay_us(100);
    LCP &= ~(1 << LCD_E);
    _delay_us(100);
    //-------three-------------
    LDP = 0 << LCD_D7 | 0 << LCD_D6 | 1 << LCD_D5 | 1 << LCD_D4 | 0 << LCD_D3
          | 0 << LCD_D2 | 0 << LCD_D1 | 0 << LCD_D0; //8 it mode
    LCP |= 1 << LCD_E | 0 << LCD_RW | 0 << LCD_RS;
    _delay_us(100);
    LCP &= ~(1 << LCD_E);
    _delay_us(100);
    //--------8 bit dual line----------
    LDP = 0 << LCD_D7 | 0 << LCD_D6 | 1 << LCD_D5 | 1 << LCD_D4 | 1 << LCD_D3
          | 0 << LCD_D2 | 0 << LCD_D1 | 0 << LCD_D0; //8 it mode
    LCP |= 1 << LCD_E | 0 << LCD_RW | 0 << LCD_RS;
    _delay_us(100);
    LCP &= ~(1 << LCD_E);
    _delay_us(100);
    //-----increment address, invisible cursor shift------
    LDP = 0 << LCD_D7 | 0 << LCD_D6 | 0 << LCD_D5 | 0 << LCD_D4 | 1 << LCD_D3
          | 1 << LCD_D2 | 0 << LCD_D1 | 0 << LCD_D0; //8 it mode
    LCP |= 1 << LCD_E | 0 << LCD_RW | 0 << LCD_RS;
    _delay_us(100);
    LCP &= ~(1 << LCD_E);
    _delay_ms(5);
    //init custom chars
    uint8_t ch = 0, chn = 0;
    while (ch < 64) {
        LCDdefinechar((LcdCustomChar + ch), chn++);
        ch = ch + 8;
    }

#endif
}

void LCDclr(void)                //Clears LCD
{
    LCDsendCommand(1 << LCD_CLR);
}

void LCDhome(void)            //LCD cursor home
{
    LCDsendCommand(1 << LCD_HOME);
}

void LCDstring(uint8_t *data, uint8_t nBytes)    //Outputs string to LCD
{
    register uint8_t i;

    // check to make sure we have a good pointer
    if (!data) return;

    // print data
    for (i = 0; i < nBytes; i++) {
        LCDsendChar(data[i]);
    }
}

void LCDGotoXY(uint8_t x, uint8_t y)    //Cursor to X Y position
{
    register uint8_t DDRAMAddr;
    // remap lines into proper order
    switch (y) {
        case 0:
            DDRAMAddr = LCD_LINE0_DDRAMADDR + x;
            break;
        case 1:
            DDRAMAddr = LCD_LINE1_DDRAMADDR + x;
            break;
        case 2:
            DDRAMAddr = LCD_LINE2_DDRAMADDR + x;
            break;
        case 3:
            DDRAMAddr = LCD_LINE3_DDRAMADDR + x;
            break;
        default:
            DDRAMAddr = LCD_LINE0_DDRAMADDR + x;
    }
    // set data address
    LCDsendCommand(1 << LCD_DDRAM | DDRAMAddr);

}

//Copies string from flash memory to LCD at x y position
//const uint8_t welcomeln1[] PROGMEM="AVR LCD DEMO\0";
//CopyStringtoLCD(welcomeln1, 3, 1);
void CopyStringtoLCD(const uint8_t *FlashLoc, uint8_t x, uint8_t y) {
    uint8_t i;
    LCDGotoXY(x, y);
    for (i = 0; (uint8_t) pgm_read_byte(&FlashLoc[i]); i++) {
        LCDsendChar((uint8_t) pgm_read_byte(&FlashLoc[i]));
    }
}
//defines char symbol in CGRAM
/*
const uint8_t backslash[] PROGMEM=
{
0b00000000,//back slash
0b00010000,
0b00001000,
0b00000100,
0b00000010,
0b00000001,
0b00000000,
0b00000000
};
LCDdefinechar(backslash,0);
*/
void LCDdefinechar(const uint8_t *pc, uint8_t char_code) {
    uint8_t a, pcc;
    uint16_t i;
    a = (char_code << 3) | 0x40;
    for (i = 0; i < 8; i++) {
        pcc = pgm_read_byte(&pc[i]);
        LCDsendCommand(a++);
        LCDsendChar(pcc);
    }
}

void LCDshiftLeft(uint8_t n)    //Scrol n of characters Right
{
    for (uint8_t i = 0; i < n; i++) {
        LCDsendCommand(0x1E);
    }
}

void LCDshiftRight(uint8_t n)    //Scrol n of characters Left
{
    for (uint8_t i = 0; i < n; i++) {
        LCDsendCommand(0x18);
    }
}

void LCDcursorOn(void) //displays LCD cursor
{
    LCDsendCommand(0x0E);
}

void LCDcursorOnBlink(void)    //displays LCD blinking cursor
{
    LCDsendCommand(0x0F);
}

void LCDcursorOFF(void)    //turns OFF cursor
{
    LCDsendCommand(0x0C);
}

void LCDblank(void)        //blanks LCD
{
    LCDsendCommand(0x08);
}

void LCDvisible(void)        //Shows LCD
{
    LCDsendCommand(0x0C);
}

void LCDcursorLeft(uint8_t n)    //Moves cursor by n poisitions left
{
    for (uint8_t i = 0; i < n; i++) {
        LCDsendCommand(0x10);
    }
}

void LCDcursorRight(uint8_t n)    //Moves cursor by n poisitions left
{
    for (uint8_t i = 0; i < n; i++) {
        LCDsendCommand(0x14);
    }
}

//adapted fro mAVRLIB
void LCDprogressBar(uint8_t progress, uint8_t maxprogress, uint8_t length) {
    uint8_t i;
    uint16_t pixelprogress;
    uint8_t c;

    // draw a progress bar displaying (progress / maxprogress)
    // starting from the current cursor position
    // with a total length of "length" characters
    // ***note, LCD chars 0-5 must be programmed as the bar characters
    // char 0 = empty ... char 5 = full

    // total pixel length of bargraph equals length*PROGRESSPIXELS_PER_CHAR;
    // pixel length of bar itself is
    pixelprogress = ((progress * (length * PROGRESSPIXELS_PER_CHAR)) / maxprogress);

    // print exactly "length" characters
    for (i = 0; i < length; i++) {
        // check if this is a full block, or partial or empty
        // (u16) cast is needed to avoid sign comparison warning
        if (((i * (uint16_t) PROGRESSPIXELS_PER_CHAR) + 5) > pixelprogress) {
            // this is a partial or empty block
            if (((i * (uint16_t) PROGRESSPIXELS_PER_CHAR)) > pixelprogress) {
                // this is an empty block
                // use space character?
                c = 0;
            } else {
                // this is a partial block
                c = pixelprogress % PROGRESSPIXELS_PER_CHAR;
            }
        } else {
            // this is a full block
            c = 5;
        }

        // write character to display
        LCDsendChar(c);
    }

}