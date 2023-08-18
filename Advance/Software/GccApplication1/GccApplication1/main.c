/*
 * GccApplication1.c
 *
 * Created: 18/08/2023 4:31:49 pm
 * Author : qhua835
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>


#define RMSVoltage 14.5
#define PeakCurrent 125
#define Power 1.60


void usart_init(uint16_t ubrr){
	UCSR0A &= ~(1<<UDRE0);    // Clearing the UDRE0, so that we can start transmitting data.
	UCSR0B |= (1<<TXEN0);		// Enabling TXEN0, to allow transmission
	
	UBRR0H = (uint8_t) (ubrr>>8); // Don't really know how this 100% works, got it from data sheet.
	UBRR0L = (uint8_t) (ubrr);
	
}

void uart_transmit_char(uint8_t data){
	while (!(UCSR0A & (1<<UDRE0))){		// Making sure theres no data in transmit register, before storing new information in for transmission.
		;
	}
	
	UDR0 = data;		// Putting data in.
}

void uart_transmit_msg(char* msg){
	
	// This function takes in a char array, and transmit it character by character.
	
	for (uint8_t i = 0; i < strlen(msg); i++){
		uart_transmit_char(msg[i]);
	}
}

// int uart_printf(char var, FILE *stream) {
// 	uart_transmit_char(var); //Using our original function to transmit UART data
// 	return 0;
// }
// 
// 	static FILE usart_stdout = FDEV_SETUP_STREAM(uart_printf, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	
	usart_init(103);  //initializing baud rate
    
	// Storing the message that needs to be printed.
	
	char voltage[] = "RMS Voltage is: ";
	char current[] = "Peak Current is: ";
	char power[] = "Power is: ";
	
	// Making new variables so they can be converted to the format that is needed to display easier.
	
	uint16_t new_voltage = RMSVoltage * 10;
	uint16_t new_current = PeakCurrent;
	uint16_t new_power = Power * 100;
	
	//Binding the stream variable to stdout
	//stdout = &usart_stdout;
	
	

    while (1) 
    {
		
		//uart_printf('0', stdout);
		
		
		uart_transmit_msg(voltage);
		uart_transmit_char(new_voltage /100 % 10  + 48);
		uart_transmit_char(new_voltage /10 % 10  + 48);
		uart_transmit_char('.');
		uart_transmit_char(new_voltage /1 % 10  + 48);							// Just transmitting.
		
		uart_transmit_char('\n\r');
		
		uart_transmit_msg(current);
		uart_transmit_char(new_current /100 % 10  + 48);
		uart_transmit_char(new_current /10 % 10  + 48);
		uart_transmit_char(new_current /1 % 10  + 48);
		
		uart_transmit_char('\n\r');
				
		uart_transmit_msg(power);
		uart_transmit_char(new_power /100 % 10  + 48);
		uart_transmit_char('.');
		uart_transmit_char(new_power /10 % 10  + 48);
		uart_transmit_char(new_power /1 % 10  + 48);
		
		uart_transmit_char('\n\r');
		uart_transmit_char('\n\r');
		uart_transmit_char('\n\r');
		

		_delay_ms(1000);  // Delay for 1 second.
    }
}

