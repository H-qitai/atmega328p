/*
 * uart.h
 *
 * Created: 19/08/2023 14:30:11
 *  Author: Tony
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
void usart_init(uint16_t ubrr);
void uart_transmit_char(uint8_t data);
void uart_transmit_msg(char* msg);



#endif /* UART_H_ */