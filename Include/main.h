/**
  ******************************************************************************
  * @file    USART/USART_HyperTerminalInterrupt/main.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <semphr.h>
#include "stm320xx8_rtu.h"
#include "3gmodule.h"
#include "datapacket.h"
#include "mbport.h"
#include "timertask.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define LED1_OFF                       GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define LED1_ON                        GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define LED1_TURN                      GPIOC->ODR ^= 0x0100

#define LED2_OFF                       GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define LED2_ON                        GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define LED2_TURN                      GPIOC->ODR ^= 0x0200

typedef enum
{
	  eInit_None=0,
    eInit_Monitor = 0x01,//心跳
    eInit_Config = 0x2,//设备注册
	  eInit_4GConfig = 0x03,//发送
	  eInit_Finish = 0x04,//事件
} Init_Pro;

extern uint8_t SYS_Init_Flag;
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void bsp_DelayMS(uint16_t ms) ;

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
