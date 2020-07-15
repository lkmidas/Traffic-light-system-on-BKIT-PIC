#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <p18f4620.h>

#define PORT_BUTTON PORTC
#define TRIS_BUTTON TRISC
#define	MAX_COL 4
#define MAX_ROW 4

extern unsigned int keyCode[];


void KeyMatrix_init(void);
void Button_init(void);
void KeyMatrix_scan(void);
void Button_scan(void);
void Button_process(void);
void Button_delay(int ms);
void scan_key_matrix_demo(void);

#endif