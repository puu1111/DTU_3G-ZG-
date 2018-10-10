//=============================================================================
//文件名称：bsp_led.c
//功能概要：LED驱动文件
//版权所有：源地工作室www.vcc-gnd.com
//淘宝网店：http://vcc-gnd.taobao.com
//更新时间：2013-11-20
//调试方式：J-Link OB ARM SWD
//=============================================================================

#include "BSP_LED.h"

void LED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE);
 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
}






