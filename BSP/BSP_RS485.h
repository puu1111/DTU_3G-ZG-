
#ifndef __BSP_USART1_H
#define __BSP_USART1_H

#include "stm32f0xx.h"
#include <stdio.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	
	
#define DIR485_Receive()    GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define DIR485_Send()       GPIO_SetBits(GPIOB,GPIO_Pin_1) 
void RS485_Init(void);

#endif
