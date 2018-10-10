
#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f0xx.h"

#define LED1_OFF                       GPIOC->BRR = 0x01 //GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define LED1_ON                        GPIOC->BSRR = 0x01//GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define LED1_TURN                      GPIOC->ODR ^= 0x01

#define LED2_OFF                       GPIOB->BRR = 0x02 //GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define LED2_ON                        GPIOB->BSRR = 0x02//GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define LED2_TURN                      GPIOB->ODR ^= 0x02

#define LED3_OFF                       GPIOC->BRR = 0x04 //GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define LED3_ON                        GPIOC->BSRR = 0x04//GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define LED3_TURN                      GPIOC->ODR ^= 0x04

#define LED4_OFF                       GPIOC->BSRR = 0x2000//GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define LED4_ON                        GPIOC->BRR = 0x2000 //GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LED4_TURN                      GPIOC->ODR ^= 0x2000

#define LED5_OFF                       GPIOF->BSRR = 0x010//GPIO_ResetBits(GPIOF,GPIO_Pin_4)
#define LED5_ON                        GPIOF->BRR = 0x010 //GPIO_SetBits(GPIOF,GPIO_Pin_4)
#define LED5_TURN                      GPIOF->ODR ^= 0x010

#define LED6_OFF                       GPIOF->BSRR = 0x020//GPIO_ResetBits(GPIOF,GPIO_Pin_4)
#define LED6_ON                        GPIOF->BRR = 0x020//GPIO_SetBits(GPIOF,GPIO_Pin_4)
#define LED6_TURN                      GPIOF->ODR ^= 0x020


void LED_Init(void);

#endif
