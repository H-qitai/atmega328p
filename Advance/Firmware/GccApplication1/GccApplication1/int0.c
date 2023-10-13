/*
 * int0.c
 *
 * Created: 13/10/2023 6:07:03 pm
 *  Author: qhua835
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "int0.h"
#include "uart.h"

ISR(INT0_vect){

}


void int0_init(){
	// INT0 is enabled
	// Den it generates an interrupt when it detects a rising edge
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01) | (1<<ISC00);
}