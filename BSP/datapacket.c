
/* Includes ------------------------------------------------------------------*/
#include "datapacket.h"
#include "datetime.h"
#include "setting.h"
#include <string.h>
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment the corresponding line to select the RTC Clock source */
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
//#define RTC_CLOCK_SOURCE_LSI  // LSI used as RTC source clock. The RTC Clock
                                // may varies due to LSI frequency dispersion

#define BKP_VALUE    0x32F0
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//const char* pointCodes[] = {
//    "Abenmla8", "Ak9dxtrm", "Ac1muwtl", "Aqd8ugfp", "Ay2jgk7a",
//};
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
uint8_t dataCounter = 0;
time_t t;
/* Private function prototypes -----------------------------------------------*/
static void RTC_Config(void);
static const char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
void initRTC(void)
{
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

//  RTC_InitTypeDef RTC_InitStructure;
//
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != BKP_VALUE)
  {
//    /* RTC configuration  */
//    RTC_Config();
//
//    /* Configure the RTC data register and RTC prescaler */
//    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
//    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
//    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
//
//    /* RTC init */
//    RTC_Init(&RTC_InitStructure);
//
//    /* Configure the time register */
//    RTC_TimeRegulate();
  }
  else
  {
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

 #ifdef RTC_CLOCK_SOURCE_LSI
    /* Enable the LSI OSC */
    RCC_LSICmd(ENABLE);
 #endif /* RTC_CLOCK_SOURCE_LSI */

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

  }
}

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
/* The RTC Clock may varies due to LSI frequency dispersion. */
  /* Enable the LSI OSC */
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  SynchPrediv = 0x18F;
  AsynchPrediv = 0x63;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
}

/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeRegulate(time_t ts)
{
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
  struct tm* tmp_time;
//  char strTime[21] = "20";

    RTC_InitTypeDef RTC_InitStructure;

    /* RTC configuration  */
    RTC_Config();

    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

    /* RTC init */
    RTC_Init(&RTC_InitStructure);

//  memset(strTime + 2, 0, sizeof(strTime) - 2);
//  BOOL rc = getCurrentTime(strTime + 2, sizeof(strTime));
//  strtotime(strTime, &tmp_time);

    tmp_time = localtime(&ts);

  /* Set Time hh:mm:ss */
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = tmp_time->tm_hour;
  RTC_TimeStructure.RTC_Minutes = tmp_time->tm_min;
  RTC_TimeStructure.RTC_Seconds = tmp_time->tm_sec;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

  /* Set Date Week/Date/Month/Year */
  RTC_DateStructure.RTC_WeekDay = tmp_time->tm_wday;
  RTC_DateStructure.RTC_Date = tmp_time->tm_mday;
  RTC_DateStructure.RTC_Month = tmp_time->tm_mon;
  RTC_DateStructure.RTC_Year = tmp_time->tm_year;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

  /* Write BkUp DR0 */
  RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_VALUE);

}

time_t getTimeStamp(void)
{
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
	struct tm tmp_time;
	time_t t;
  /* Get the current Time */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

 
  tmp_time.tm_hour = RTC_TimeStructure.RTC_Hours;
  tmp_time.tm_min = RTC_TimeStructure.RTC_Minutes;
  tmp_time.tm_sec = RTC_TimeStructure.RTC_Seconds;
  tmp_time.tm_mday = RTC_DateStructure.RTC_Date;
  tmp_time.tm_mon = RTC_DateStructure.RTC_Month;
  tmp_time.tm_year = RTC_DateStructure.RTC_Year;

  t = mktime(&tmp_time);

  return t;
}

void getTimeFLAG(char *times)
{
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
	struct tm tmp_time;
	time_t t;
  /* Get the current Time */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

 
 
  times[0] = RTC_TimeStructure.RTC_Seconds;
	times[1] = RTC_TimeStructure.RTC_Hours;
  times[2] = RTC_TimeStructure.RTC_Minutes;
  times[3] = RTC_DateStructure.RTC_Date;
  times[4] = RTC_DateStructure.RTC_Month;
  times[5] = RTC_DateStructure.RTC_Year-100;
}

