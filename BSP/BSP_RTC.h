
#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#include "stm32f0xx.h"
#include <stdio.h>

	#define BKP_VALUE    0x32F0
	
  extern  __IO uint32_t AsynchPrediv, SynchPrediv ;
	
	
	extern  RTC_TimeTypeDef   RTC_TimeStructure;
	extern  RTC_DateTypeDef RTC_DateStructure;
  extern  RTC_InitTypeDef   RTC_InitStructure;
  extern  RTC_AlarmTypeDef  RTC_AlarmStructure;


	void Calendar(void);
	void RTC_Configuration(void);
	void RTC_DateShow(void);
	void RTC_AlarmShow(void);
	void RTC_DateRegulate(void);
	void RTC_TimeRegulate(void);
	void RTC_AlarmRegulate(void);
	void RTC_Config(void);
	void RTC_TimeShow(void);
	
	uint8_t USART_Scanf(uint32_t value);

#endif
