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
#include <stdlib.h>
#include <stdio.h>

// including all header files
#include "uart.h"
#include "timer0.h"
#include "display.h"
#include "adc.h"
#include "int0.h"

// Macro
#define SAMPLESIZE 40


// Magic statement
static FILE usart_stdout = FDEV_SETUP_STREAM(uart_printf, NULL, _FDEV_SETUP_WRITE);

// Create two arrays to store the values of voltage as well as current
// These arrays stores the ADC value when ISR is triggered.
volatile extern uint16_t voltage_adc[SAMPLESIZE] = {0};
volatile extern uint16_t current_adc[SAMPLESIZE] = {0};


int main(void)
{
	// magic
	stdout = &usart_stdout;
		
	// initializing baud rate for 2MHz
	// initialize timer0
	// initialize display
	// enable global interrupt
	usart_init(12);
	timer0_init();
	int0_init();
	display_init();
	adc_init();
	sei();
	
	// Declare the variable and arrays needed for calculation later
	float voltage_ac[SAMPLESIZE] = {0};
	float current_ac[SAMPLESIZE] = {0};
	uint16_t voltage_rms = 0;
	uint16_t current_pk = 0;
	uint16_t power = 0;

    while (1) 
    {		
		// Converting ADC value to Voltage/100 and mA
		// Offset is stripped and formulas applied.
		for (uint8_t i = 0; i < SAMPLESIZE; i++){;
			voltage_ac[i] = (((((float)voltage_adc[i])*5.0/1024)-2.053) * 21.74);
			current_ac[i] = (((((float)current_adc[i])*5.0/1024)-2.053) * 2);
		}
		
		// Converts the adc values to square and sum
		// AKA applying Riemann Sum
		// Convert Pinstantaenous to Paverage(Real power)
		voltage_rms = Vadc_to_Vsquaredadc(voltage_ac);//  * 14/10; If Vpk is needed this is den added.
		current_pk = Iadc_to_Isquaredadc(current_ac);
		power = linear_approximation(voltage_ac, current_ac);
		
		// The values calculated above is now displayed
		// Just transmitting.
		printf("RMS Voltage is: %d%d.%dV\r\n", (voltage_rms/100%10), (voltage_rms/10%10), (voltage_rms%10));
 		printf("Peak Current is:  %d.%d%dA\r\n", (current_pk/100%10), (current_pk/10%10), (current_pk%10));
 		printf("Power is: %d%d.%dW\r\n",(power /100 %10), (power /10 %10), (power %10));
		printf("\r\n");

		dispvoltage = voltage_rms;
		dispcurrent = current_pk;
		disppower = power;
	}
}

	
