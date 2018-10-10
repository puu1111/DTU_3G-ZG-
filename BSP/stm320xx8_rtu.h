/**
  ******************************************************************************
  * @file    stm320518_eval.h
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    16-January-2014
  * @brief   This file contains definitions for STM320518_RTU's Leds, push-buttons
  *          and COM ports hardware resources.
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
#ifndef __STM320518_RTU_H
#define __STM320518_RTU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_RTU
  * @{
  */

/** @addtogroup STM320518_RTU
  * @{
  */

/** @addtogroup STM320518_RTU_LOW_LEVEL
  * @{
  */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3,
  LED5 = 4,
  LED6 = 5,
  LED7 = 6,
  RST = 7,
} Led_TypeDef;

typedef enum
{
  COM_2 = 0,
  COM_3 = 1
} COM_TypeDef;

/** @defgroup STM320518_RTU_LOW_LEVEL_Exported_Constants
  * @{
  */

/**
  * @brief  Define for STM320518_RTU board
  */
#if !defined (USE_STM320518_RTU)
 #define USE_STM320518_RTU
#endif

/**
 * @brief Select the revision A or B(default) of the RTU board used.
 */
#if !defined (STM320518_RTU_REVB)
 #define STM320518_RTU_REVB
#endif

#if !defined (STM320518_RTU_REVA)
 /* #define STM320518_RTU_REVA */
#endif


/** @addtogroup STM320518_RTU_LOW_LEVEL_LED
  * @{
  */
#define LEDn                            8

#define LED1_PIN                         GPIO_Pin_6
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOC

#define LED2_PIN                         GPIO_Pin_7
#define LED2_GPIO_PORT                   GPIOC
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOC

#define LED3_PIN                         GPIO_Pin_9
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOC

#define LED4_PIN                         GPIO_Pin_8
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK                    RCC_AHBPeriph_GPIOC

#define LED5_PIN                         GPIO_Pin_8
#define LED5_GPIO_PORT                   GPIOA
#define LED5_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define LED6_PIN                         GPIO_Pin_9
#define LED6_GPIO_PORT                   GPIOA
#define LED6_GPIO_CLK                    RCC_AHBPeriph_GPIOA

#define LED7_PIN                         GPIO_Pin_2
#define LED7_GPIO_PORT                   GPIOA
#define LED7_GPIO_CLK                    RCC_AHBPeriph_GPIOB

#define RST_PIN                         GPIO_Pin_5
#define RST_PORT                         GPIOA
#define RST_CLK                         RCC_AHBPeriph_GPIOA

/**
  * @}
  */

/** @addtogroup STM320308_RTU_LOW_LEVEL_COM
  * @{
  */
#define COMn                             2

/**
 * @brief Definition for COM port1, connected to USART1
 */
#define RTU_COM_2                        USART2
#define RTU_COM_2_CLK                    RCC_APB1Periph_USART2

#define RTU_COM_2_TX_PIN                 GPIO_Pin_2
#define RTU_COM_2_TX_GPIO_PORT           GPIOA
#define RTU_COM_2_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define RTU_COM_2_TX_SOURCE              GPIO_PinSource2
#define RTU_COM_2_TX_AF                  GPIO_AF_1

#define RTU_COM_2_RX_PIN                 GPIO_Pin_3
#define RTU_COM_2_RX_GPIO_PORT           GPIOA
#define RTU_COM_2_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define RTU_COM_2_RX_SOURCE              GPIO_PinSource3
#define RTU_COM_2_RX_AF                  GPIO_AF_1

#define RTU_COM_2_CTS_PIN                GPIO_Pin_11
#define RTU_COM_2_CTS_GPIO_PORT          GPIOA
#define RTU_COM_2_CTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define RTU_COM_2_CTS_SOURCE             GPIO_PinSource11
#define RTU_COM_2_CTS_AF                 GPIO_AF_1

#define RTU_COM_2_RTS_PIN                GPIO_Pin_1
#define RTU_COM_2_RTS_GPIO_PORT          GPIOA
#define RTU_COM_2_RTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define RTU_COM_2_RTS_SOURCE             GPIO_PinSource1
#define RTU_COM_2_RTS_AF                 GPIO_AF_1

