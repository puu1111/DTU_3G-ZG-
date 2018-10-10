#ifndef __timertask_H
#define __timertask_H
#include "mbport.h"

#define  InitStatus 2
#define  StopStatus 0
#define  FinStatus  1

extern void AppObjCreate (void);
void TIM_Configuration(unsigned int arr,unsigned int psc);
void TIM7_Config(unsigned int arr,unsigned int psc);
extern TimerHandle_t xTimers[2];
extern BOOL getRSSItimeFlag;
extern BOOL getRealtimeFlag;
#endif
