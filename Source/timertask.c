#include "main.h"
#include "setting.h"
#include "N_6000.h"
#include "N_6205.h"
TimerHandle_t xTimers[2] = {NULL};
BOOL getRSSItimeFlag=InitStatus;
BOOL getRealtimeFlag=0;
BOOL HW_N3030_SendFlag=0;
BOOL ucN_6205RX_FLAG=0;
uint16_t timerNcount=0;
uint8_t N6205RXTIMCNT=0;
uint8_t SYS_Init_Flag=0;
uint8_t D_TIMflag=0;
uint8_t QSP_TIMflag=1;
extern uint8_t QSN_Flag;
extern uint16_t RxCount;
extern uint8_t Heart_time;
extern uint8_t NEWMSG_FLAG;
//extern N_3030DecodeMEM N_3030DM;
/*
*********************************************************************************************************
*	函 数 名: vTimerCallback
*	功能说明: 定时器回调函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void vTimerCallback(xTimerHandle pxTimer)
{
	uint32_t ulTimerID;

//	configASSERT(pxTimer);

//	/* 获取那个定时器时间到 */
//	ulTimerID = (uint32_t)pvTimerGetTimerID(pxTimer);
//	
//	/* 处理定时器0任务 */
//	if(ulTimerID == 0)
//	{
//		  xTimerStop(xTimers[0], 100); 
//	}
//	
//	/* 处理定时器1任务 */
//	if(ulTimerID == 1)
//	{
//		  xTimerStop(xTimers[1], 100); 
//	}
}

/*
*********************************************************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建任务通信机制
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void AppObjCreate (void)
{
	const TickType_t  xTimerPer[2] = {1000, 2000};
	
	/* 
	  1. 创建定时器，如果在RTOS调度开始前初始化定时器，那么系统启动后才会执行。
	  2. 统一初始化两个定时器，他们使用共同的回调函数，在回调函数中通过定时器ID来区分
	     是那个定时器的时间到。当然，使用不同的回调函数也是没问题的。
	*/
//	xTimers[0] = xTimerCreate("3G CSQ",          /* 定时器名字 */
//							       xTimerPer[0],    /* 定时器周期,单位时钟节拍 */
//							       pdFALSE,          /* 周期性 */
//							       (void *) 0,      /* 定时器ID */
//							       vTimerCallback); /* 定时器回调函数 */
//										 
//	xTimers[1] = xTimerCreate("3G RESEND",          /* 定时器名字 */
//							       xTimerPer[1],    /* 定时器周期,单位时钟节拍 */
//							       pdFALSE,          /* 周期性 */
//							       (void *) 1,      /* 定时器ID */
//							       vTimerCallback); /* 定时器回调函数 */
//	/*for(i = 0; i < 2; i++)
//	{
//		

//		if(xTimers[i] == NULL)
//		{
//			/* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
//		}
//		else
//		{
//			 /* 启动定时器，系统启动后才开始工作 */
//			 if(xTimerStart(xTimers[i], 100) != pdPASS)
//			 {
//				 /* 定时器还没有进入激活状态 */
//			 }
//		}
//	}*/
}

void TIM2_ClearTim(void)
{
	  	#if HW_N3030  
    timerNcount=0;
	  N_3030DM.ucDEcodeTIM=0;
	#endif
}

void TIM7_IRQHandler(void)
{
    static uint16_t Init_TIM[2]={0};
	  if(TIM_GetITStatus(TIM7,TIM_IT_Update) != RESET) 
    {
        TIM_ClearITPendingBit(TIM7,TIM_FLAG_Update);
        if(SYS_Init_Flag==1)
        {
           Init_TIM[0]++;
           if(Init_TIM[0]>3000)
           { 
              Init_TIM[0]=0;
              SYS_Init_Flag=3;
           }  
        }  
    }  
}
	
void TIM7_Config(unsigned int arr,unsigned int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///使能TIM2时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM7,ENABLE);
	//TIM_Cmd(TIM2,ENABLE);//使能定时器2
}