time_t getRealTimeStamp(char *timestr)
{
	 RTC_TimeTypeDef RTC_TimeStructure;
   RTC_DateTypeDef RTC_DateStructure;
	 struct tm tmp_time;
	 
	 
	 uint8_t i;
  /* Get the current Time */
	
	 for(i=0;i<17;i++)
	 {
			timestr[i]-=0x30;
	 }	 
	 RTC_DateStructure.RTC_Year=100+timestr[0]*10+timestr[1]; 
	 RTC_DateStructure.RTC_Month=timestr[3]*10+timestr[4]-1;
	 RTC_DateStructure.RTC_Date=timestr[6]*10+timestr[7];
	 
	 RTC_TimeStructure.RTC_Hours=timestr[9]*10+timestr[10];
	 RTC_TimeStructure.RTC_Minutes=timestr[12]*10+timestr[13];
	 RTC_TimeStructure.RTC_Seconds=timestr[15]*10+timestr[16];

   tmp_time.tm_hour = RTC_TimeStructure.RTC_Hours;
	 tmp_time.tm_min = RTC_TimeStructure.RTC_Minutes;
	 tmp_time.tm_sec = RTC_TimeStructure.RTC_Seconds;
   tmp_time.tm_mday = RTC_DateStructure.RTC_Date;
   tmp_time.tm_mon = RTC_DateStructure.RTC_Month;
   tmp_time.tm_year = RTC_DateStructure.RTC_Year;

   t = mktime(&tmp_time);
}


void num2char(char *str, float number, uint8_t g, uint8_t l)
{
    uint8_t i,ntemp=0;
    int temp = number/1;
    float t2 = 0.0;
	  if(temp<0)
		{
		  ntemp=1;
			str[0]='-';
		}	
	  if(temp/10000)
		{
		   g=5;
		}	
		else if(temp/1000)
		{
		   g=4;
		}	
		else if(temp/100)
		{
		   g=3;
		}	
		else if(temp/10)
		{
		   g=2;
		}	
		else g=1;
    for (i=1; i<=g; i++)
    {
        if (temp==0)
            str[g-i+ntemp] = table[0];
        else
            str[g-i+ntemp] = table[temp%10];
        temp = temp/10;
    }
    *(str+g) = '.';
    temp = 0;
    t2 = number;
    for(i=1; i<=l; i++)
    {
        temp = t2*10;
        str[g+i+ntemp] = table[temp%10];
        t2 = t2*10;
    }
    *(str+g+l+1+ntemp) = '\0';
}
	 
#if HW_N6000 || HW_N3030
void makeN_CRT_DataPack(char* databuf, HWDeviceStatus N_CRTDS)
{
	  static uint16_t meg_cnt=0;
	  uint8_t i;  
	  uint32_t tempCKS;
	  databuf[0]='@';//启动符
	  databuf[1]='@';
	  databuf[2]=meg_cnt; //业务流水
	  databuf[3]=meg_cnt>>8;
	  meg_cnt++;
	  databuf[4]=0x01;  //协议版本
	  databuf[5]=0x01;
	  getTimeFLAG(databuf+6);
	 // memcpy(databuf+6, N_CRTDS.ucDev_TimeDay,6);//时间标签
	  memset(databuf+12, 0, 6);
	  memset(databuf+18, 0, 6);
	 #if HW_N6000
	  if(!N_6000DS.ucMSG_Type)
	 #elif HW_N3030
		if(!N_3030DS.ucMSG_Type)
	 #endif		
		{	
				databuf[24]=48;//应用数据单元长度
				databuf[25]=0x00;
				databuf[26]=2;//命令字节
				databuf[27]=2;//类型标志符
				databuf[28]=1;//信息对象数目
				databuf[29]=1;//系统类型
				databuf[30]=N_CRTDS.ucSYS_Address;//系统地址
				databuf[31]=N_CRTDS.ucDev_Type;//部件类型
				memcpy(databuf+32, N_CRTDS.ucDev_Address,4);//部件地址
				databuf[36]=N_CRTDS.ucDev_Status;//部件状态
				databuf[37]=N_CRTDS.ucDev_Status>>8;
				memcpy(databuf+38, N_CRTDS.ucDev_Notice,31);//部件说明
				memcpy(databuf+69, N_CRTDS.ucDev_TimeDay,6);//时间标签
				for(i=2;i<27;i+=2)
				{
					 tempCKS=databuf[i]+databuf[i+1];
				}
				databuf[75]=tempCKS;
				databuf[76]='#';
				databuf[77]='#';
		}
    else
		{
				databuf[24]=12;//应用数据单元长度
				databuf[25]=0x00;
				databuf[26]=2;//命令字节
				databuf[27]=1;//类型标志符
				databuf[28]=1;//信息对象数目
				databuf[29]=1;//系统类型
				databuf[30]=N_CRTDS.ucSYS_Address;//系统地址
			  databuf[31]=N_CRTDS.ucDev_Status;//系统状态
				databuf[32]=N_CRTDS.ucDev_Status>>8;
				memcpy(databuf+33, N_CRTDS.ucDev_TimeDay,6);//时间标签
				for(i=2;i<27;i+=2)
				{
					 tempCKS=databuf[i]+databuf[i+1];
				}
				databuf[39]=tempCKS;
				databuf[40]='#';
				databuf[41]='#';
		}	
}		
#endif

