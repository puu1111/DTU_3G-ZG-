/*
*********************************************************************************************************
*
*	模块名称 : TM7705 驱动模块(2通道带PGA的16位ADC)
*	文件名称 : bsp_tm7705.h
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _TM7706_H
#define _TM7706_H

void bsp_InitTM7705(void);
void TM7705_CalibSelf(uint8_t _ch,uint8_t num);
void TM7705_SytemCalibZero(uint8_t _ch,uint8_t num);
void TM7705_SytemCalibFull(uint8_t _ch,uint8_t num);
uint16_t TM7705_ReadAdc(uint8_t _ch,uint8_t num);

void TM7705_WriteReg(uint8_t _RegID, uint32_t _RegValue,uint8_t num);
uint32_t TM7705_ReadReg(uint8_t _RegID,uint8_t num);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
