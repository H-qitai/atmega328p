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

volatile uint16_t counter = 0;
volatile uint8_t displayinfo = 0;
volatile uint16_t dispvoltage = 0;
volatile uint16_t dispcurrent = 0;
volatile uint16_t disppower = 0;

ISR(TIMER0_COMPA_vect) {

	if (counter == 200){
		counter = 0;
		switch (displayinfo){
			case 0:
				seperate_and_load_characters(dispvoltage, 2);
				break;
			case 1:
				seperate_and_load_characters(dispcurrent, 1);
				break;
			case 2:
				seperate_and_load_characters(disppower, 2);
				break;
		}
		displayinfo++;
		if (displayinfo == 4){
			displayinfo = 0;
		}
	}
	counter++;
	send_next_character_to_display();
}

ISR(TIMER0_OVF_vect){
	// This ISR is never reached in CTC mode.
	// Unless OCR0A = 255;
}


void timer0_init() {
	//Set CTC mode and prescaler to 256 (100)
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS02);
	
	// Enable interrupt
	TIMSK0 |= (1<<OCIE0A);

	// Set the compare value time a time of approximately 1ms
	// Tmatch = (OCRvalue + 1) / Ftimer
	// 1ms * 2Mhz/256 - 1 approx = 7
	OCR0A = 7;
	
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