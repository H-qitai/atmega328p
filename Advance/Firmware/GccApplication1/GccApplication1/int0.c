/*
 * int0.c
 *
 * Created: 13/10/2023 6:07:03 pm
 *  Author: qhua835
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.h"
#include "int0.h"
#include "uart.h"

volatile uint16_t counter = 0;
volatile uint8_t displayinfo = 0;
volatile uint16_t dispvoltage = 0;
volatile uint16_t dispcurrent = 0;
volatile uint16_t disppower = 0;

ISR(INT0_vect){
	if (counter == 500){
		counter = 0;
		switch (displayinfo){
			case 0:
				seperate_and_load_characters(dispvoltage, 3, 1);
				break;
			case 1:
				seperate_and_load_characters(dispcurrent, 2, 2);
				break;
			case 2:
				seperate_and_load_characters(disppower, 3, 3);
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


void int0_init(){
	// INT0 is enabled
	// Den it generates an interrupt when it detects a rising edge
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01) | (1<<ISC00);
}