void TIM2_IRQHandler(void)
{
	static uint8_t timercount=0;
  static uint16_t CSQ_TIM[2]={0};
	static uint16_t timerHcount=0;
  static uint16_t N6205TIMCNT[3]={0,0,0};
  static uint32_t gettime=0;
   static uint32_t qsptime=0;
// 	CLI();			//关闭总中断
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET) 
	{
		 TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
		 timercount++;
		 timerHcount++;
    
     if(!getRealtimeFlag)
      {
          gettime++;
          if(gettime>4320000)
          { 
             gettime=0;
             getRealtimeFlag=1;
          }  
      }  
	
		 if(ucN_6205RX_FLAG==1)
		 {
			    N6205RXTIMCNT++;
			    if(N6205RXTIMCNT==2)
					{
					   N6205RXTIMCNT=0;
						 ucN_6205RX_FLAG=2;
					}		 
		 }	 
		 if(ucN_6205Heart_FLAG)
		 {
			   N6205TIMCNT[0]++;
			   if(N6205TIMCNT[0]>=Heart_time*100 && ucN_6205Send_FLAG==ZX_NoneFlag)
				 {
				    N6205TIMCNT[0]=0;
					  N6205TIMCNT[1]=0;
						ucN_6205Send_FLAG=ZX_HeartFlag;
				 }	 
				 else if(NEWMSG_FLAG)
				 {
				    NEWMSG_FLAG=0;
					  N6205TIMCNT[0]=0;
				 }	 
         
         
         if(D_TIMflag)
         {
             N6205TIMCNT[2]++;
             if(D_TIMflag==2)
             {
                D_TIMflag=1;
                N6205TIMCNT[2]=0;
             }  
             else if(N6205TIMCNT[2]>29999 && ucN_6205Send_FLAG==ZX_NoneFlag)
             {
                 N6205TIMCNT[2]=0;
                 ucN_6205Device_FLAG=1;
                 ucN_6205Send_FLAG=ZX_DeviceFlag;
             }  
             
             
         }       
         
         if(QSP_TIMflag)
         {
             qsptime++;
             if(qsptime>179999&& ucN_6205Send_FLAG==ZX_NoneFlag)
             {
                 qsptime=0;
                 ucN_6205Send_FLAG=ZX_QSPVaule;
             }  
         }  
		 }	 
     
     /*if()
     {
        
     } */ 
     if(ucN_6205Heart_FLAG==Heart_STATE_ON)
     {
			   N6205TIMCNT[1]=0;		   	 
				 ucN_6205Heart_FLAG=Heart_STATE_WAITING;			  
		 }	
		 else if(ucN_6205Heart_FLAG==Heart_STATE_WAITING)
		 {
				 N6205TIMCNT[1]++;
         if(HW_N6205MSG.ucHeartFlag && ucN_6205Send_FLAG==ZX_NoneFlag)
				 {
						 //ucN_6205Heart_FLAG=0;//
             D_TIMflag=2;
						 ucN_6205Send_FLAG=ZX_DeviceFlag;
						 HW_N6205MSG.ucHeartFlag=0;
				 }	 
				 else if(N6205TIMCNT[1]>3100)
				 {
				    N6205TIMCNT[1]=0;
					  ucN_6205Heart_FLAG=Heart_STATE_ERROR;			 
				 }
		 }	
		 else if(ucN_6205Heart_FLAG==Heart_STATE_ERROR)
		 {
				 if(!HW_N6205MSG.ucHeartFlag && ucN_6205Send_FLAG==ZX_NoneFlag)
						{
                D_TIMflag=2;
								ucN_6205Send_FLAG=ZX_DeviceFlag;
								HW_N6205MSG.ucHeartFlag=1;
						}	
		 }	 
   
     
     if(QSN_Flag)
     {  
        if(QSN_Flag==1)
        {  
            CSQ_TIM[0]++;
            if(CSQ_TIM[0]==100)
            {  
                CSQ_TIM[0]=0;
                if( GPIO_ReadOutputDataBit(LED6_GPIO_PORT,LED6_PIN))
                {
                    STM_RTU_LEDOff(LED6);
                    //STM_RTU_LEDOff(LED2);
                }
                else
                {
                     STM_RTU_LEDOn(LED6);
                    //STM_RTU_LEDOn(LED2);										 
                } 
             } 
         } 
         if(QSN_Flag==2)
         {
             CSQ_TIM[1]++;
            if(CSQ_TIM[1]==50)
            {  
                CSQ_TIM[1]=0;
                if( GPIO_ReadOutputDataBit(LED6_GPIO_PORT,LED6_PIN))
                {
                    STM_RTU_LEDOff(LED6);
                    //STM_RTU_LEDOff(LED2);
                }
                else
                {
                     STM_RTU_LEDOn(LED6);
                    //STM_RTU_LEDOn(LED2);										 
                } 
             } 
         }  
     } 
	 }
}

void TIM_Configuration(unsigned int arr,unsigned int psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM2时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器2更新中断
	TIM_Cmd(TIM2,ENABLE);
	//TIM_Cmd(TIM2,ENABLE);//使能定时器2
}

void bsp_DelayMS(uint16_t ms)  
{  
 /* ??????????????????? */  
 TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;  
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  ///使能TIM2时钟
 /* ???????36000,???72M?,????500us?1*/  
 TIM_TimeBaseInitStruct.TIM_Prescaler= 23999;   
 TIM_TimeBaseInitStruct.TIM_Period=ms*2;  
 TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);  
    
 TIM_SelectOnePulseMode(TIM6, TIM_OPMode_Single);  
 TIM_ClearFlag(TIM6, TIM_IT_Update);  
   
 TIM_Cmd(TIM6, ENABLE);  
     
 while(TIM_GetFlagStatus(TIM6, TIM_IT_Update)==RESET);    
 TIM_ClearFlag(TIM6, TIM_IT_Update);  
}  

void delay_us(uint16_t ms)  
{  
 /* ??????????????????? */  
 TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;  
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  ///使能TIM2时钟
 /* ???????36000,???72M?,????500us?1*/  
 TIM_TimeBaseInitStruct.TIM_Prescaler= 23999;   
 TIM_TimeBaseInitStruct.TIM_Period=ms*20;  
 TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);  
    
 TIM_SelectOnePulseMode(TIM6, TIM_OPMode_Single);  
 TIM_ClearFlag(TIM6, TIM_IT_Update);  
   
 TIM_Cmd(TIM6, ENABLE);  
     
 while(TIM_GetFlagStatus(TIM6, TIM_IT_Update)==RESET);    
 TIM_ClearFlag(TIM6, TIM_IT_Update);  
}  

