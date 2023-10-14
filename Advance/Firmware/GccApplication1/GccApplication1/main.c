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
// This can be used later to store the Vrms/Ipk
// volatile extern	uint16_t voltage_rms = 0;
// volatile extern	uint16_t current_pk = 0;
// volatile extern	uint32_t powersum = 0;

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
	long int voltage_ac[SAMPLESIZE] = {0};
	long int current_ac[SAMPLESIZE] = {0};
	int voltage_bar[SAMPLESIZE] = {0};
	int current_bar[SAMPLESIZE] = {0};
	long int power[SAMPLESIZE] = {0};
	uint16_t voltage_rms = 0;
	uint16_t current_pk = 0;
	uint32_t powersum = 0;

    while (1) 
    {		
		// Converting ADC value to Voltage/100 and mA
		// Offset is stripped and formulas applied.
		for (uint8_t i = 0; i < SAMPLESIZE; i++){;
			voltage_ac[i] = ((((long int)voltage_adc[i]*500/1024)-205) * 22);
			current_ac[i] = ((((long int)current_adc[i]*5000/1024)-2053) * 2);
		}
		
		// Applying linear approximation for 
		for (uint8_t i = 0; i < SAMPLESIZE; i++){
			if (i == SAMPLESIZE-1){
				voltage_bar[i] = (voltage_ac[i] + voltage_ac[0])/2;
			}else{
				voltage_bar[i] = (voltage_ac[i] + voltage_ac[i+1])/2;
			}
			if (i == 0){
				current_bar[i] = (current_ac[SAMPLESIZE-1] + current_ac[i])/2;
			}else{
				current_bar[i] = (current_ac[i-1] + current_ac[i])/2;
			}
		}
		
		// Calculating all instantaneous power
		for (uint8_t i = 0; i < SAMPLESIZE; i++){
			power[i] = ((voltage_ac[i] * (long int) current_bar[i])) + (((long int) voltage_bar[i] * current_ac[i]));
		}
		
		// Summing all instantaneous for average power later
		for (uint8_t i = 0; i < SAMPLESIZE; i++){
			powersum = powersum + power[i];
		}
		
		// Converts the adc values to square and sum
		// AKA applying Riemann Sum
		// Convert Pinstantaenous to Paverage(Real power)
		voltage_rms = adc_to_squaredadc(voltage_ac);//  * 14/10; If Vpk is needed this is den added.
		current_pk = adc_to_squaredadc(current_ac);
		powersum = powersum/80/1000;
				
				
		// The values calculated above is now displayed		
		// Just transmitting.
		printf("RMS Voltage is: %d%d.%d%dV\r\n", (voltage_rms /1000 % 10), (voltage_rms /100 % 10), (voltage_rms /10 % 10), (voltage_rms % 10));
 		printf("Peak Current is:  %dmA\r\n", current_pk * 14 / 10);
		printf("Power is: %lu%lu.%lu%luW\r\n",(powersum /1000 %10), (powersum /100 %10), (powersum /10 %10), (powersum /1 % 10));
		//printf("Energy: %iWh\r\n", current_pk/14*10 * voltage_rms);
		printf("\r\n");

		dispvoltage = voltage_rms;
		dispcurrent = current_pk*14/10;
		disppower = powersum;
		powersum = 0;
	}
}