#define RTU_COM_2_IRQn                   USART2_IRQn

/**
 * @brief Definition for COM port2, connected to USART2
 */
#define RTU_COM_3                        USART3
#define RTU_COM_3_CLK                    RCC_APB1Periph_USART3

#define RTU_COM_3_TX_PIN                 GPIO_Pin_10
#define RTU_COM_3_TX_GPIO_PORT           GPIOC
#define RTU_COM_3_TX_GPIO_CLK            RCC_AHBPeriph_GPIOC
#define RTU_COM_3_TX_SOURCE              GPIO_PinSource10
#define RTU_COM_3_TX_AF                  GPIO_AF_1

#define RTU_COM_3_RX_PIN                 GPIO_Pin_11
#define RTU_COM_3_RX_GPIO_PORT           GPIOC
#define RTU_COM_3_RX_GPIO_CLK            RCC_AHBPeriph_GPIOC
#define RTU_COM_3_RX_SOURCE              GPIO_PinSource11
#define RTU_COM_3_RX_AF                  GPIO_AF_1

#define RTU_COM_3_CTS_PIN                GPIO_Pin_6
#define RTU_COM_3_CTS_GPIO_PORT          GPIOA
#define RTU_COM_3_CTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define RTU_COM_3_CTS_SOURCE             GPIO_PinSource6
#define RTU_COM_3_CTS_AF                 GPIO_AF_4

#define RTU_COM_3_RTS_PIN                GPIO_Pin_1
#define RTU_COM_3_RTS_GPIO_PORT          GPIOC
#define RTU_COM_3_RTS_GPIO_CLK           RCC_AHBPeriph_GPIOC
#define RTU_COM_3_RTS_SOURCE             GPIO_PinSource1
#define RTU_COM_3_RTS_AF                 GPIO_AF_1

#define RTU_COM_3_IRQn                   USART3_IRQn

/**
  * @}
  */

/** @addtogroup STM320518_RTU_LOW_LEVEL_SD_SPI
  * @{
  */
/**
  * @brief  SD SPI Interface pins
  */
#define SD_SPI                           SPI1
#define SD_SPI_CLK                       RCC_APB2Periph_SPI1

#define SD_SPI_SCK_PIN                   GPIO_Pin_3                  /* PA.05 */
#define SD_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOA */
#define SD_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define SD_SPI_SCK_SOURCE                GPIO_PinSource3
#define SD_SPI_SCK_AF                    GPIO_AF_0

//#if defined (STM320518_RTU_REVA)
// #define SD_SPI_MISO_PIN                 GPIO_Pin_6                  /* PA.06 */
// #define SD_SPI_MISO_GPIO_PORT           GPIOA                       /* GPIOA */
// #define SD_SPI_MISO_GPIO_CLK            RCC_AHBPeriph_GPIOA
// #define SD_SPI_MISO_SOURCE              GPIO_PinSource6
// #define SD_SPI_MISO_AF                  GPIO_AF_0
//#elif defined (STM320518_RTU_REVB)
// #define SD_SPI_MISO_PIN                 GPIO_Pin_4                  /* PB.04 */
// #define SD_SPI_MISO_GPIO_PORT           GPIOB                       /* GPIOB */
// #define SD_SPI_MISO_GPIO_CLK            RCC_AHBPeriph_GPIOB
// #define SD_SPI_MISO_SOURCE              GPIO_PinSource4
// #define SD_SPI_MISO_AF                  GPIO_AF_0
//#endif /* STM320518_RTU_REVA*/

 #define SD_SPI_MISO_PIN                 GPIO_Pin_4                  /* PB.04 */
 #define SD_SPI_MISO_GPIO_PORT           GPIOB                       /* GPIOB */
 #define SD_SPI_MISO_GPIO_CLK            RCC_AHBPeriph_GPIOB
 #define SD_SPI_MISO_SOURCE              GPIO_PinSource4
 #define SD_SPI_MISO_AF                  GPIO_AF_0

