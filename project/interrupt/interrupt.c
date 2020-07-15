#include "interrupt.h"
#include "..\timer\timer.h"


unsigned int cnt0 = 0,cnt1 = 0;

void Interrupt_init(void){
	INTCONbits.GIE = 1;  // Enable global interrupt
	RCONbits.IPEN = 1;   // Enable interrupt priority
	INTCONbits.PEIE = 1; // Peripheral External Interupt Enable
}

void ExtInt0_init(void){
	INTCON2bits.INTEDG0 = 0;  // Interrupt0 on falling edge
	INTCONbits.INT0IF = 0;    // Clear Interrupt Flag bit
	INTCONbits.INT0IE = 1;    // Enable INT0
}


#pragma code interruptVectorLow	= 0x18
void interruptVectorLow(void){
	_asm
		GOTO	lowIsr
	_endasm
}

#pragma code interruptVectorHigh = 0x08
void interruptVectorHigh(void){
	_asm
		GOTO	highIsr
	_endasm
}

#pragma code
#pragma interruptlow lowIsr
void lowIsr(void){
	if (INTCONbits.T0IF){ // Timer0
		Timer0_stop();
		INTCONbits.T0IF = 0;
		Timer0_start();
		Timer0_isr();
	}
	if (PIR1bits.TMR1IF){ // Timer1
		Timer1_stop();
		PIR1bits.TMR1IF = 0;
		Timer1_start();	
		Timer1_isr();
	}
	if (PIR2bits.TMR3IF){ // Timer3
		Timer3_stop();
		PIR2bits.TMR3IF = 0;
		Timer3_start();
		Timer3_isr();
	}
	if (INTCONbits.INT0IF){ // ext_interupt0
		INTCONbits.INT0IF = 0;
		//add code here
	}
    if (PIR1bits.RCIF == 1){ // UART interupt Receive
		PIR1bits.RCIF = 0;
	}
}

#pragma code
#pragma interrupt highIsr
void highIsr(void){
	if (INTCONbits.T0IF){ // Timer0
		Timer0_stop();
		INTCONbits.T0IF = 0;
		Timer0_start();
		Timer0_isr();
	}
	if (PIR1bits.TMR1IF){ // Timer1
		Timer1_stop();
		PIR1bits.TMR1IF = 0;
		Timer1_start();	
		Timer1_isr();
	}
	if (PIR2bits.TMR3IF){ // Timer3
		Timer3_stop();
		PIR2bits.TMR3IF = 0;
		Timer3_start();
		Timer3_isr();
	}
	if (INTCONbits.INT0IF){ // ext_interupt0
		INTCONbits.INT0IF = 0;
		//add code here
	}

    if(PIR1bits.RCIF == 1){ // UART interupt Receive
		PIR1bits.RCIF = 0;
	}
    //if(PIR1bits.TXIF == 1) // UART interupt Receive
}

