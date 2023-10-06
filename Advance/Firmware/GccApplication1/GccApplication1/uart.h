/*
 * uart.h
 *
 * Created: 19/08/2023 14:30:11
 *  Author: Tony
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdio.h>

// Declare usart_init
void usart_init(uint16_t ubrr);

// Declare uart_transmit a single character
void uart_transmit_char(uint8_t data);

// Declare uart_transmit a message
void uart_transmit_msg(char* msg);

// straight up print the message
int uart_printf(char var, FILE *stream);



#endif /* UART_H_ */