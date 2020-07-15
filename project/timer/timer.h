#ifndef _TIMER_H_
#define _TIMER_H_

#include <p18f4620.h>

void Timer0_init(unsigned int count);
void Timer0_start(void);
void Timer0_stop(void);
void Timer1_init(unsigned int count);
void Timer1_start(void);
void Timer1_stop(void);
void Timer2_init(unsigned char count);
void Timer2_start(void);
void Timer2_stop(void);
void Timer3_init(unsigned int count);
void Timer3_start(void);
void Timer3_stop(void);

void Timer0_isr(void);
void Timer1_isr(void);
void Timer2_isr(void);
void Timer3_isr(void);

// High level functions for timers, called by user
void Timer0_set(unsigned long ms);
void Timer1_set(unsigned long ms);
void Timer3_set(unsigned long ms);
void v_Timer0(unsigned long ms);
void v_Timer1(unsigned long ms);

// Global variables declared by extern (without initial assignment)
extern unsigned char Timer0_flag, Timer1_flag, Timer2_flag, Timer3_flag;

#endif