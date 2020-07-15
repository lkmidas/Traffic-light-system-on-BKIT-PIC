#include "timer.h"

unsigned int Timer0_value = 0, Timer1_value = 0, Timer2_value = 0, Timer3_value = 0;
unsigned char Timer0_flag = 0, Timer1_flag = 0, Timer2_flag = 0, Timer3_flag = 0;
unsigned int v_cnt0 = 0, v_cnt1 = 0;
unsigned int Timer0_cnt = 0, Timer1_cnt = 0, Timer2_cnt = 0, Timer3_cnt = 0;
unsigned int Timer0_MUL = 1, Timer1_MUL = 1, Timer2_MUL = 1, Timer3_MUL = 1;

/* TIMER0 */
void Timer0_init(unsigned int count){
	T0CONbits.TMR0ON = 0;    // turn off Timer0
	Timer0_value = count;
	T0CONbits.T08BIT = 0;    // set mode 16bit Timer0
	TMR0L = (65535 - Timer0_value) % 256;    // count Timer0_value times
	TMR0H = (65535 - Timer0_value) / 256;
	T0CONbits.T0CS = 0;      // select internal clock include crystal
	T0CONbits.PSA = 1;       // no prescaler
	INTCONbits.T0IE = 1;     // enable Timer0 interrupt
	INTCONbits.T0IF = 0;     // clear interrupt Timer0 flag
	INTCON2bits.TMR0IP = 1;	 // Timer0 interrupt high priority 	
	T0CONbits.TMR0ON = 1;    // start Timer0
}

void Timer0_stop(void){
	T0CONbits.TMR0ON = 0;	
}

void Timer0_start(void){
	
	TMR0L = (65535 - Timer0_value) % 256;    // count 50000 times
	TMR0H = (65535 - Timer0_value) / 256;
	T0CONbits.TMR0ON = 1;
}

/* TIMER1 */
void Timer1_init(unsigned int count){
	T1CONbits.TMR1ON = 0;    // turn off Timer1
	Timer1_value = count;
	TMR1H = (65535 - Timer1_value) / 256;    // count Timer1_value times
	TMR1L = (65535 - Timer1_value) % 256;
	PIE1bits.TMR1IE = 1;    // enable Timer1 interrupt
	PIR1bits.TMR1IF = 0;    // clear Timer1 interrupt flag
	IPR1bits.TMR1IP = 0;    // Timer1 interrupt low priority 	
	//T1CON
	T1CONbits.RD16 = 1;	    // select Timer1 16 bits mode
	T1CONbits.T1RUN = 1; 
	T1CONbits.T1CKPS0 = 0;
	T1CONbits.T1CKPS1 = 0;  // prescaler 1:1
	T1CONbits.T1OSCEN = 0;  // oscillator invert and feedback for external oscillator
	//T1CONbits.T1SYNC = 0; // this bit ignore when TMR1CS = 0
	T1CONbits.TMR1CS = 0;   // select internal clock source
	T1CONbits.TMR1ON = 1;   // start Timer1
}

void Timer1_start(void){
	TMR1H = (65535 - Timer1_value) / 256;
	TMR1L = (65535 - Timer1_value) % 256;
	T1CONbits.TMR1ON = 1;
}

void Timer1_stop(void){
	T1CONbits.TMR1ON = 0;
}

/* TIMER2 */
void Timer2_init(unsigned char count){
	T2CONbits.TMR2ON = 0;    // turn off Timer2
	TMR2 = 0x00;
	Timer2_value = count;    // count Timer2_value times
	PR2 = Timer2_value;
	PIE1bits.TMR2IE = 1;     // enable Timer2 interrupt
	PIR1bits.TMR2IF = 0;     // clear interrupt Timer2 flag

	IPR1bits.TMR2IP = 0;     // Timer2 interrupt low priority	

	T2CONbits.T2CKPS0 = 0;
	T2CONbits.T2CKPS1 = 0;   // prescaler 1:1
	T2CONbits.TMR2ON = 1;    // start Timer2
}

void Timer2_start(void){
	PR2 = Timer2_value;
	T2CONbits.TMR2ON = 1;
}

void Timer2_stop(void){
	T2CONbits.TMR2ON = 0;
}

/* TIMER3 */
void Timer3_init(unsigned int count){
	T3CONbits.TMR3ON = 0;    // turn off Timer3
	Timer3_value = count;
	TMR3H = (65535 - Timer3_value) / 256;    // count Timer3_value times
	TMR3L = (65535 - Timer3_value) % 256;
	PIE2bits.TMR3IE = 1;    // enable Timer3 interrupt
	PIR2bits.TMR3IF = 0;    // clear Timer3 interrupt flag

	IPR2bits.TMR3IP = 0;    // Timer3 interrupt low priority 	

	//T3CON
	T3CONbits.RD16 = 1;     // select Timer3 16 bits mode
	T3CONbits.T3CKPS0 = 0;
	T3CONbits.T3CKPS1 = 0;  // prescaler 1:1
	//T3CONbits.T3SYNC = 0; // this bit ignore when TMR3CS = 0
	T3CONbits.TMR3CS = 0;   // select internal clock source
	T3CONbits.TMR3ON = 1;   // start Timer3
}

void Timer3_start(void){
	TMR3L = (65535 - Timer3_value) % 256;
	TMR3H = (65535 - Timer3_value) / 256;
	T3CONbits.TMR3ON = 1;
}

void Timer3_stop(void){
	T3CONbits.TMR3ON = 0;
}

// Helper functions for timer interrupt
void Timer0_isr(void){
	Timer0_cnt ++;
	if (Timer0_cnt >= Timer0_MUL){
		Timer0_cnt = 0;
		Timer0_flag = 1;
        // Call high speed and high priority functions
	}
}

void Timer1_isr(void){
	Timer1_cnt ++;
	if (Timer1_cnt >= Timer1_MUL){
		Timer1_cnt = 0;
		Timer1_flag = 1;
	}
	// Call functions with high speed and higher priority than main
}

void Timer2_isr(void){
	Timer2_cnt ++;
	if (Timer2_cnt >= Timer2_MUL){
		Timer2_cnt = 0;
		Timer2_flag = 1;
	}
}

void Timer3_isr(void){
	Timer3_cnt ++;
	if (Timer3_cnt >= Timer3_MUL){
		Timer3_cnt = 0;
		Timer3_flag = 1;
	}
}

void Timer0_set(unsigned long ms){
	Timer0_MUL = ms;
	Timer0_cnt = 0;
	Timer0_flag = 0;
}

void Timer1_set(unsigned long ms){
	Timer1_MUL = ms / 5;
	Timer1_cnt = 0;
	Timer1_flag = 0;
}

void Timer3_set(unsigned long ms){
	Timer3_MUL = ms / 10;
	Timer3_cnt = 0;
	Timer3_flag = 0;
}

/* VIRTUAL TIMER */
