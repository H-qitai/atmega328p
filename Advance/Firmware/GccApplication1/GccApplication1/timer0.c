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


// Defining the references voltages and current/ power, these information will be collected from adc later on.
#define RMSVoltage 14.5
#define PeakCurrent 125
#define Power 1.60


// Making new variables so they can be converted to the format that is needed to display easier.
	
volatile uint16_t new_voltage = RMSVoltage * 10;
volatile uint16_t new_power = Power * 100;

ISR(TIMER0_COMPA_vect) { 
	send_next_character_to_display();
}


// bugged
ISR(TIMER0_OVF_vect){
	uart_transmit_msg("RMS Voltage is: ");
	uart_transmit_char(new_voltage /100 % 10  + 48);
	uart_transmit_char(new_voltage /10 % 10  + 48);
	uart_transmit_char('.');
	uart_transmit_char(new_voltage /1 % 10  + 48);							// Just transmitting.
	uart_transmit_msg("\r\n");

	printf("Peak Current is: %d\r\n", PeakCurrent);
			
	uart_transmit_msg("Power is: ");
	uart_transmit_char(new_power /100 % 10  + 48);
	uart_transmit_char('.');
	uart_transmit_char(new_power /10 % 10  + 48);
	uart_transmit_char(new_power /1 % 10  + 48);
	uart_transmit_msg("\r\n");
	uart_transmit_msg("\r\n");
}


void timer0_init() {
	//Set CTC mode (WGM01) and prescaler to 256 (100)
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
	TCCR0A = 0;
	TCCR0B = 0;
}


uint8_t timer0_check_clear_compare(){
	if (TIFR0 & (1 << OCF0A )){
		//Note: in datasheet this is done by writing 1 to the compare flag
		TIFR0 |= (1<<OCF0A);
		
		return 1;
	}
	return 0;
}