#define SD_SPI_MOSI_PIN                  GPIO_Pin_5                  /* PA.07 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOA */
#define SD_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SD_SPI_MOSI_SOURCE               GPIO_PinSource5
#define SD_SPI_MOSI_AF                   GPIO_AF_0

#define SD_CS_PIN                        GPIO_Pin_15                  /* PF.05 */
#define SD_CS_GPIO_PORT                  GPIOA                       /* GPIOF */
#define SD_CS_GPIO_CLK                   RCC_AHBPeriph_GPIOA

#define SD_DETECT_PIN                    GPIO_Pin_15                 /* PB.15 */
#define SD_DETECT_EXTI_LINE              EXTI_Line15
#define SD_DETECT_EXTI_PIN_SOURCE        EXTI_PinSource15
#define SD_DETECT_GPIO_PORT              GPIOB                       /* GPIOB */
#define SD_DETECT_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SD_DETECT_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOB
#define SD_DETECT_EXTI_IRQn              EXTI4_15_IRQn

/**
  * @}
  */


/** @addtogroup STM320518_RTU_LOW_LEVEL_TSENSOR_I2C
  * @{
  */
/**
  * @brief  LM75 Temperature Sensor I2C Interface pins
  */
#define LM75_I2C                         I2C1
#define LM75_I2C_CLK                     RCC_APB1Periph_I2C1

#define LM75_I2C_SCL_PIN                 GPIO_Pin_6                  /* PB.06 */
#define LM75_I2C_SCL_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SCL_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define LM75_I2C_SCL_SOURCE              GPIO_PinSource6
#define LM75_I2C_SCL_AF                  GPIO_AF_1

#define LM75_I2C_SDA_PIN                 GPIO_Pin_7                  /* PB.07 */
#define LM75_I2C_SDA_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SDA_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define LM75_I2C_SDA_SOURCE              GPIO_PinSource7
#define LM75_I2C_SDA_AF                  GPIO_AF_1

#define LM75_I2C_SMBUSALERT_PIN          GPIO_Pin_5                  /* PB.05 */
#define LM75_I2C_SMBUSALERT_GPIO_PORT    GPIOB                       /* GPIOB */
#define LM75_I2C_SMBUSALERT_GPIO_CLK     RCC_AHBPeriph_GPIOB
#define LM75_I2C_SMBUSALERT_SOURCE       GPIO_PinSource5
#define LM75_I2C_SMBUSALERT_AF           GPIO_AF_3

/**
  * @}
  */

/** @addtogroup STM320518_RTU_LOW_LEVEL_I2C_EE
  * @{
  */
/**
  * @brief  I2C EEPROM Interface pins
  */
#define sEE_I2C                          I2C1
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C1

#define sEE_I2C_SCL_PIN                  GPIO_Pin_6                  /* PB.06 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource6
#define sEE_I2C_SCL_AF                   GPIO_AF_1

#define sEE_I2C_SDA_PIN                  GPIO_Pin_7                  /* PB.07 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource7
#define sEE_I2C_SDA_AF                   GPIO_AF_1

/**
  * @}
  */

/** @defgroup STM320518_RTU_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_RTU_LEDInit(Led_TypeDef Led);
void STM_RTU_LEDOn(Led_TypeDef Led);
void STM_RTU_LEDOff(Led_TypeDef Led);
void STM_RTU_LEDToggle(Led_TypeDef Led);
void STM_RTU_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct, FunctionalState RS485EN);
void STM_RTU_NVICInit(IRQn_Type irq,unsigned char pri);
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void);
void sFLASH_LowLevel_DeInit(void);
void sFLASH_LowLevel_Init(void);
void LM75_LowLevel_DeInit(void);
void LM75_LowLevel_Init(void);
void sEE_LowLevel_DeInit(void);
void sEE_LowLevel_Init(void);
void Uart_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,uint32_t cpar,\
	uint32_t *cmar,uint16_t cndtr,uint32_t cdir);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM320518_RTU_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
