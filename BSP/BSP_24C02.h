
#ifndef __BSP_24C02_H
#define __BSP_24C02_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Private define ------------------------------------------------------------*/
#define SCL_H         GPIOF->BSRR = GPIO_Pin_6	 
#define SCL_L         GPIOF->BRR  = GPIO_Pin_6     
#define SDA_H         GPIOF->BSRR = GPIO_Pin_7	 
#define SDA_L         GPIOF->BRR  = GPIO_Pin_7	
#define SCL_read       GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_6)   
#define SDA_read       GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_7)  	 

#define I2C_PageSize  8  /* 24C02Ã¿Ò³8×Ö½Ú */

#define ADDR_24LC02		0xA0

/* Private function prototypes -----------------------------------------------*/
void I2C_Configuration(void);
FunctionalState I2C_WriteByte(uint8_t SendByte, uint16_t WriteAddress, uint8_t DeviceAddress);
FunctionalState I2C_ReadByte(uint8_t* pBuffer,   uint16_t length,   uint16_t ReadAddress,  uint8_t DeviceAddress);

#endif 

