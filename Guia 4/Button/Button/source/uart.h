/*
 * uart.h
 *
 *  Created on: Oct 30, 2020
 *      Author: Guido
 */

#ifndef UART_H_
#define UART_H_


void uart_init (UART_Type * uartch, int sysclk, int baud);
char uart_getchar (UART_Type * channel);
void uart_putchar (UART_Type * channel, char ch);

#endif /* UART_H_ */
