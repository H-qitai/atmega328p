/*
 * adc.c
 *
 * Created: 16/09/2023 16:13:10
 *  Author: Tony
 */ 

#include "adc.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <math.h>

#define SAMPLESIZE 40
volatile uint8_t i = 0;
volatile uint8_t j = 0;
volatile uint8_t flag = 0;


ISR(ADC_vect){
	if (flag == 0){
		if (i < SAMPLESIZE){
			voltage_adc[i] = adc_read(0);

		} else if (i >= SAMPLESIZE){
			current_adc[i-SAMPLESIZE] = adc_read(1);
		}
		if (i >= SAMPLESIZE*2){
			i = 0;
			flag = 1;
		}
		i++;
		TIFR0 |= (1<<OCF0A);
	}
}

void adc_init(void){
	ADMUX |= (1<<REFS0); //AVCC set as reference, ADC0 selected and results are right adjusted
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADIE) | (1<<ADATE); //Set ADEN bit to 1 (enable ADC) and prescaler to 100 (i.e. 16), Set on auto conversion mode
	ADCSRB |= (1<<ADTS1) | (1<<ADTS0);  // Trigger source is the zero voltage crossing on rising edge
	ADCSRA  |= (1<<ADSC);
}

uint16_t adc_read(uint8_t chan){
	 ADMUX &= 0xF0;  // Channel clear.
	 ADMUX |= chan;
	 
	 // Start conversion
	 ADCSRA |= (1<<ADSC);
	 
	 while (!(ADCSRA & (1<<ADIF))) { //ADIF bit is checked to see if it is 0 //If ADIF bit is not 1, wait until it becomes 1
	 ;
	 }

	 return ((ADCL << 0) | (ADCH << 8));
}

uint16_t Vadc_to_Vsquaredadc(volatile float adcvalues[40]){
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

uint16_t Iadc_to_Isquaredadc(volatile float adcvalues[40]){
	// This function takes the value the adc read and stored.
	// Square each individual value and it is den all summed together
	// and it is divided by the sample size and squared.
	
	float adc_rms32 = 0.0;
	float rms = 0;
	
	for (uint8_t i = 0; i < 40; i++){
		adc_rms32 = (adcvalues[i] * adcvalues[i]);
		rms = adc_rms32 + rms;
	}
	rms = sqrt(rms/40.0)*sqrt(2)*100;
	
	return (uint16_t) rms;
}


float linear_approximation(volatile float vac[SAMPLESIZE], volatile float iac[SAMPLESIZE]){
	// Declare the values needed for calculation
    float power = 0;
	float voltage_bar = 0;
	float current_bar = 0;
	float instantaneous_power = 0;


	// Implement linear approximation and sum all instantaneous power
    for (uint8_t i = 0; i < SAMPLESIZE; i++) {
		voltage_bar = 0;
		current_bar = 0;
		instantaneous_power = 0;

	    // Calculate voltage_bar and current_bar
	    if (i == 0) {
		    voltage_bar = (vac[i] + vac[i+1]) / 2;
		    current_bar = (iac[SAMPLESIZE-1] + iac[i]) / 2;
		}else if (i == SAMPLESIZE - 1) {
		    voltage_bar = (vac[i] + vac[0]) / 2;
		    current_bar = (iac[i-1] + iac[i]) / 2;
		}else{
		    voltage_bar = (vac[i] + vac[i+1]) / 2;
		    current_bar = (iac[i-1] + iac[i]) / 2;
	    }

	    // Calculate instantaneous power for the current sample
	    instantaneous_power = (vac[i] * current_bar) + (voltage_bar * iac[i]);

	    // Accumulate the instantaneous power
	    power = power + instantaneous_power;
    }
	
	// Scale and convert to Average power (Real)
    return power*10/80;
}