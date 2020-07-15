#include "lcd.h"

#define LCD_SCREEN_0 0
#define LCD_SCREEN_1 1
#define LCD_SCREEN_2 2
#define LCD_SCREEN_3 3

int currentRow, currentCol;
char LCD_status = LCD_SCREEN_0;
unsigned char LCD_screen[2][16] = {
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};

void LCD_delay(unsigned char time){
    int i, j;
    for(i = 0; i < time; i++){
        for(j = 0; j < 1; j++){
        }
    }
}

void v_timer0(unsigned long time){
    int i, j;
    for(i = 0; i < time; i++){
        for(j = 0; j < 160; j++){
        }
    }
}

void LCD_sendCmd(unsigned char cmd){
    // Send command to LCD
	LCD_PORT &= 0x0f;
	LCD_PORT |= (cmd & 0xf0);
	LCD_PORT |= (1 << LCD_E);
	//LCD_delay(1);
	v_timer0(1);

	LCD_PORT &= ~(1 << LCD_E);
	v_timer0(1);

	LCD_PORT &= 0x0f;
	LCD_PORT |= (cmd << 4);
	LCD_PORT |= (1 << LCD_E);
	v_timer0(1);

	LCD_PORT &= ~(1 << LCD_E);
	v_timer0(1);
}

void LCD_init(void){
    unsigned char cmd;
    v_timer0(25); // Wait for more than 15ms after Vcc = 4.5V
    LCD_PORT&=0x0f;
	LCD_PORT&=0xf8;
    // One
    LCD_DATA_OUT(0x03 << 4)
	EN(SET);
    v_timer0(1);
	EN(CLR);
    v_timer0(1);
    // Two
    LCD_DATA_OUT(0x03 << 4)
	EN(SET);
    v_timer0(1);
	EN(CLR);
    v_timer0(1);
    // Three
    LCD_DATA_OUT(0x03 << 4)
	EN(SET);
    v_timer0(1);
	EN(CLR);
    v_timer0(1);
    // Four
    LCD_DATA_OUT(0x02 << 4)
	EN(SET);
    v_timer0(1);
	EN(CLR);
    v_timer0(1);
    // 4 bit-dual line
	LCD_sendCmd(0x28);
    // Increment address, invisible cursor shift
	LCD_sendCmd(0x0c);	
}

void LCD_write4Bits(unsigned char dat){
	//RW(WRITE);
    LCD_DATA_OUT(dat);
	EN(SET);
    LCD_delay(1);
	EN(CLR);
    LCD_delay(1);
}

void LCD_writeCmd(unsigned char cmd){
	RS(CMD);
    LCD_delay(1);
	LCD_write4Bits(cmd);
	LCD_write4Bits(cmd << 4);
}

void LCD_writeData(unsigned char data){
	RS(DAT);
    LCD_delay(1);
	LCD_write4Bits(data);
    LCD_write4Bits(data << 4);
    RS(CMD);
}

void LCD_printChar(char c){
    LCD_writeData(c);
}

unsigned long LCD_powerOf(int A, int x){
    char i;
    unsigned long temp = 1;
    for(i = 0; i < x; i++){
        temp *= A;
    }
    return temp;
}

/* Asynchronous LCD methods */
void LCD_setCursor(unsigned char row, unsigned char col){
    unsigned char address;
    switch (row){
        case 0:
            address = 0x80;    // Starting address of 1st line
            break;
        case 1:
            address = 0xC0;    // Starting address of 2nd line
            break;
    }
    address = address + col;
    LCD_writeCmd(address);    // Set DDRAM address counter to 0											  
}

void LCD_clear(){
    LCD_writeCmd(0x01);    // Display clear
    LCD_setCursor(0, 0);
}

void LCD_printNum(unsigned char x, unsigned char y, long num){
    char i;
    char numFlag = 0;
    LCD_setCursor(x, y);
    if (num == 0){
        LCD_printChar('0');
        return;
    }
    if (num < 0){
        LCD_printChar('-');
        num *= -1;
    }
    for (i = 10; i > 0; i--){
        if ((num / LCD_powerOf(10, i - 1)) != 0){
            numFlag = 1;
            LCD_printChar(num / LCD_powerOf(10, i - 1) + '0');
        } else {
            if (numFlag != 0){
                LCD_printChar('0');
            }
        }
        num %= LCD_powerOf(10, i - 1);
    }
}

void LCD_printStr(unsigned char x, unsigned char y, const rom unsigned char* string){
	LCD_setCursor(x, y);
	while (*string){
        LCD_printChar(*string++);
    }
}

/* Helper synchronous method */
void LCD_writeCharSync(unsigned char c){
    LCD_screen[currentRow][currentCol] = c;
    currentCol++;
}

/* Synchronous LCD methods */
void LCD_clearSync(){
    char i;
    for (i = 0; i < 16; i++){
        LCD_screen[0][i] = ' ';
        LCD_screen[1][i] = ' ';
    }
}

void LCD_printCharSync(unsigned char x, unsigned char y, unsigned char c){
    currentRow = x % 2;
    currentCol = y % 16;
    LCD_screen[currentRow][currentCol] = c;
}

void LCD_printNumSync(unsigned char x, unsigned char y, long num){
    char numFlag = 0;
    char i;
    currentRow = x % 2;
    currentCol = y % 16;
    if (num == 0){
        LCD_writeCharSync('0');
        return;
    }
    if (num < 0){
        LCD_writeCharSync('-');
        num *= -1;
    }
    //else
    //	LCD_writeCharSync(' ');
    for(i = 10; i > 0; i--){
        if ((num / LCD_powerOf(10, i - 1)) != 0){
            numFlag = 1;
            LCD_writeCharSync(num / LCD_powerOf(10, i - 1) + '0');
        } else {
            if(numFlag != 0){
                LCD_writeCharSync('0');
            }
        }
        num %= LCD_powerOf(10, i - 1);
    }
}

void LCD_printStrSync(unsigned char x, unsigned char y, const rom unsigned char *string){
    currentRow = x % 2;
    currentCol = y % 16;
    while (*string){
        LCD_writeCharSync(*string++);
    }
}

/* Display LCD synchronously methods */
void LCD_display(void){
    unsigned char i;
    LCD_setCursor(0,0);
    for (i = 0; i < 16; i++){
        LCD_printChar(LCD_screen[0][i]);
    }
    LCD_setCursor(1,0);
    for (i = 0; i < 16; i++){
        LCD_printChar(LCD_screen[1][i]);
    }
}

void LCD_display2(void){
    unsigned char i;
    switch (LCD_status){
        case LCD_SCREEN_0:
            LCD_setCursor(0, 0);
            for (i = 0; i < 16; i++){
                LCD_printChar(LCD_screen[0][i]);
            }
            LCD_status = LCD_SCREEN_1;
            break;
        case LCD_SCREEN_1:
            LCD_setCursor(1, 0);
            for (i = 0; i < 16; i++){
                LCD_printChar(LCD_screen[1][i]);
            }
            LCD_status = LCD_SCREEN_0;
            break;
        default:
            LCD_status = LCD_SCREEN_0;
            break;
    }
}