/*
 * uart.c
 *
 * Created: 19/08/2023 14:29:38
 *  Author: Tony
 */ 
#include <avr/io.h>
#include <stdint.h>
#include <string.h>

#include "uart.h"


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
