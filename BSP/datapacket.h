#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

#include "main.h"
#include <stdio.h>
#include <time.h>
#include "N_6000.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void initRTC(void);

void RTC_TimeRegulate(time_t ts);
void makeN_CRT_DataPack(char* databuf, HWDeviceStatus N_CRTDS);
void makeDataPack(char* databuf, USHORT dataRead[], USHORT dataReadCoid[],int len);
time_t getRealTimeStamp(char *timestr);
time_t getTimeStamp(void);

#endif /* RTC_H_INCLUDED */
