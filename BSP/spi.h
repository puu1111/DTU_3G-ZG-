#ifndef __SPI_H
#define __SPI_H
#include "stm32f0xx.h"
#include "stm32f0xx_spi.h"


/**
  * @brief  SD SPI Interface pins
  */
#define SD_SPI                           SPI1
#define SD_SPI_CLK                       RCC_APB2Periph_SPI1

#define SD_SPI_SCK_PIN                   GPIO_Pin_3                /* PA.05 */
#define SD_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOA */
#define SD_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define SD_SPI_SCK_SOURCE                GPIO_PinSource3
#define SD_SPI_SCK_AF                    GPIO_AF_0


 #define SD_SPI_MISO_PIN                 GPIO_Pin_4                  /* PA.06 */
 #define SD_SPI_MISO_GPIO_PORT           GPIOB                      /* GPIOA */
 #define SD_SPI_MISO_GPIO_CLK            RCC_AHBPeriph_GPIOB
 #define SD_SPI_MISO_SOURCE              GPIO_PinSource4
 #define SD_SPI_MISO_AF                  GPIO_AF_0

#define SD_SPI_MOSI_PIN                  GPIO_Pin_5                 /* PA.07 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOB                     /* GPIOA */
#define SD_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SD_SPI_MOSI_SOURCE               GPIO_PinSource5
#define SD_SPI_MOSI_AF                   GPIO_AF_0

#define TM7706_SPI                           SPI2
#define TM7706_SPI_CLK                       RCC_APB1Periph_SPI2

#define TM7706_SPI_SCK_PIN                   GPIO_Pin_13                /* PA.05 */
#define TM7706_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOA */
#define TM7706_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define TM7706_SPI_SCK_SOURCE                GPIO_PinSource3
#define TM7706_SPI_SCK_AF                    GPIO_AF_0

#define TM7706_SPI_MISO_PIN                 GPIO_Pin_14                  /* PA.06 */
#define TM7706_SPI_MISO_GPIO_PORT           GPIOB                      /* GPIOA */
#define TM7706_SPI_MISO_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define TM7706_SPI_MISO_SOURCE              GPIO_PinSource4
#define TM7706_SPI_MISO_AF                  GPIO_AF_0

#define TM7706_SPI_MOSI_PIN                  GPIO_Pin_15                 /* PA.07 */
#define TM7706_SPI_MOSI_GPIO_PORT            GPIOB                     /* GPIOA */
#define TM7706_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define TM7706_SPI_MOSI_SOURCE               GPIO_PinSource5
#define TM7706_SPI_MOSI_AF                   GPIO_AF_0

//#define SD_CS_PIN                        GPIO_Pin_12                  /* PF.05 */
//#define SD_CS_GPIO_PORT                  GPIOB                       /* GPIOF */
//#define SD_CS_GPIO_CLK                   RCC_AHBPeriph_GPIOB
		  	    													  
void SPIx_Init(void);			 //初始化SPI口
void SPI2_Init(void);
void SPIx_SetSpeed(uint8_t SpeedSet); //设置SPI速度   
uint8_t SPIx_ReadWriteByte(uint8_t TxData);//SPI总线读写一个字节
void SPI2_SetSpeed(uint8_t SpeedSet); //设置SPI速度   
uint8_t SPI2_WriteByte(uint8_t TxData);//SPI总线读写一个字节
uint8_t SPI2_ReadByte(uint8_t *RxData);//SPI总线读写一个字节		 
#endif

