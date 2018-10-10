/**
  ******************************************************************************
  * @file    stm320518_eval.c
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    16-January-2014
  * @brief   This file provides firmware functions to manage Leds, push-buttons,
  *          COM ports, SD card on SPI and temperature sensor (LM75) available on
  *          STM320518-RTU evaluation board from STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "stm320xx8_rtu.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_RTU
  * @{
  */

/** @addtogroup STM320518_RTU
  * @{
  */

/** @defgroup STM320518_RTU_LOW_LEVEL
  * @brief This file provides firmware functions to manage Leds, push-buttons,
  *        COM ports, SD card on SPI and temperature sensor (LM75) available on
  *        STM320518-RTU evaluation board from STMicroelectronics.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT,
                                 LED4_GPIO_PORT, LED5_GPIO_PORT, LED6_GPIO_PORT,
                                 LED7_GPIO_PORT,RST_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN,
                                 LED4_PIN, LED5_PIN, LED6_PIN, LED7_PIN ,RST_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK,
                                 LED4_GPIO_CLK, LED5_GPIO_CLK, LED6_GPIO_CLK,
                                 LED7_GPIO_CLK,RST_CLK};

uint8_t USART_RX_Buffer[20];
USART_TypeDef* COM_USART[COMn] = {RTU_COM_2, RTU_COM_3};

GPIO_TypeDef* COM_TX_PORT[COMn] = {RTU_COM_2_TX_GPIO_PORT, RTU_COM_3_TX_GPIO_PORT};

GPIO_TypeDef* COM_RX_PORT[COMn] = {RTU_COM_2_RX_GPIO_PORT, RTU_COM_3_RX_GPIO_PORT};

GPIO_TypeDef* COM_DE_PORT[COMn] = {RTU_COM_2_RTS_GPIO_PORT, RTU_COM_3_RTS_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {RTU_COM_2_CLK, RTU_COM_3_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {RTU_COM_2_TX_GPIO_CLK, RTU_COM_3_TX_GPIO_CLK};

const uint32_t COM_RX_PORT_CLK[COMn] = {RTU_COM_2_RX_GPIO_CLK, RTU_COM_3_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {RTU_COM_2_TX_PIN, RTU_COM_3_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {RTU_COM_2_RX_PIN, RTU_COM_3_RX_PIN};

const uint16_t COM_DE_PIN[COMn] = {RTU_COM_2_RTS_PIN, RTU_COM_3_RTS_PIN};

const uint8_t COM_TX_PIN_SOURCE[COMn] = {RTU_COM_2_TX_SOURCE, RTU_COM_3_TX_SOURCE};

const uint8_t COM_RX_PIN_SOURCE[COMn] = {RTU_COM_2_RX_SOURCE, RTU_COM_3_RX_SOURCE};

const uint8_t COM_DE_PIN_SOURCE[COMn] = {RTU_COM_2_RTS_SOURCE, RTU_COM_3_RTS_SOURCE};

const uint8_t COM_TX_AF[COMn] = {RTU_COM_2_TX_AF, RTU_COM_3_TX_AF};

const uint8_t COM_RX_AF[COMn] = {RTU_COM_2_RX_AF, RTU_COM_3_RX_AF};

const uint8_t COM_DE_AF[COMn] = {RTU_COM_2_RTS_AF, RTU_COM_3_RTS_AF};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup STM320518_RTU_LOW_LEVEL_Private_Functions
  * @{
  */

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *          This parameter can be one of following parameters:
  *            @arg LED1
  *            @arg LED2
  *            @arg LED3
  *            @arg LED4
  * @retval None
  */
void STM_RTU_LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
  GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *          This parameter can be one of following parameters:
  *            @arg LED1
  *            @arg LED2
  *            @arg LED3
  *            @arg LED4
  * @retval None
  */
void STM_RTU_LEDOff(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BRR = GPIO_PIN[Led];
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *          This parameter can be one of following parameters:
  *            @arg LED1
  *            @arg LED2
  *            @arg LED3
  *            @arg LED4
  * @retval None
  */
void STM_RTU_LEDOn(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *          This parameter can be one of following parameters:
  *            @arg LED1
  *            @arg LED2
  *            @arg LED3
  *            @arg LED4
  * @retval None
  */
void STM_RTU_LEDToggle(Led_TypeDef Led)
{
  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}

void Uart_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,uint32_t cpar,uint32_t *cmar,uint16_t cndtr,uint32_t cdir)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA_CHx);  	//USART3_TX ~ CHANNEL2
	DMA_InitStructure.DMA_PeripheralBaseAddr =cpar;	//USART3->TDR
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)cmar;              
	DMA_InitStructure.DMA_DIR = cdir;	//usart3 tx_dr :source of data
	DMA_InitStructure.DMA_BufferSize=cndtr;         //one byte sizeof(USART3_RX_Buffer) this is DMA1_Channel2->CNDTR;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	//warning: this is important ,attention please !!!!
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA_CHx, &DMA_InitStructure);
	
	//===========================================================================================// 
	
	//DMA_ClearFlag(DMA1_FLAG_GL3);
	//USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(DMA_CHx,DMA_IT_TC,ENABLE);
	
	//DMA_Cmd(DMA_CHx, ENABLE);
	//DMA_Cmd(DMA1_Channel3, ENABLE);
}	

