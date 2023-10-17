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
volatile uint8_t flag = 0;


ISR(ADC_vect){
	if (flag == 0){
		flag = 1;
		for (uint8_t i = 0; i < SAMPLESIZE; i++){
			voltage_adc[i] = adc_read(0);
			current_adc[i] = adc_read(1);
		}
	}
// 	if (flag == 0){
// 		if (i < SAMPLESIZE){
// 			voltage_adc[i] = adc_read(0);
// 
// 		} else if (i >= SAMPLESIZE){
// 			current_adc[i - SAMPLESIZE] = adc_read(1);
// 		}
// 		i++;
// 		if (i >= SAMPLESIZE*2){
// 			i = 0;
// 			flag = 1;
// 		}
// 	}
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
	 
	 ADCSRA |= (1<<ADIF);

	 return ((ADCL << 0) | (ADCH << 8));
}

uint16_t Vadc_to_Vsquaredadc(float acvalues[40]){
	// This function takes the value the adc read and stored.
	// Square each individual value and it is den all summed together
	// and it is divided by the sample size and squared.
	
	float adc_rms32 = 0.0;
	float rms = 0;
	
	for (uint8_t i = 0; i < 40; i++){
		adc_rms32 = (acvalues[i] * acvalues[i]);
		rms = adc_rms32 + rms;
	}
	
	rms = rms/SAMPLESIZE;
	rms = sqrt(rms);
	rms = rms*10;
	
	return (uint16_t) rms;
}

uint16_t Iadc_to_Isquaredadc(float acvalues[40]){
	// This function takes the value the adc read and stored.
	// Square each individual value and it is den all summed together
	// and it is divided by the sample size and squared.
	
	float adc_rms32 = 0.0;
	float rms = 0;
	
	for (uint8_t i = 0; i < 40; i++){
		adc_rms32 = (acvalues[i] * acvalues[i]);
		rms = adc_rms32 + rms;
	}
	
	rms = rms/SAMPLESIZE;
	rms = sqrt(rms);
	rms = rms * 100;
	
	return (uint16_t) rms;
}

float adc_to_V(uint16_t voltage_adc){
	// This function converts adc voltage value to  voltage
	float voltage = 0;
	voltage = (float) voltage_adc;
	voltage = voltage * 5.0;
	voltage = voltage / 1024.0;
	voltage = voltage - 2.053;
	voltage = voltage * 21.74;
	
	return voltage;
}

float adc_to_A(uint16_t current_adc){
	// This function converts adc current value to current
	float current = 0;
	current = (float) current_adc;
	current = current * 5.0;
	current = current / 1024.0;
	current = current - 2.053;
	current = current / 1.1;
	
	return current;
}



float linear_approximation(float vac[SAMPLESIZE], float iac[SAMPLESIZE]){
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
	power = power * 100;
	power = power * 1.7 * sqrt(2);
	return power/(SAMPLESIZE*2);
}