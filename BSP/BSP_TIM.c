//=============================================================================
//文件名称：BSP_BEEP.c
//功能概要：蜂鸣器驱动文件
//版权所有：源地工作室www.vcc-gnd.com
//淘宝网店：http://vcc-gnd.taobao.com
//更新时间：2013-11-20
//调试方式：J-Link OB ARM SWD
//=============================================================================
#include "TIM.h"

void BEEP_Init(void)
	{
   	

		GPIO_InitTypeDef         GPIO_InitStructure;
		
		TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

		
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 		

 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);	
		
		
 	TIM_BaseInitStructure.TIM_Period = 500-1;
    TIM_BaseInitStructure.TIM_Prescaler = 72-1;
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
		
		//启用ARR的影子寄存器（直到产生更新事件才更改设置）
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    //TIM1_OC1模块设置（设置1通道占空比）
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 120;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //启用CCR1寄存器的影子寄存器（直到产生更新事件才更改设置）
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    //TIM2_OC2模块设置（设置2通道占空比）
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 680;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    //启用CCR2寄存器的影子寄存器（直到产生更新事件才更改设置）
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
		
		
		
		//死区设置
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime = 0x90; //这里调整死区大小0-0xff
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
   
    //TIM1开启
    //TIM_Cmd(TIM1, ENABLE);
    //TIM1_OC通道输出PWM（一定要加）
    TIM_CtrlPWMOutputs(TIM1, ENABLE);	
		
  }