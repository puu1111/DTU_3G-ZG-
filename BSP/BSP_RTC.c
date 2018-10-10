#include "BSP_RTC.h"

  __IO uint32_t AsynchPrediv=0, SynchPrediv=0 ;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
  RTC_InitTypeDef RTC_InitStructure;
  RTC_AlarmTypeDef  RTC_AlarmStructure;



void Calendar(void)
	
	{
		NVIC_InitTypeDef  NVIC_InitStructure;
		EXTI_InitTypeDef  EXTI_InitStructure;
		
		printf("\n\r如果RTC没有进行第一次设置需要进行配置包括时间、日历和闹钟\n\r");
		if (RTC_ReadBackupRegister(RTC_BKP_DR0) != BKP_VALUE)
		{  
   
     RTC_Config();//RTC初始化配置

    /* 配置RTC数据寄存器和RTC*/
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;    //
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;     //
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//选择小时格式
   
    
    if (RTC_Init(&RTC_InitStructure) == ERROR)//检测RTC是否初始化成功
    {
      printf("\n\r RTC初始化失败，晶振有问题 \n\r");
    }

    /* 进行时间配置*/
		RTC_DateRegulate();
    RTC_TimeRegulate(); 
		}
  else
		{
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\n出现一次电源复位\n\r");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\n 出现一次外部复位\n\r");
    }

    printf("\n\r没有必要设置RTC\n\r");
    
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Clear the RTC Alarm Flag */
    RTC_ClearFlag(RTC_FLAG_ALRAF);

    /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
    EXTI_ClearITPendingBit(EXTI_Line17);

    /* Display the RTC Time and Alarm */
//	RTC_DateShow();
    RTC_TimeShow();
    RTC_AlarmShow();

	}
	  /* RTC Alarm A Interrupt Configuration */
  /* EXTI configuration *********************************************************/
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}	

void RTC_Config(void)
{
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR(电源时钟和备份)的时钟

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);//取消备份写保护
    
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);//复位备份区域，开启外部低速振荡器

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)//等待时钟复位
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择RTC时钟
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);//使能RTC时钟

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();//等待RTC  APB寄存器 同步
}

void RTC_DateRegulate(void)
{
  uint32_t dmp_yy=0xFF,dmp_mm=0xFF,dmp_dd=0xFF,dmp_ww=0xFF;

	printf("\n\r==============Date Settings=====================================\n\r");
	printf("\n\r设置(00-99)年份:\n\r");
	while (dmp_yy == 0xFF)
	{
	  dmp_yy=USART_Scanf(99);
		RTC_DateStructure.RTC_Year=dmp_yy;
	}
	  printf("  %0.2d\n\r", dmp_yy);
	
	printf("\n\r设置月份:\n\r");
	while (dmp_mm == 0xFF)
	{
	  dmp_mm=USART_Scanf(12);
		RTC_DateStructure.RTC_Month=dmp_mm;
	}
	  printf("  %0.2d\n\r", dmp_mm);
	
	printf("\n\r设置日期:\n\r");
	while (dmp_dd == 0xFF)
	{
	  dmp_dd=USART_Scanf(31);
		RTC_DateStructure.RTC_Date=dmp_dd;
	}
	  printf("  %0.2d\n\r", dmp_dd);
	
		printf("\n\r设置星期几:\n\r");
	while (dmp_ww == 0xFF)
	{
	  dmp_ww=USART_Scanf(7);
		RTC_DateStructure.RTC_WeekDay=dmp_ww;
	}
	  printf("  %0.2d\n\r", dmp_ww);
	
	 if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
  {
    printf("\n\r>> !! RTC Set Date failed. !! <<\n\r");
  } 
  else
  {
    printf("\n\r>> !! RTC Set Date success. !! <<\n\r");
    RTC_DateShow();
    /* Indicator for the RTC configuration */
    RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_VALUE);
  }


}
void RTC_TimeRegulate(void)


