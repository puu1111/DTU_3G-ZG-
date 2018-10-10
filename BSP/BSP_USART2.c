//=============================================================================
//文件名称：bsp_usart2.c
//功能概要：串口1驱动文件
//版权所有：源地工作室www.vcc-gnd.com
//淘宝网店：http://vcc-gnd.taobao.com
//更新时间：2013-11-20
//调试方式：J-Link OB ARM SWD
//=============================================================================

#include "BSP_USART2.h"


void USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //使能GPIOA的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);//配置PA2成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);//配置PA3成第二功能引脚  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	/* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = 115200;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure);		
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);           //使能接收中断
	USART_Cmd(USART2, ENABLE);                             //使能USART1
	
	/* USART1的NVIC中断配置 */
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
				
}

//=============================================================================
//文件名称：
//功能概要：USART2中断函数
//参数说明：无
//函数返回：无
//=============================================================================
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
   USART_SendData(USART2,USART_ReceiveData(USART2));
	 while (USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
	}
			
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
