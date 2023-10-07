/*
 * GccApplication1.c
 *
 * Created: 18/08/2023 4:31:49 pm
 * Author : qhua835
 */ 
// Defining the controller frequency
#define F_CPU 2000000UL

// Including all library needed
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

// including all header files
#include "uart.h"
#include "timer0.h"
#include "display.h"
#include "adc.h"

// The lines below shalled be removed later
// Defining the references voltages and current/ power, these information will be collected from adc later on.
#define Power 1.6
// Making new variables so they can be converted to the format that is needed to display easier. (temporary)
volatile uint16_t new_power = Power * 100;



// Magic statement
static FILE usart_stdout = FDEV_SETUP_STREAM(uart_printf, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	// magic
	stdout = &usart_stdout;
		
	uint16_t count = 0; // Just a temporary testing variable from Lab 6 for 7 segment
	
	// Create two arrays to store the values of voltage as well as current
	uint16_t voltage[40] = {0};
	uint16_t current[40] = {0};
	
	// This can be used later to calculate the Vrms/Irms
	uint16_t voltage_rms = 0;
	uint16_t current_rms = 0;
	
	
	// initializing baud rate for 2MHz
	// initialize timer0
	// initialize display
	// enable global interrupt
	usart_init(12);  
	timer0_init();
	display_init();
	adc_init();
	sei();
	
    while (1) 
    {	
		seperate_and_load_characters(count, 1);   // Leave like this for now (testing)
		_delay_ms(400);							  // Will display RMS value of voltage and current later.
		count++;
		if (count > 9999){
			count = 0;
		}
		
		// load the value read from adc to the array
		// Should be implemented in ISR instead
		for (uint8_t i = 0; i < 40; i++){
			voltage[i] = adc_read(0);
			current[i] = adc_read(1);
		}
		
		// Converts the adc values to square and sum
		// Than the value is converted to milli scale for easier display
		voltage_rms = adc_convert_mv((adc_to_squaredadc(voltage)));
		current_rms = adc_convert_ma(adc_to_squaredadc(current));
	
		
		// The values calculated above is now displayed		
		printf("RMS Voltage is: %d.%d%d%d\r\n", (voltage_rms /1000 % 10), (voltage_rms /100 % 10), (voltage_rms /10 % 10), (voltage_rms % 10));
		printf("RMS Current is:  %d.%d%d%d\r\n", (current_rms /1000 % 10), (current_rms /100 % 10), (current_rms /10 % 10), (current_rms % 10));                  // Just transmitting.
		
		
		// This is currently just a testing for power
		// Real power calculation will come later
		
// 		uart_transmit_msg("Power is: ");
// 		uart_transmit_char(new_power /100 % 10  + 48);
// 		uart_transmit_char('.');
// 		uart_transmit_char(new_power /10 % 10  + 48);
// 		uart_transmit_char(new_power /1 % 10  + 48);
// 		uart_transmit_msg("\r\n");
// 		uart_transmit_msg("\r\n");
    
	}
}

