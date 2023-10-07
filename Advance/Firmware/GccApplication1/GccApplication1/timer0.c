/*
 * timer0.c 
 *
 * Created: 22/09/2023 5:14:14 pm
 *  Author: qhua835
 */ 
#include "timer0.h"
#include "display.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


ISR(TIMER0_COMPA_vect) { 
	send_next_character_to_display();
}


ISR(TIMER0_OVF_vect){
	// What is this for?
	// i forgot
	// Might've found an alternative forgot
}


void timer0_init() {
	//Set CTC mode and prescaler to 256 (100)
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS02);
	
	// Enable interrupt
	TIMSK0 |= (1<<OCIE0A);

	// Set the compare value for 10ms (9.984ms) 78?*F_CPU/256
	// the value of 77 is fine in proteus but on the actual 7 segment is flickering
	// therefore i just changed the value randomly to 10
	// which will be roughly 1.28ms
	OCR0A = 10;
	
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