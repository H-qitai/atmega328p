/*
 * timer0.h
 *
 * Created: 22/09/2023 5:13:39 pm
 *  Author: qhua835
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_
#include <stdbool.h>
#include <stdint.h>

volatile uint16_t dispvoltage;
volatile uint16_t dispcurrent;
volatile uint16_t disppower;

//Initialize timer0 as per Part 1
void timer0_init();
// Terminating the timer
void timer0_termit();

//Using polling check if timer0 has reached comparison value
//if so, it will clear the compare flag and return 1
//otherwise, it returns 0
uint8_t timer0_check_clear_compare();


#endif