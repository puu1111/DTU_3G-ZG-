//=============================================================================
//文件名称：BSP_ADC1.c
//功能概要：ADC1驱动文件
//版权所有：源地工作室www.vcc-gnd.com
//淘宝网店：http://vcc-gnd.taobao.com
//更新时间：2013-11-20
//调试方式：J-Link OB ARM SWD
//=============================================================================

#include "BSP_ADC1.h"

 __IO uint32_t TempSensVoltmv=0;
 __IO uint32_t VrefIntVoltmv=0;
 __IO uint32_t PC3Voltmv=0; 
 __IO uint32_t VbatVoltmv=0; 
 
 uint16_t RegularConvData_Tab[4];



void ADC1_DMA_Init(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	DMA_InitTypeDef     DMA_InitStructure;
	ADC_InitTypeDef     ADC_InitStructure;

	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	ADC_DeInit(ADC1);//ADC恢复默认设置		

  DMA_DeInit(DMA1_Channel1);	/* DMA1 Channel1 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;//外设地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设作为数据传输的来源
  DMA_InitStructure.DMA_BufferSize = 4;//
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器不变
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度为16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度为16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA_Priority设定DMA通道x的软件优先级
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMA通道x没有设置为内存到内存传输
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		  
  
	
	DMA_Cmd(DMA1_Channel1, ENABLE);/* DMA1 Channel1 enable */			
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular); /* Enable ADC_DMA */	
  ADC_DMACmd(ADC1, ENABLE);  
	
		

	ADC_StructInit(&ADC_InitStructure);//初始化ADC结构
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位精度
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //规定模式装换工作在连续模式
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对其为右对齐
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward; //ADC的扫描方向
  ADC_Init(ADC1, &ADC_InitStructure); 
	 
  ADC_ChannelConfig(ADC1, ADC_Channel_13 , ADC_SampleTime_239_5Cycles); /* Convert the ADC1 Channel 11 with 239.5 Cycles as sampling time */  

  ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint ,ADC_SampleTime_239_5Cycles); 
  ADC_VrefintCmd(ENABLE);
	
	ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor ,ADC_SampleTime_239_5Cycles);
	ADC_TempSensorCmd(ENABLE);
	
	ADC_ChannelConfig(ADC1, ADC_Channel_Vbat ,ADC_SampleTime_239_5Cycles);
	ADC_VbatCmd(ENABLE);
	
	ADC_GetCalibrationFactor(ADC1); /* ADC Calibration */  
  ADC_Cmd(ADC1, ENABLE);  /* Enable ADCperipheral[PerIdx] */	  
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); /* Wait the ADCEN falg */
  ADC_StartOfConversion(ADC1); /* ADC1 regular Software Start Conv */ 
			
}

