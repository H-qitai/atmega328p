/*
 * GccApplication1.c
 *
 * Created: 18/08/2023 4:31:49 pm
 * Author : qhua835
 */ 

#define F_CPU 2000000UL

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#include "uart.h"


#define RMSVoltage 14.5
#define PeakCurrent 125
#define Power 1.60



int main(void)
{
	
	usart_init(12);  //initializing baud rate for 2MHz
    
	// Storing the message that needs to be printed.
	
	char voltage[] = "RMS Voltage is: ";
	char current[] = "Peak Current is: ";
	char power[] = "Power is: ";
	
	// Making new variables so they can be converted to the format that is needed to display easier.
	
	uint16_t new_voltage = RMSVoltage * 10;
	uint16_t new_current = PeakCurrent;
	uint16_t new_power = Power * 100;
	
	

    while (1) 
    {
		
		uart_transmit_msg(voltage);
		uart_transmit_char(new_voltage /100 % 10  + 48);
		uart_transmit_char(new_voltage /10 % 10  + 48);
		uart_transmit_char('.');
		uart_transmit_char(new_voltage /1 % 10  + 48);							// Just transmitting.
		
		uart_transmit_char('\r');
		uart_transmit_char('\n');
		
		uart_transmit_msg(current);
		uart_transmit_char(new_current /100 % 10  + 48);
		uart_transmit_char(new_current /10 % 10  + 48);
		uart_transmit_char(new_current /1 % 10  + 48);
		
		uart_transmit_char('\r');
		uart_transmit_char('\n');
		
		uart_transmit_msg(power);
		uart_transmit_char(new_power /100 % 10  + 48);
		uart_transmit_char('.');
		uart_transmit_char(new_power /10 % 10  + 48);
		uart_transmit_char(new_power /1 % 10  + 48);
		
		uart_transmit_char('\r');
		uart_transmit_char('\n');
		uart_transmit_char('\n');
		

		_delay_ms(1000);  // Delay for 1 second.
    
	}
}

