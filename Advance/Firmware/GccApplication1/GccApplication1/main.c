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
	
	// Declare the variable and arrays needed for calculation later
	float voltage_ac[SAMPLESIZE] = {0};
	float current_ac[SAMPLESIZE] = {0};
	float voltage_bar[SAMPLESIZE] = {0};
	float current_bar[SAMPLESIZE] = {0};
	float powerall[SAMPLESIZE] = {0};
	uint16_t voltage_pk = 0;
	uint16_t current_rms = 0;
	float fpower = 0;
	uint16_t power = 0;

	// initializing baud rate for 2MHz
	// initialize timer0
	// initialize display
	adc_init();
	timer0_init();
	usart_init(12);
	int0_init();
	display_init();

	// enable global interrupt
	sei();

    while (1)
    {	
		// Wait for adc to finish converting...
		// 1 sample is 0.1ms 80 samples in total for both current and voltage
		// 8ms is needed
		_delay_ms(8);
		// Converting ADC value to Voltage/100 and mA
		// Offset is stripped and formulas applied.
		for (uint8_t i = 0; i < SAMPLESIZE; i++){
			voltage_ac[i] = adc_to_V(voltage_adc[i]);
			current_ac[i] = adc_to_A(current_adc[i]);
		}
		
		// Converts the adc values to square and sum
		// AKA applying Riemann Sum
		// Convert Pinstantaenous to Paverage(Real power)
		voltage_pk = Vadc_to_Vsquaredadc(voltage_ac);
		current_rms = Iadc_to_Isquaredadc(current_ac);
		for (uint8_t i = 0; i < SAMPLESIZE; i++) {
			if (i == 0) {
				voltage_bar[i] = (voltage_ac[i] + voltage_ac[i+1]) / 2;
				current_bar[i] = (current_ac[SAMPLESIZE-1] + current_ac[i]) / 2;
				}else if (i == SAMPLESIZE - 1) {
				voltage_bar[i] = (voltage_ac[i] + voltage_ac[0]) / 2;
				current_bar[i] = (current_ac[i-1] + current_ac[i]) / 2;
				}else{
				voltage_bar[i] = (voltage_ac[i] + voltage_ac[i+1]) / 2;
				current_bar[i] = (current_ac[i-1] + current_ac[i]) / 2;
			}
		}
		for (uint8_t i = 0; i <SAMPLESIZE; i ++){
			powerall[i] = (voltage_ac[i] * current_bar[i]) + (voltage_bar[i] * current_ac[i]);

			// Accumulate the instantaneous power
			fpower = fpower + powerall[i];
		}
		
		fpower = fpower/80.0;
		fpower = fpower * 100;
		power = (uint16_t) fpower;
		
// 		fpower = linear_approximation(voltage_ac, current_ac);
// 		fpower = fpower;
// 		power = (uint16_t)fpower;
		
		
// 		printf("Voltage, Current\r\n");
// 		for (uint8_t i = 0; i < SAMPLESIZE; i++){
// 			printf("%d,%d\r\n", voltage_adc[i], current_adc[i]);
// 		}
		
		// The values calculated above is now displayed
		// Just transmitting.
		printf("The RMS voltage is: %d%d.%dV\r\n", (voltage_pk/100%10), (voltage_pk/10%10), (voltage_pk%10));
 		printf("The RMS current is:  %d.%d%dA\r\n", (current_rms/100%10), (current_rms/10%10), (current_rms%10));
		printf("Power is: %d%d.%dW\r\n",((int)(power /10 %10)), (int)(power %10), (int)(power *10 %10));
		printf("\r\n");

		dispvoltage = voltage_pk;
		dispcurrent = current_rms;
		disppower = power;
		flag = 0;
		fpower = 0;
	}
}

	
