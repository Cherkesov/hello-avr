#define F_CPU 1000000UL  // 1 MHz

#define LCD_PORT PORTA
#define LCD_DDR DDRA
#define LCD_PIN PINA

#define DATA_BUS 0XF0

#define RS 0
#define RW 1
#define E  2


#include <avr/io.h>
#include <avr/delay.h>

uint8_t Busy_Flag = 0;

void Check_Busy(void) {
    //шина данных - вход с подтяжкой
    LCD_DDR &= ~DATA_BUS;
    LCD_PORT |= DATA_BUS;

    //будем читать из lcd
    LCD_PORT |= (1 << RW);

    //команды
    LCD_PORT &= ~(1 << RS);

    do {
        LCD_PORT |= (1 << E);
        _delay_us(2);
        Busy_Flag = (LCD_PIN & 0X80);
        LCD_PORT &= ~(1 << E);

        _delay_us(1);

        LCD_PORT |= (1 << E);
        _delay_us(2);
        LCD_PORT &= ~(1 << E);
    }
    while (Busy_Flag);

    //шина данных - выход
    LCD_DDR |= DATA_BUS;
    //переводим LCD в режим записи
    LCD_PORT &= ~(1 << RW);
}

void Strob(void) {
    LCD_PORT |= (1 << E);
    _delay_us(2);
    LCD_PORT &= ~(1 << E);
}

void Common_Write_Func(uint8_t data) {
    uint8_t temp = (data & 0XF0);

    LCD_PORT &= ~(1 << RW);
    LCD_DDR |= DATA_BUS;

    //затираем прошлую отправку нулями
    LCD_PORT &= ~DATA_BUS;
    LCD_PORT |= temp;
    Strob();

    //затираем прошлую отправку нулями
    LCD_PORT &= ~DATA_BUS;
    LCD_PORT |= (data << 4);
    Strob();
}

void Write_Command(uint8_t data) {
    Check_Busy();
    LCD_PORT &= ~(1 << RS);
    Common_Write_Func(data);
}

void Write_Data(uint8_t data) {
    Check_Busy();
    LCD_PORT |= (1 << RS);
    Common_Write_Func(data);
}

void Write_Init_Command(uint8_t data) {
    //ножки по которым передаются команды/данные  на выход
    LCD_DDR |= DATA_BUS;
    //будем слать команду
    LCD_PORT &= ~(1 << RS);
    LCD_PORT &= ~(1 << RW);

    //затираем прошлую отправку нулями
    LCD_PORT &= ~DATA_BUS;
    //выводим команду в шину
    LCD_PORT |= data;
    Strob();

    _delay_us(100);
}

void LCD_Init(void) {
    _delay_ms(50);

    Write_Init_Command(0x20);
    Write_Init_Command(0x20);
    //включаем дисплей, в режиме 2-х линий
    Write_Init_Command(0xC0);

    _delay_us(50);

    Write_Init_Command(0x00);
    //включаем отображение курсора
    Write_Init_Command(0xE0);

    _delay_us(50);

    Write_Init_Command(0x00);
    //очищаем дисплей
    Write_Init_Command(0x10);

    _delay_ms(2);

    Write_Init_Command(0x00);
    //значение DDRAM увеличивается, без сдвига экрана
    Write_Init_Command(0x60);
}

__inline void LCD_Clear(void) {
    Write_Command(0X01);
}

void Draw_String(const char *str) {
    uint8_t data = 0;
    while (*str) {
        data = *str++;
        Write_Data(data);
    }
}


__inline void Set_Ddram_Address(uint8_t address) {
    Write_Command(address | 0x80);
}


int main(void) {
    LCD_DDR = 0XFF;
    LCD_Init();

    Set_Ddram_Address(0X43);
    Draw_String("Hello, ");
    Draw_String("World");

    while (1) {
        //
    }
}