/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *          This parameter can be one of following parameters:
  *            @arg COM_2
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *         contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void STM_RTU_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct, FunctionalState RS485EN)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_DeInit(COM_USART[COM]);
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM]|RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable USART clock */
  ///if(COM == COM_2)
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
//  else
//    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);

  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

  if(RS485EN == ENABLE)
  {
    /* Connect PXx to USARTx_De */
    GPIO_PinAFConfig(COM_DE_PORT[COM], COM_DE_PIN_SOURCE[COM], COM_DE_AF[COM]);
  }

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  if(RS485EN == ENABLE)
  {
    /* Configure USART Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = COM_DE_PIN[COM];
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(COM_DE_PORT[COM], &GPIO_InitStructure);
  }

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);

#ifdef USE_OVERSAMPLING_8
  #define OVERSAMPLING_VALUE 0x08
  USART_OverSampling8Cmd(COM_USART[COM], ENABLE);
#else
  #define OVERSAMPLING_VALUE 0x100
#endif

  if(RS485EN == ENABLE)
  {
    /* Enable driver enable mode */
    USART_DECmd(COM_USART[COM], ENABLE);

    /* Configure DE assertion time */
    USART_SetDEAssertionTime(COM_USART[COM], OVERSAMPLING_VALUE);

    /* Configure DE deassertion time */
    USART_SetDEDeassertionTime(COM_USART[COM], OVERSAMPLING_VALUE);

    /* Configure polarity of DE */
    USART_DEPolarityConfig(COM_USART[COM], USART_DEPolarity_High);
  }

  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
	USART_DMACmd(COM_USART[COM], USART_DMAReq_Rx, ENABLE);
  USART_DMACmd(COM_USART[COM], USART_DMAReq_Tx, ENABLE);	
}

/**
  * @brief  Configures NVIC.
  * @param  COM: Specifies the COM port to be configured.
  *          This parameter can be one of following parameters:
  *            @arg COM_2
  * @retval None
  */
void STM_RTU_NVICInit(IRQn_Type irq,unsigned char pri)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = irq;
  NVIC_InitStructure.NVIC_IRQChannelPriority = pri;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  DeInitializes the SPI interface.
  * @param  None
  * @retval None
  */
void SD_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  SPI_Cmd(SD_SPI, DISABLE); /* SD_SPI disable */
  SPI_I2S_DeInit(SD_SPI);   /* DeInitializes the SD_SPI */

  /* SD_SPI Periph clock disable */
  RCC_APB2PeriphClockCmd(SD_SPI_CLK, DISABLE);

  /* Configure SD_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_MISO_PIN;
  GPIO_Init(SD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_MOSI_PIN;
  GPIO_Init(SD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI_CS_PIN pin: SD Card CS pin */
  GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;
  GPIO_Init(SD_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
  GPIO_InitStructure.GPIO_Pin = SD_DETECT_PIN;
  GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Initializes the SPI and GPIOs resources used to drive the uSD card
  * @param  None
  * @retval None
  */
void SD_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  /* SD_SPI_CS_GPIO, SD_SPI_MOSI_GPIO, SD_SPI_MISO_GPIO, SD_SPI_DETECT_GPIO
       and SD_SPI_SCK_GPIO Periph clock enable */
  RCC_AHBPeriphClockCmd(SD_CS_GPIO_CLK | SD_SPI_MOSI_GPIO_CLK | SD_SPI_MISO_GPIO_CLK |
                        SD_SPI_SCK_GPIO_CLK | SD_DETECT_GPIO_CLK, ENABLE);

  /* SD_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(SD_SPI_CLK, ENABLE);

  /* Configure SD_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(SD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_MISO_PIN;
  GPIO_Init(SD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = SD_SPI_MOSI_PIN;
  GPIO_Init(SD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI_CS_PIN pin: SD Card CS pin */
  GPIO_InitStructure.GPIO_Pin = SD_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(SD_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
  GPIO_InitStructure.GPIO_Pin = SD_DETECT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_InitStructure);

  /* Connect PXx to SD_SPI_SCK */
  GPIO_PinAFConfig(SD_SPI_SCK_GPIO_PORT, SD_SPI_SCK_SOURCE, SD_SPI_SCK_AF);

  /* Connect PXx to SD_SPI_MISO */
  GPIO_PinAFConfig(SD_SPI_MISO_GPIO_PORT, SD_SPI_MISO_SOURCE, SD_SPI_MISO_AF);

  /* Connect PXx to SD_SPI_MOSI */
  GPIO_PinAFConfig(SD_SPI_MOSI_GPIO_PORT, SD_SPI_MOSI_SOURCE, SD_SPI_MOSI_AF);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SD_SPI);

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SD_SPI, &SPI_InitStructure);

  SPI_RxFIFOThresholdConfig(SD_SPI, SPI_RxFIFOThreshold_QF);

  SPI_Cmd(SD_SPI, ENABLE); /* SD_SPI enable */
}

