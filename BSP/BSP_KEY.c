//=============================================================================
//文件名称：bsp_key.c
//功能概要：KEY驱动文件
//版权所有：源地工作室www.vcc-gnd.com
//淘宝网店：http://vcc-gnd.taobao.com
//更新时间：2013-11-20
//调试方式：J-Link OB ARM SWD
//=============================================================================

#include "BSP_KEY.h"
#include "BSP_OLED.h"    	// OLED显示器驱动头文件
#include "BSP_USART1.h"   // USASRT1驱动模块头

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
          
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
}


void KEY_EXTI_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStruct; 
     EXTI_InitTypeDef EXTI_InitStruct;
     NVIC_InitTypeDef NVIC_InitStruct;

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
              
              
     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12; 
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
     GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
     GPIO_Init(GPIOC, &GPIO_InitStruct);

     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9);
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource12);
              
     EXTI_ClearITPendingBit(EXTI_Line6);
     EXTI_ClearITPendingBit(EXTI_Line7);
     EXTI_ClearITPendingBit(EXTI_Line8);
     EXTI_ClearITPendingBit(EXTI_Line9);
     EXTI_ClearITPendingBit(EXTI_Line12);
              
     EXTI_InitStruct.EXTI_Line = EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9|EXTI_Line12;
     EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
     EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; 
     EXTI_InitStruct.EXTI_LineCmd = ENABLE;
     EXTI_Init(&EXTI_InitStruct); 

     NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
     NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStruct);
}



//=============================================================================
//文件名称：main
//功能概要：USART1中断函数
//参数说明：无
//函数返回：int
//=============================================================================
void EXTI4_15_IRQHandler(void)
{
	if ( EXTI_GetITStatus(EXTI_Line6)!= RESET ) 
	{		
		printf("KEY1 按下\r\n");
		OLED_DispStr(64, 1, "KEY1 OK", &tFont12);
		EXTI_ClearITPendingBit(EXTI_Line6);
	}		
	
		if ( EXTI_GetITStatus(EXTI_Line7)!= RESET ) 
	{		
		printf("KEY2 按下\r\n");
		OLED_DispStr(64, 1, "KEY2 OK", &tFont12);
		EXTI_ClearITPendingBit(EXTI_Line7);
	}		
		if ( EXTI_GetITStatus(EXTI_Line8)!= RESET ) 
	{		
		printf("KEY3 按下\r\n");
		OLED_DispStr(64, 1, "KEY3 OK", &tFont12);
		EXTI_ClearITPendingBit(EXTI_Line8);
	}		
		if ( EXTI_GetITStatus(EXTI_Line9)!= RESET ) 
	{		
		printf("KEY4 按下\r\n");
		OLED_DispStr(64, 1, "KEY4 OK", &tFont12);
		EXTI_ClearITPendingBit(EXTI_Line9);
	}	
			if ( EXTI_GetITStatus(EXTI_Line12)!= RESET ) 
	{		
	  printf("JOY 按下\r\n");
		OLED_DispStr(64, 1, "JOY  OK", &tFont12);
		EXTI_ClearITPendingBit(EXTI_Line12);
	}	
	
}
