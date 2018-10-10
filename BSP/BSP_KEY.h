
#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f0xx.h"

#define PC6IN                      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)
#define PC7IN                      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
#define PC8IN                      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)
#define PC9IN                      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)
#define PC12IN                     GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)

void KEY_Init(void);
void KEY_EXTI_Init(void);

#endif