{	
  uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

  printf("\n\r时间设置\n\r");
  RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
  printf("\n\r设置小时:\n\r");
  while (tmp_hh == 0xFF)
  {
    tmp_hh = USART_Scanf(23);
    RTC_TimeStructure.RTC_Hours = tmp_hh;
  }
  printf("  %0.2d\n\r", tmp_hh);
  
  printf("\n\r设置分钟:\n\r");
  while (tmp_mm == 0xFF)
  {
    tmp_mm = USART_Scanf(59);
    RTC_TimeStructure.RTC_Minutes = tmp_mm;
  }
  printf("  %0.2d\n\r", tmp_mm);
  
  printf("\n\r设置秒:\n\r");
  while (tmp_ss == 0xFF)
  {
    tmp_ss = USART_Scanf(59);
    RTC_TimeStructure.RTC_Seconds = tmp_ss;
  }
  printf("  %0.2d\n\r", tmp_ss);

  /* 检查时间设置是否成功*/
  if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
  {
    printf("\n\r>> !! RTC时间设置失败。!! <<\n\r");
  } 
  else
  {
    printf("\n\r>> !! RTC时间设置成功。!! <<\n\r");
    RTC_TimeShow();//打印时间
    /* Indicator for the RTC configuration */
    RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_VALUE);//指定RTC配置
  }

  tmp_hh = 0xFF;
  tmp_mm = 0xFF;
  tmp_ss = 0xFF;

  /* 失能闹钟_A */
  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

  printf("\n\r闹钟设置\n\r");
  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
  printf("\n\r设置闹钟小时:\n\r");
  while (tmp_hh == 0xFF)
  {
    tmp_hh = USART_Scanf(23);
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = tmp_hh;
  }
  printf("  %0.2d\n\r", tmp_hh);
  
  printf("\n\r设置闹钟分钟:\n\r");
  while (tmp_mm == 0xFF)
  {
    tmp_mm = USART_Scanf(59);
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = tmp_mm;
  }
  printf("  %0.2d\n\r", tmp_mm);
  
  printf("\n\r设置闹钟秒数:\n\r");
  while (tmp_ss == 0xFF)
  {
    tmp_ss = USART_Scanf(59);
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = tmp_ss;
  }
  printf("  %0.2d", tmp_ss);

  /* Set the Alarm A */
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

  /* Configure the RTC Alarm A register */
  RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
  printf("\n\r>> !! RTC Set Alarm success. !! <<\n\r");
  RTC_AlarmShow();

  /* 使能RTC闹钟中断 */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
   
  /* 使能alarm  A */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}




void RTC_DateShow(void)
{

	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  printf("\n\r当前日期:  %0.4d-%0.2d-%0.2d \n\r",RTC_DateStructure.RTC_Year ,RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date);
}

void RTC_TimeShow(void)
{
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);//获取时间
  printf("\n\r当前时间:  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}


void RTC_AlarmShow(void)
{
  RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);//获取闹钟设置的时间
  printf("\n\r闹钟时间:  %0.2d:%0.2d:%0.2d \n\r", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
}



void RTC_Configuration(void)
	{
		RCC_BackupResetCmd(ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		PWR_BackupAccessCmd(ENABLE);
		RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div32);
		RCC_RTCCLKCmd(ENABLE);
	}

	

uint8_t USART_Scanf(uint32_t value)
{
  uint32_t index = 0;
  uint32_t tmp[2] = {0, 0};

  while (index < 2)
  {
    /* Loop until RXNE = 1 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {}
    tmp[index++] = (USART_ReceiveData(USART1));
    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    {
      printf("\n\r Please enter valid number between 0 and 9 \n\r");
      index--;
    }
  }
  /* Calculate the Corresponding value */
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  /* Checks */
  if (index > value)
  {
    printf("\n\r Please enter valid number between 0 and %d \n\r", value);
    return 0xFF;
  }
  return index;
}


