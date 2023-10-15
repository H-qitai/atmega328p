/*
 * adc.c
 *
 * Created: 16/09/2023 16:13:10
 *  Author: Tony
 */ 

#include "adc.h"
#include "uart.h"
#include <avr/io.h> //Necessary for definitions of ADMUX etc
#include <avr/interrupt.h>
#include <stdint.h>
#include <math.h>

#define SAMPLESIZE 40


ISR(ADC_vect){
	for (uint8_t i = 0; i < SAMPLESIZE; i++){
		voltage_adc[i] = adc_read(0);
		current_adc[i] = adc_read(1);
	}
}

void adc_init(void){
	ADMUX |= (1<<REFS0); //AVCC set as reference, ADC0 selected and results are right adjusted
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADIE) | (1<<ADATE) | (1<<ADSC); //Set ADEN bit to 1 (enable ADC) and prescaler to 100 (i.e. 16), Set on auto conversion mode
	ADCSRB |= (1<<ADTS1);  // Trigger source is the zero voltage crossing on rising edge
}

uint16_t adc_read(uint8_t chan){
	 ADMUX &= 0xF0;  // Channel clear.
	 ADMUX |= chan;
	 
	 // Start conversion
	 ADCSRA |= (1<<ADSC);
	 
	 while (!(ADCSRA & (1<<ADIF))) { //ADIF bit is checked to see if it is 0 //If ADIF bit is not 1, wait until it becomes 1
	 }

	 return ((ADCL << 0) | (ADCH << 8));
}

uint16_t Vadc_to_Vsquaredadc(float adcvalues[40]){
	// This function takes the value the adc read and stored.
	// Square each individual value and it is den all summed together
	// and it is divided by the sample size and squared.
	
	float adc_rms32 = 0.0;
	float rms = 0;
	
	for (uint8_t i = 0; i < 40; i++){
		adc_rms32 = (adcvalues[i] * adcvalues[i]);
		rms = adc_rms32 + rms;
	}
	rms = sqrt(rms/40.0)*10;
	
	return (uint16_t) rms;
}

uint16_t Iadc_to_Isquaredadc(float adcvalues[40]){
	// This function takes the value the adc read and stored.
	// Square each individual value and it is den all summed together
	// and it is divided by the sample size and squared.
	
	float adc_rms32 = 0.0;
	float rms = 0;
	
	for (uint8_t i = 0; i < 40; i++){
		adc_rms32 = (adcvalues[i] * adcvalues[i]);
		rms = adc_rms32 + rms;
	}
	rms = sqrt(rms/40.0)*100*sqrt(2);
	
	return (uint16_t) rms;
}