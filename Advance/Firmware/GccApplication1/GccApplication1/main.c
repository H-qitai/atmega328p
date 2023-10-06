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
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

#include "uart.h"
#include "timer0.h"
#include "display.h"

// Magic statement
static FILE usart_stdout = FDEV_SETUP_STREAM(uart_printf, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	
	usart_init(12);  //initializing baud rate for 2MHz
	timer0_init();
	
	// magic
	stdout = &usart_stdout;
	
	uint16_t count = 0;
	
	timer0_init();
	init_display();
	sei();
	
	

    while (1) 
    {	
		seperate_and_load_characters(count, 1);
		_delay_ms(400);
		count++;
		if (count > 9999){
			count = 0;
		}
    
	}
}

