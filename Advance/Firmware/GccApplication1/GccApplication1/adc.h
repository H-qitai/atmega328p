/*
 * adc.h
 *
 * Created: 16/09/2023 16:12:40
 *  Author: Tony
 */ 

//Note the use of #ifndef, #define, #endif to protect against multiple inclusions
#ifndef _ADC_H
#define _ADC_H
#define SAMPLESIZE 40
#include <stdint.h> //Necessary for definitions of uint8_t etc

volatile extern uint16_t voltage_adc[SAMPLESIZE];
volatile extern uint16_t current_adc[SAMPLESIZE];

// Functions linkers
void adc_init();
uint16_t adc_read(uint8_t chan);

uint16_t Vadc_to_Vsquaredadc(float adcvalues[40]);
uint16_t Iadc_to_Isquaredadc(float adcvalues[40]);
float linear_approximation(float vac[SAMPLESIZE], float iac[SAMPLESIZE]);


#endif