void makeDataPack(char* databuf, USHORT dataRead[], USHORT dataReadCoid[],int len)
{
   char *tmpBuf=malloc(40);
	  char i,j;
    float ftemp;
    sprintf(databuf, "2,%d,%ld,", ++dataCounter, getTimeStamp());

		if(NULL != tmpBuf)
		{
				for(i = 0; i < len; i++)
				{				 
					 // ftemp=*(float *)dataRead+i;
						char tmpStr[9] = "";
						char tmpStr1[9] = "";
						 
					  switch((pPoints[i]->regType))
						{
							  case 0:
								case 1:
                case 2:
                case 5:		
								case 53:	
													strncpy(tmpStr, pPoints[i]->pointCode, 8);
													sprintf(tmpBuf, "%s;%d", tmpStr, dataReadCoid[i]);
													strcat(databuf, tmpBuf);
													if(i == len - 1)
															strcat(databuf, "$");
													else
															strcat(databuf, ";");
                          break;
													
                default:	
                          strncpy(tmpStr, pPoints[i]->pointCode, 8);
								         
													if(pPoints[i-1]->dataType==8&&i>0)
													{
															if((pPoints[i]->dataType)==8)
															{				
																  j=j+2;
																	num2char(tmpStr1,*(float*)(dataRead+j),5,2);																	
																	sprintf(tmpBuf, "%s;%s", tmpStr, tmpStr1);
															}
															else
															{
																  j=j+1;
																	sprintf(tmpBuf, "%s;%d", tmpStr, dataRead[j]);
															}
													}	
													else
													{
															if((pPoints[i]->dataType)==8)
															{				
																  if(i%2==1)
																	{
																		j=i+1;
																	}	 
																	else
																	 {
																			j=i;
																	 }	
																	num2char(tmpStr1,*(float*)(dataRead+j),5,2);																	
																	sprintf(tmpBuf, "%s;%s", tmpStr, tmpStr1);
															}
															else
															{
																	sprintf(tmpBuf, "%s;%d", tmpStr, dataRead[i]);
															}
													}	
				
											
													strcat(databuf, tmpBuf);
													if(i == len - 1)
															strcat(databuf, "$");
													else
															strcat(databuf, ";");										
													break;
						} 								
				}
				free(tmpBuf);
		}		
}

void makeUSDataPack(char* databuf, USHORT dataRead[], int len)
{
    char* tmpBuf = malloc(80);
	  int i;
    float ftemp;
     sprintf(databuf, "2,%d,%ld,", ++dataCounter, getTimeStamp());
    if(NULL != tmpBuf)
    {
        for(i = 0; i < len; i++)
        {
            char tmpStr[9] = "";
            strncpy(tmpStr, pPoints[i]->pointCode, 8);
            sprintf(tmpBuf, "%s;%d", tmpStr, dataRead[i]);
            strcat(databuf, tmpBuf);
            if(i == len - 1)
                strcat(databuf, "$");
            else
                strcat(databuf, ";");
        }
        free(tmpBuf);
    }
}

