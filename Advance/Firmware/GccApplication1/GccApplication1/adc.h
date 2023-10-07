/*
 * adc.h
 *
 * Created: 16/09/2023 16:12:40
 *  Author: Tony
 */ 

//Note the use of #ifndef, #define, #endif to protect against multiple inclusions
#ifndef _ADC_H
#define _ADC_H

#include <stdint.h> //Necessary for definitions of uint8_t etc

// Functions linkers
void adc_init();
uint16_t adc_read(uint8_t chan);
uint16_t adc_convert_mv(uint16_t voltage);
uint16_t adc_convert_ma(uint16_t current);

uint16_t adc_to_squaredadc(uint16_t adcvalues[40]);



#endif