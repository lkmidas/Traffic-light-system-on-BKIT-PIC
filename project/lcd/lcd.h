#ifndef _LCD_H_
#define _LCD_H_

#include <p18f4620.h>

#define LCD_RS 2    // define MCU pin connected to LCD RS
//#define LCD_RW    // define MCU pin connected to LCD R/W
#define LCD_E  3    // define MCU pin connected to LCD E
#define LCD_D4 4    // define MCU pin connected to LCD D3
#define LCD_D5 5    // define MCU pin connected to LCD D4
#define LCD_D6 6    // define MCU pin connected to LCD D5
#define LCD_D7 7    // define MCU pin connected to LCD D6

#define LCD_LINE0		0x00
#define LCD_LINE1		0x40

#define CLR   0
#define SET   1
#define DAT   1    // data mode
#define CMD   0    // command mode
#define READ  1    // read mode
#define WRITE 0    // write mode

#define LCD_PORT PORTB
#define LCD_DDR  TRISB

// For 4 high bits
#define RS(x) ((x) ? (LCD_PORT |= (1 << LCD_RS)) : (LCD_PORT &= ~(1 << LCD_RS)))
#define EN(x) ((x) ? (LCD_PORT |= (1 << LCD_E)) : (LCD_PORT &= ~(1 << LCD_E)))

#define LCD_DATA_OUT(x) LCD_PORT &= 0x0F; LCD_PORT |= (x);
#define LCD_DATA_IN     LCD_PORT & 0x0F;
#define LCD_DIR_IN      LCD_PORT |= 0x0F;
// Internal methods
void LCD_delay(unsigned char time);
void LCD_sendCmd(unsigned char cmd);
void LCD_init(void);
void LCD_write4Bits(unsigned char dat);
void LCD_writeCmd(unsigned char cmd);
void LCD_writeData(unsigned char data);
// Asynchronous methods
void LCD_printChar(char c);
void LCD_setCursor(unsigned char row, unsigned char column);
void LCD_clear(void);
void LCD_printNum(unsigned char x, unsigned char y, long num);
void LCD_printStr(unsigned char x, unsigned char y, const rom unsigned char* string);
// Sychronous methods
void LCD_writeCharSync(unsigned char c);
void LCD_printCharSync(unsigned char x, unsigned char y,unsigned char c);
void LCD_clearSync(void);
void LCD_printNumSync(unsigned char x, unsigned char y, long num);
void LCD_printStrSync(unsigned char x, unsigned char y, const rom unsigned char* string);
// Display methods
void LCD_display(void);
void LCD_display2(void);
#endif
