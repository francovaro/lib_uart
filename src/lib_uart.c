/*
 * lib_uart.c
 *
 *  Created on: 27/08/2020
 *      Author: franc
 */
#include <stddef.h>
#include "lib_uart.h"

static void USART2_NVIC_Config(t_UART_lib_ uart_to_config);

static FunctionalState _USART_config[e_UART_None] = {DISABLE};

/**
 *
 * @param uart_to_config
 * @param irqEnabled
 */
ErrorStatus UART_lib_config(t_UART_lib_ uart_to_config, uint8_t irqEnabled, uint16_t IT_TO_SET, uint8_t dma_enabled)
{
	ErrorStatus			config = SUCCESS;
	GPIO_InitTypeDef 	GPIO_InitStruct;
	USART_InitTypeDef 	USART_InitStructure;
	USART_TypeDef*		_usart_to_use = NULL;

	usart_serialReceived = 0;
	usart_nrOfByte = 0;

	GPIO_StructInit(&GPIO_InitStruct);
	USART_StructInit(&USART_InitStructure);

	if ((_USART_config[uart_to_config] == ENABLE)
			|| uart_to_config >= e_UART_None)
	{
		config = ERROR;
	}
	else
	{
		switch(uart_to_config)
		{
			case e_UART_1:
			{

			}
			break;
			case e_UART_2:
			{
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

				// rx PA10
				// tx PA9
				GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
				GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

				// Initialize USART2_Tx
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;

				_usart_to_use = (USART_TypeDef *)USART2;
			}
			break;
			case e_UART_3:
			{

			}
			break;
			case e_UART_4:
			{

			}
			break;
			case e_UART_5:
			{

			}
			break;
			default:
			case e_UART_None:
			{
				/* theoretically can't fall here */
			}
			break;
		}

		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_Init(GPIOA , &GPIO_InitStruct);

		 /*!< This member configures the USART communication baud rate.
		     The baud rate is computed using the following formula:

		     - IntegerDivider = ((PCLKx) / (8 * (OVR8+1) * (USART_InitStruct->USART_BaudRate)))
		     - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 8 * (OVR8+1)) + 0.5
		 Where OVR8 is the "oversampling by 8 mode" configuration bit in the CR1 register. */

		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(_usart_to_use ,&USART_InitStructure);

		if(irqEnabled == ENABLE)
		{
			USART_ITConfig(_usart_to_use, IT_TO_SET, ENABLE); /* ENABLE INTERRUPT ON rx */
			USART2_NVIC_Config(uart_to_config);
		}

		if (dma_enabled != RESET)
		{
			USART_DMACmd(_usart_to_use, dma_enabled, ENABLE);
		}

		USART_Cmd(_usart_to_use , ENABLE);
	}

	return config;
}

/**
 *
 */
void UART_lib_sendData(const char * strToSend, uint16_t byteToSend)
{
	uint16_t count;

	for(count = 0 ; count < byteToSend ; count++)
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, *strToSend++);

	}
}

/**
 *
 * @param c
 * @return
 */
int UART_lib_putc(int c)
{
  assert_param(IS_USART_123_PERIPH(USARTx));

  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  USART2->DR =  (c & 0xff);

  return 0;
}

/**
 *
 */
static void USART2_NVIC_Config(t_UART_lib_ uart_to_config)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx Interrupt */
  switch(uart_to_config)
  {
  	  case e_UART_1:
  	  {
  		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  	  }
  	  break;
  	  case e_UART_2:
  	  {
  		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  	  }
  	  break;
  	  case e_UART_3:
  	  {
  		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  	  }
  	  break;
  	  case e_UART_4:
  	  {
  		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
  	  }
  	  break;
  	  case e_UART_5:
  	  {
  		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
  	  }
  	  break;
  	  default:
  	  case e_UART_None:
  	  {

  	  }
  	break;
  }

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
 *
 */
__attribute__((weak)) void USART2_IRQHandler(void)
{
	 if( USART_GetITStatus(USART2,USART_IT_RXNE ) != RESET)
	 {
		usart_rxBuffer[usart_nrOfByte] = USART_ReceiveData(USART2);
		usart_nrOfByte++;
		usart_serialReceived = SET;
	 }
}
