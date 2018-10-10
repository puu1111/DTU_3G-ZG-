/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：SPI.c
 * 描述    ：SPI初始化
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2015-08-20
 * 硬件连接  :SCLK--PA5;MISO---PA6;MOSI--PA7
 * 调试方式：J-Link-OB
********************************************************************************/
#include "spi.h"

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C							  
SPI_InitTypeDef  SPI_InitStructure;


#define TM7706_SPI                           SPI2
#define TM7706_SPI_CLK                       RCC_APB1Periph_SPI2

#define TM7706_SPI_SCK_PIN                   GPIO_Pin_13                /* PA.05 */
#define TM7706_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOA */
#define TM7706_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define TM7706_SPI_SCK_SOURCE                GPIO_PinSource3
#define TM7706_SPI_SCK_AF                    GPIO_AF_0

#define TM7706_SPI_MISO_PIN                 GPIO_Pin_13                  /* PA.06 */
#define TM7706_SPI_MISO_GPIO_PORT           GPIOB                      /* GPIOA */
#define TM7706_SPI_MISO_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define TM7706_SPI_MISO_SOURCE              GPIO_PinSource4
#define TM7706_SPI_MISO_AF                  GPIO_AF_0

#define TM7706_SPI_MOSI_PIN                  GPIO_Pin_15                 /* PA.07 */
#define TM7706_SPI_MOSI_GPIO_PORT            GPIOB                     /* GPIOA */
#define TM7706_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define TM7706_SPI_MOSI_SOURCE               GPIO_PinSource5
#define TM7706_SPI_MOSI_AF                   GPIO_AF_0

void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(TM7706_SPI_SCK_GPIO_CLK | TM7706_SPI_MISO_GPIO_CLK |TM7706_SPI_MOSI_GPIO_CLK , ENABLE);	
  RCC_APB1PeriphClockCmd(TM7706_SPI_CLK, ENABLE);
 
	
	GPIO_PinAFConfig(TM7706_SPI_SCK_GPIO_PORT,TM7706_SPI_SCK_SOURCE,TM7706_SPI_SCK_AF);
	GPIO_PinAFConfig(TM7706_SPI_MISO_GPIO_PORT,TM7706_SPI_MISO_SOURCE,TM7706_SPI_MISO_AF);
	GPIO_PinAFConfig(TM7706_SPI_MOSI_GPIO_PORT,TM7706_SPI_MOSI_SOURCE,TM7706_SPI_MOSI_AF);
	
	GPIO_InitStructure.GPIO_Pin = TM7706_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;/*!< I/O output speed: High 50 MHz */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//上拉
	GPIO_Init(TM7706_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	/*!< 配置 SD_SPI 管脚: MISO */
	GPIO_InitStructure.GPIO_Pin = TM7706_SPI_MISO_PIN;
	GPIO_Init(TM7706_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

	/*!< 配置 SD_SPI 管脚: MOSI */
	GPIO_InitStructure.GPIO_Pin = TM7706_SPI_MOSI_PIN;
	GPIO_Init(TM7706_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
		
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(TM7706_SPI, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
  SPI_RxFIFOThresholdConfig(TM7706_SPI, SPI_RxFIFOThreshold_QF);
	SPI_Cmd(TM7706_SPI, ENABLE); //使能SPI外设
	
	SPIx_ReadWriteByte(0xff);//启动传输

}   

void SPIx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(SD_SPI_MOSI_GPIO_CLK | SD_SPI_MISO_GPIO_CLK |SD_SPI_SCK_GPIO_CLK , ENABLE);	
  RCC_APB2PeriphClockCmd(SD_SPI_CLK, ENABLE);
 
	
	GPIO_PinAFConfig(SD_SPI_SCK_GPIO_PORT,SD_SPI_SCK_SOURCE,SD_SPI_SCK_AF);
	GPIO_PinAFConfig(SD_SPI_MISO_GPIO_PORT,SD_SPI_MISO_SOURCE,SD_SPI_MISO_AF);
	GPIO_PinAFConfig(SD_SPI_MOSI_GPIO_PORT,SD_SPI_MOSI_SOURCE,SD_SPI_MOSI_AF);
	
	GPIO_InitStructure.GPIO_Pin = SD_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;/*!< I/O output speed: High 50 MHz */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//上拉
	GPIO_Init(SD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	/*!< 配置 SD_SPI 管脚: MISO */
	GPIO_InitStructure.GPIO_Pin = SD_SPI_MISO_PIN;
	GPIO_Init(SD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

	/*!< 配置 SD_SPI 管脚: MOSI */
	GPIO_InitStructure.GPIO_Pin = SD_SPI_MOSI_PIN;
	GPIO_Init(SD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
		
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SD_SPI, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
  SPI_RxFIFOThresholdConfig(SD_SPI, SPI_RxFIFOThreshold_QF);
	SPI_Cmd(SD_SPI, ENABLE); //使能SPI外设
	
	SPIx_ReadWriteByte(0xff);//启动传输

}   

 /**
  * @file   SPIx_SetSpeed
  * @brief  速度设置
  * @param  SpeedSet:速度参数
  * @retval 无
  */  
void SPIx_SetSpeed(uint8_t SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
	SPI_Init(SD_SPI, &SPI_InitStructure);
	SPI_Cmd(SD_SPI,ENABLE);
} 

void SPI2_SetSpeed(uint8_t SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
	SPI_Init(TM7706_SPI, &SPI_InitStructure);
	SPI_Cmd(TM7706_SPI,ENABLE);
} 

 /**
  * @file   SPIx_ReadWriteByte
  * @brief  读写一个字节
  * @param  TxData:要写入的字节
  * @retval 读取到的字节
  */

uint8_t SPI2_WriteByte(uint8_t TxData)
{		
		uint8_t retry=0;				 
		while (SPI_I2S_GetFlagStatus(TM7706_SPI, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  	
		SPI_SendData8(TM7706_SPI, TxData); //通过外设SPIx发送一个数据
}

uint8_t SPI2_ReadByte(uint8_t *RxData)
{		
		uint8_t retry=0;				 
		
	
		retry++;
		if(retry>200)*RxData=0;
		  						    
		*RxData=SPI_ReceiveData8(TM7706_SPI); //返回通过SPIx最近接收的数据	

}

uint8_t SPIx_ReadWriteByte(uint8_t TxData)
{		
		uint8_t retry=0;				 
		while (SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
   	
		SPI_SendData8(SD_SPI, TxData); //通过外设SPIx发送一个数据
		retry=0;
		while (SPI_I2S_GetFlagStatus(SD_SPI, SPI_I2S_FLAG_RXNE) == RESET)
		{
		;
		} //检查指定的SPI标志位设置与否:接受缓存非空标志位
		
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
		return SPI_ReceiveData8(SD_SPI); //返回通过SPIx最近接收的数据	

}































