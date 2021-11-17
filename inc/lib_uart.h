/*
 * lib_uart.c
 *
 *  Created on: 27/08/2020
 *      Author: franc
 */

#ifndef _LIB_UART_H_
#define _LIB_UART_H_

#include "stm32f4xx.h"

#define USART_MAX_BUFFER (255u)

volatile uint8_t usart_rxBuffer[USART_MAX_BUFFER];

volatile FlagStatus usart_serialReceived;
volatile uint16_t usart_nrOfByte;

typedef enum
{
	e_UART_1,
	e_UART_2,
	e_UART_3,
	e_UART_4,
	e_UART_5,
	e_UART_None
}t_UART_lib_;

extern ErrorStatus UART_lib_config(t_UART_lib_ uart_to_config,uint8_t irqEnabled, uint16_t IT_TO_SET, uint8_t dma_enabled);
extern void UART_lib_sendData(t_UART_lib_ sel_uart, char * strToSend, uint16_t byteToSend);
extern void UART_lib_free_send(t_UART_lib_ sel_uart, char * strToSend);
extern int UART_lib_putc(int c);

#endif /* _LIB_UART_H_ */