/**
  * @brief  DeInitializes peripherals used by the LM75 driver.
  * @param  None
  * @retval None
  */
void LM75_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Disable LM75_I2C */
  I2C_Cmd(LM75_I2C, DISABLE);

  /* DeInitializes the LM75_I2C */
  I2C_DeInit(LM75_I2C);

  /* LM75_I2C Periph clock disable */
  RCC_APB1PeriphClockCmd(LM75_I2C_CLK, DISABLE);

  /* Configure LM75_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = LM75_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(LM75_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /* Configure LM75_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = LM75_I2C_SDA_PIN;
  GPIO_Init(LM75_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

  /* Configure LM75_I2C pin: SMBUS ALERT */
  GPIO_InitStructure.GPIO_Pin = LM75_I2C_SMBUSALERT_PIN;
  GPIO_Init(LM75_I2C_SMBUSALERT_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Initializes the I2C source clock and IOs used to drive the LM75
  * @param  None
  * @retval None
  */
void LM75_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* LM75_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(LM75_I2C_CLK, ENABLE);

  /* Configure the I2C clock source. The clock is derived from the HSI */
  RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);

  /* LM75_I2C_SCL_GPIO_CLK, LM75_I2C_SDA_GPIO_CLK
       and LM75_I2C_SMBUSALERT_GPIO_CLK Periph clock enable */
  RCC_AHBPeriphClockCmd(LM75_I2C_SCL_GPIO_CLK | LM75_I2C_SDA_GPIO_CLK |
                        LM75_I2C_SMBUSALERT_GPIO_CLK, ENABLE);

  /* Connect PXx to I2C_SCL */
  GPIO_PinAFConfig(LM75_I2C_SCL_GPIO_PORT, LM75_I2C_SCL_SOURCE, LM75_I2C_SCL_AF);

  /* Connect PXx to I2C_SDA */
  GPIO_PinAFConfig(LM75_I2C_SDA_GPIO_PORT, LM75_I2C_SDA_SOURCE, LM75_I2C_SDA_AF);

  /* Connect PXx to I2C_SMBUSALER */
  GPIO_PinAFConfig(LM75_I2C_SMBUSALERT_GPIO_PORT, LM75_I2C_SMBUSALERT_SOURCE, LM75_I2C_SMBUSALERT_AF);

  /* Configure LM75_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = LM75_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(LM75_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /* Configure LM75_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = LM75_I2C_SDA_PIN;
  GPIO_Init(LM75_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

  /* Configure LM75_I2C pin: SMBUS ALERT */
  GPIO_InitStructure.GPIO_Pin = LM75_I2C_SMBUSALERT_PIN;
  GPIO_Init(LM75_I2C_SMBUSALERT_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* sEE_I2C Peripheral Disable */
  I2C_Cmd(sEE_I2C, DISABLE);

  /* sEE_I2C DeInit */
  I2C_DeInit(sEE_I2C);

  /* sEE_I2C Periph clock disable */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, DISABLE);

  /* GPIO configuration */
  /* Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /* Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Initializes the I2C source clock and IOs used to drive the EEPROM.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Configure the I2C clock source. The clock is derived from the HSI */
  RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);

  /* sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_AHBPeriphClockCmd(sEE_I2C_SCL_GPIO_CLK | sEE_I2C_SDA_GPIO_CLK, ENABLE);

  /* sEE_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, ENABLE);

  /* Connect PXx to I2C_SCL*/
  GPIO_PinAFConfig(sEE_I2C_SCL_GPIO_PORT, sEE_I2C_SCL_SOURCE, sEE_I2C_SCL_AF);

  /* Connect PXx to I2C_SDA*/
  GPIO_PinAFConfig(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_SOURCE, sEE_I2C_SDA_AF);

  /* GPIO configuration */
  /* Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /* Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}

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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
