#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <p18f4620.h>

void Interrupt_init(void);
void ExtInt0_init(void);
void lowIsr(void);
void highIsr(void);


#endif