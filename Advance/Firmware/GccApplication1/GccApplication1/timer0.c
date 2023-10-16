/*
 * timer0.c 
 *
 * Created: 22/09/2023 5:14:14 pm
 *  Author: qhua835
 */ 
#include "timer0.h"
#include "display.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


ISR(TIMER0_COMPA_vect) {
	
}


void timer0_init() {
	//Set CTC mode and prescaler to 8 (010)
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1<<CS01);
	
	// Enable interrupt
	TIMSK0 |= (1<<OCIE0A);

	// Set the compare value time a time of approximately 1ms
	// Tmatch = (OCRvalue + 1) / Ftimer
	// 1ms * 2Mhz/256 - 1 approx = 7
	
	// 0.1ms * 2Mhz/8 - 1 = 24
	OCR0A = 24;
	
	// Set the count value to zero
	TCNT0 = 0;
}

void timer0_termit() {
	// This function terminates timer0
	// Why do i have this?
	// Zero clue
	
	TCCR0A = 0;
	TCCR0B = 0;
}


uint8_t timer0_check_clear_compare(){
	// This function checks if the compare match flag has been cleared or not
	
	if (TIFR0 & (1 << OCF0A )){
		//Note: in datasheet this is done by writing 1 to the compare flag
		TIFR0 |= (1<<OCF0A);
		
		return 1;
	}
	return 0;
}