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



ISR(ADC_vect){
	// This is where the adc reading will be.
	// Implement later
}

void adc_init(void){
	ADMUX |= (1<<REFS0); //AVCC set as reference, ADC0 selected and results are right adjusted
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADIF) | (1<<ADATE); //Set ADEN bit to 1 (enable ADC) and prescaler to 100 (i.e. 16)
	ADCSRB |= (1<<ADTS1); 
}

uint16_t adc_read(uint8_t chan){
	 ADMUX &= 0xF0;  // Channel clear.
	 ADMUX |= chan;
	 
	 ADCSRA |= (1 << ADSC); // Start the conversion.... (Very important)
	 
	 
	 while (!(ADCSRA & (1<<ADIF))) { //ADIF bit is checked to see if it is 0 //If ADIF bit is not 1, wait until it becomes 1
	 }

	 return ((ADCL << 0) | (ADCH << 8));
}

uint16_t adc_to_squaredadc(uint16_t adcvalues[40]){
	// This function takes the value the adc read and stored.
	// Square each individual value and it is den all summed together
	// and it is divided by the sample size and squared.
	
	uint32_t adc_rms32 = 0;
	
	for (uint8_t i = 0; i < 40; i++){
		adc_rms32 += (uint32_t)adcvalues[i] * (uint32_t)adcvalues[i];

	}
	
	return sqrt(adc_rms32/40);
}


uint16_t adc_convert_mv(uint16_t voltage){
	// This function converts adc reading to mV.
	
	uint32_t voltage_mv = 0;
	voltage_mv = (uint32_t)voltage * 5000/1024;
	return voltage_mv;
}

uint16_t adc_convert_ma(uint16_t current){
	// This function converts adc reading to mA.
	// Same as above idk why i didnt implement this in one function. :p
	
	uint32_t current_ma = 0;
	current_ma = (uint32_t)current * 5000/1024;
	return current_ma;
}