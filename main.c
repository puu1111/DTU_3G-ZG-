/*
 * MODBUS Library: STM32 port
 * Copyright (c) 2008 Christian Walter <cwalter@embedded-solutions.at>
 * All rights reserved.
 *
 * $Id: demo.c,v 1.2 2009-01-01 23:37:23 cwalter Exp $
 */

/* ----------------------- System includes ----------------------------------*/


/* ----------------------- Platform includes --------------------------------*/
#include "main.h"
#include <jansson.h>
#include "setting.h"
#include "CH395.H"
#include "TM7706.h"
#include "stm320518_i2c_ee.h"
#include "24cxx.h" 
#include "N_6000.h"
#include "N_6205.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mbm.h"
#include "mbmul.h"
/* ----------------------- Mqtt includes ----------------------------------*/
#include "mqtt.h"
#include "DTU_test.h"
/* ----------------------- Defines ------------------------------------------*/
#define APP2_TASK_PRIORITY              ( tskIDLE_PRIORITY + 2 )
#define APP2_TASK_STACKSIZE             ( configMINIMAL_STACK_SIZE  )
#define APP3_TASK_PRIORITY              ( tskIDLE_PRIORITY + 3 )
#define APP3_TASK_STACKSIZE             ( 256 )

#define MBM_SERIAL_PORT           ( MB_UART2 )
#define MBM_SERIAL_BAUDRATE       ( 115200 )
#define MBM_PARITY                ( MB_PAR_NONE )

#define DEBUG_LED_ERROR           ( 0 )
#define DEBUG_LED_WORKING         ( 1 )

//#define APPLICATION_ADDRESS     (uint32_t)0x08003000
//#if   (defined ( __CC_ARM ))
//  __IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
//#elif (defined (__ICCARM__))
//#pragma location = 0x20000000
//  __no_init __IO uint32_t VectorTable[48];
//#elif defined   (  __GNUC__  )
//  __IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
//#elif defined ( __TASKING__ )
//  __IO uint32_t VectorTable[48] __at(0x20000000);
//#endif

/* ----------------------- Type definitions ---------------------------------*/

/* ----------------------- Static variables ---------------------------------*/
STATIC ErrorStatus HSEStartUpStatus;
BOOL            bPub = FALSE;
BOOL            configMode = TRUE;
BOOL DataMiningMode=1;
time_t ttemp[2];

 char timestr[21];
 	int volt[8];
/* ----------------------- Static functions ---------------------------------*/
STATIC void     vRCCConfiguration( void );
STATIC void     vNVICConfiguration( void );
STATIC void     vGPIOConfiguration( void );
STATIC void     vLEDConfiguration( void );
void     vUARTConfiguration(COM_TypeDef comX, uint32_t baudRate, uint32_t wordLength, uint32_t stopBits, uint32_t parity, FunctionalState rs485EN);
STATIC void     vRS485Configuration( void );
STATIC void     vIOSetLED( UBYTE ubIdx, BOOL bTurnOn );
void            vApp2Task( void *pvParameters );
void            vApp3Task( void *pvParameters );
void READ_AD_DATA(u16 *adc,u16 *Coils);
void jansson_pack_test(void);
void Pro_Upload(void);
void LedAllOn(void);
void LedAllOff(void);
//extern N_3030DecodeMEM N_3030DM;
extern BOOL FirstHeart;
extern uint8_t EppromFlag;
char  dataBuf[500];
extern uint8_t D_TIMflag;
extern uint8_t TxBuffer[];
extern uint16_t N_RxCount;
extern uint8_t N_RxBuffer[][332];
extern uint16_t RxCount;
extern void Fatfs_Init(void);
extern void uprintf(uint16_t num);
extern void MBMSerialInit(UCHAR ucPort, ULONG ulBaudRate, eMBSerialParity eParity);
extern BOOL MBMFun(UCHAR ucFuncode,UCHAR ucSlaveAddress, USHORT usInputStartAddress, USHORT usNInputs,
                        USHORT *arubBufferOut);
/* ----------------------- Start implementation -----------------------------*/

int
main( void )
 {
    uint16_t adc[18],p,numdata;
	  uint8_t mode_flag= 0;
	  uint16_t i,j;
	 
//	 uint8_t i = 0;
//   for(i = 0; i < 48; i++)
//  {
//    VectorTable[i] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
//  }

//  /* Enable the SYSCFG peripheral clock*/
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
//  /* Remap SRAM at 0x00000000 */
//  SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	
#if defined( LIBDEBUG ) & ( LIBDEBUG == 1 )
    libdebug(  );
#endif
    vRCCConfiguration(  );
    vGPIOConfiguration(  );
    vNVICConfiguration(  );
    vLEDConfiguration(  );
	  TIM_Configuration(999,479);
    TIM7_Config(999,479);
		vUARTConfiguration(COM_2, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, ENABLE);
    
		AT24CXX_Init();			//IIC³õÊ¼»¯ 
    bsp_DelayMS(500);
    if(LoadSetting()==0xaa)
    {
        vRS485Configuration();
    }  
     initRTC();
    USART_ITConfig(RTU_COM_2, USART_IT_RXNE, ENABLE);
    vUARTConfiguration(COM_3, 115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, DISABLE); 
		USART_ITConfig(RTU_COM_3, USART_IT_RXNE, ENABLE);	
		Uart_DMA_Config(DMA1_Channel7,(u32)&USART3->TDR,(u32 *)TxBuffer,\
		              2,DMA_DIR_PeripheralDST);	

		HW_N6205MSG.ucHeartFlag=0;
		HW_N6205MSG.ucALLNUM=0;
		HW_N6205MSG.ucFlag=0;
		HW_N6205MSG.ucOVERNUM=0;
		ucN_6205Heart_FLAG=Heart_STATE_ON;
	
		RxCount=0;
		configMode = FALSE;
		USART_Cmd(USART2, ENABLE);
		TIM_Cmd(TIM2,ENABLE); 
    HW_N6205MSG.ucTXbuffer[0]=0x68;
    HW_N6205MSG.ucTXbuffer[1]=0x06;
    HW_N6205MSG.ucTXbuffer[5]=0x16;
    SYS_Init_Flag=1;
    
    while(1)
		{ 
       if(SYS_Init_Flag==eInit_4GConfig)
       {
            STM_RTU_LEDOn(LED4);
            STM_RTU_LEDOn(LED5);
            bPub = init3GModule( );          
            getCurrentTime(timestr, 21);
            RTC_TimeRegulate(getRealTimeStamp(timestr));
            STM_RTU_LEDOff(LED4);
            STM_RTU_LEDOff(LED5);
            USART_ITConfig(RTU_COM_2, USART_IT_RXNE, ENABLE);
            Uart_DMA_Config(DMA1_Channel4,(u32)&USART2->TDR,(u32 *)HW_N6205MSG.ucTXbuffer,\
	               2,DMA_DIR_PeripheralDST);	
            ucN_6205Heart_FLAG=0;
            SYS_Init_Flag=eInit_Finish;
       }
       else if(SYS_Init_Flag==eInit_Finish)
       {           
            Pro_Upload();                            
       }
       else
       {
            if(SYS_Init_Flag==eInit_Config)
            {
                LedAllOn();
                mode_flag= Delay(0x25FFFFF);
                if(FALSE == mode_flag)//(0x4FFffFF))
                {       
                    while(ReceiveSetting( ));
                }  
                LoadSetting();
                vRS485Configuration(); 
                LedAllOff();
                SYS_Init_Flag=eInit_4GConfig;
            }  
       }          
   }  
}

void LedAllOn(void)
{
    STM_RTU_LEDOn(LED1);
    STM_RTU_LEDOn(LED2);
    STM_RTU_LEDOn(LED3);
    STM_RTU_LEDOn(LED4);
    STM_RTU_LEDOn(LED5);
    STM_RTU_LEDOn(LED6);
}

void LedAllOff(void)
{
     STM_RTU_LEDOff(LED1);
    STM_RTU_LEDOff(LED2);
    STM_RTU_LEDOff(LED3);
    STM_RTU_LEDOff(LED4);
    STM_RTU_LEDOff(LED5);
    STM_RTU_LEDOff(LED6);
}

void Data_Upload(uint16_t Txlen,uint8_t mode)
{
     static  uint8_t Status3G=0; 	
    if (bPub)
    {		
        if(EppromFlag!=0xaa)
        {  
            EppromFlag=0xaa;
            AT24CXX_Write(3, &EppromFlag,1);
          //  AT24CXX_Read(3, &EppromFlag,1);
           // EppromFlag=0;
        } 
      
        Status3G=mqtt_pub(dataBuf,Txlen); 
        if(FirstHeart)
        {
            FirstHeart=0;
            D_TIMflag=1;
        }  
        else
        {  
           if(mode==ZX_RegisterFlag)
           {  
              ucN_6205Send_FLAG=ZX_NoneFlag;
           }
           else 
            ucN_6205Send_FLAG=ucN_6205Send_FLAG==ZX_RegisterFlag?ZX_RegisterFlag:ZX_NoneFlag;
        }
        if(ucN_6205Heart_FLAG==Heart_STATE_ERROR)
        ucN_6205Heart_FLAG=Heart_STATE_UP;
    }	
    
    if(Status3G==1)
    {										
        vIOSetLED( DEBUG_LED_ERROR, FALSE );
        vIOSetLED( DEBUG_LED_WORKING, TRUE );
        bsp_DelayMS(10);
    }
    else
    {
        vIOSetLED( DEBUG_LED_WORKING, FALSE );
        vIOSetLED( DEBUG_LED_ERROR, TRUE );
         while(!Status3G)
         {	          
            STM_RTU_LEDOff(RST);
            bsp_DelayMS(300);
            STM_RTU_LEDOn(RST);
            bsp_DelayMS(8000);
            init3GModule(  );
            Status3G=mqtt_pub(dataBuf,Txlen);                                          
         }                          

    }	

    if(getRealtimeFlag==1)
    {				
        uint8_t timestr[21];
        if(getCurrentTime(timestr,21)!=FALSE)
        {  
           // timestr[0]=0;
            RTC_TimeRegulate(getRealTimeStamp(timestr));
           // getRealTimeStamp(timestr);
        }  
        getRealtimeFlag=0;
    }			
}

void Pro_Upload(void)
{
   static  uint8_t Status3G=0; 	 
	 static uint16_t tempTxLen; 
   uint8_t  mode_status;
  
   switch(ucN_6205Send_FLAG)
   {
        case ZX_NoneFlag:
        {
            mode_status=ucN_6205Send_FLAG;
            if(HW_N6205MSG.ucOVERNUM<HW_N6205MSG.ucALLNUM)
            {
                mode_status=ZX_EventFlag;
                tempTxLen=makeN6205_CRT_DataPack(dataBuf,mode_status);
                Data_Upload(tempTxLen,mode_status);
            }	 
        }
        break;
        
        case ZX_RegisterFlag:
        case ZX_EventFlag:
        case ZX_DeviceFlag:
        case ZX_HeartFlag:
        case ZX_QSPVaule:  
        {
            if(FirstHeart)
            {
                //ucN_6205Heart_FLAG=Heart_STATE_UP;
                mode_status=ZX_DeviceFlag;
                
            }  
            else
            {             
                mode_status=ucN_6205Send_FLAG;
            }  
            tempTxLen=makeN6205_CRT_DataPack(dataBuf,mode_status);
            if(tempTxLen)
            {	
               Data_Upload(tempTxLen,mode_status);
            }
            else
            {
                ucN_6205Send_FLAG=ZX_NoneFlag;
            }									
        }	
        break;
        
      /*  case ZX_ReSendFlag:
        {						  			 
           
        }	
        break;	*/
    }    
}
void
vApp2Task( void *pvParameters )
{
#if defined( MBP_ENABLE_DEBUG_FACILITY ) && ( MBP_ENABLE_DEBUG_FACILITY == 1 )
    //static char     arubBuffer[200];
#endif
    for( ;; )
    {
        vTaskDelay( 5000 );
#if defined( MBP_ENABLE_DEBUG_FACILITY ) && ( MBP_ENABLE_DEBUG_FACILITY == 1 )
        vTaskList( arubBuffer );
        vMBPPortLog( MB_LOG_ERROR, MB_LOG_PORT_OTHER, "%s", arubBuffer );
#endif
//			  if(getRSSItimeFlag==FinStatus)
//				{				
//						getRSSI();
//					  TIM_Cmd(TIM2,ENABLE);
//			      getRSSItimeFlag=StopStatus;
//				}	
    }
}

STATIC void
vRS485Configuration( void )
{
    uint32_t baudRate = srl.baudRate;
    uint32_t wordLength;
    uint32_t stopBits;
    uint32_t parity;

    switch(srl.wordLength)
    {
    case 8:
        wordLength = USART_WordLength_8b;
        break;
    case 9:
        wordLength = USART_WordLength_9b;
        break;
    case 7:
        wordLength = USART_WordLength_7b;
        break;
    default:
        break;
    }

    switch(srl.stopBits)
    {
    case 1:
        stopBits = USART_StopBits_1;
        break;
    case 2:
        stopBits = USART_StopBits_2;
        break;
    default:
        break;
    }

    switch(srl.parity)
    {
    case 0:
        parity = USART_Parity_No;
        break;
    case 1:
        parity = USART_Parity_Even;
        break;
    case 2:
        parity = USART_Parity_Odd;
        break;
    default:
        break;
    }

    vUARTConfiguration(COM_2, baudRate, wordLength, stopBits, parity, ENABLE);
}

STATIC void
vIOSetLED( UBYTE ubIdx, BOOL bTurnOn )
{
    STATIC BOOL     bIsInitalized = FALSE;

    if( !bIsInitalized )
    {
        bIsInitalized = TRUE;
    }
    switch ( ubIdx )
    {
    case DEBUG_LED_ERROR:
        if( bTurnOn )
        {
            STM_RTU_LEDOn(LED4);
        }
        else
        {
            STM_RTU_LEDOff(LED4);
        }
        break;

    case DEBUG_LED_WORKING:
        if( bTurnOn )
        {
            STM_RTU_LEDOn(LED3);
        }
        else
        {
            STM_RTU_LEDOff(LED3);
        }
        break;

    default:
        break;
    }
}

void
vUARTConfiguration(COM_TypeDef comX, uint32_t baudRate, uint32_t wordLength, uint32_t stopBits, uint32_t parity, FunctionalState rs485EN)
{
  USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = baudRate;
  USART_InitStructure.USART_WordLength = wordLength;
  USART_InitStructure.USART_StopBits = stopBits;
  USART_InitStructure.USART_Parity = parity;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  STM_RTU_COMInit(comX, &USART_InitStructure, rs485EN);
}

void
vUARTRXCmd(USART_TypeDef *comX,FunctionalState NewState)
{
	 USART_InitTypeDef USART_InitStructure;
	if(NewState==ENABLE)
  comX->CR1 |= USART_Mode_Rx ;
	else
  comX->CR1 &= ~USART_Mode_Rx;		
}

STATIC void
vLEDConfiguration( void )
{
   STM_RTU_LEDInit(LED1);
    STM_RTU_LEDInit(LED2);
    STM_RTU_LEDInit(LED3);
    STM_RTU_LEDInit(LED4);
    STM_RTU_LEDInit(LED5);
    STM_RTU_LEDInit(LED6);
    STM_RTU_LEDInit(LED7);
    STM_RTU_LEDInit(RST);
    STM_RTU_LEDOff(LED1);
    STM_RTU_LEDOff(LED2);
    STM_RTU_LEDOff(LED3);
    STM_RTU_LEDOff(LED4);
    STM_RTU_LEDOff(LED5);
    STM_RTU_LEDOff(LED6);
    //STM_RTU_LEDOn(LED7);
    STM_RTU_LEDOff(RST);
    bsp_DelayMS(300);
    STM_RTU_LEDOn(RST);
}

STATIC void
vRCCConfiguration( void )
{
    /* RCC system reset(for debug purpose) */
    RCC_DeInit(  );

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON );

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp(  );

    if( HSEStartUpStatus == SUCCESS )
    {
        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1 );
        /* PCLK1 = HCLK/2 */
        RCC_PCLKConfig( RCC_HCLK_Div2 );
        /* Flash 2 wait state */
        FLASH_SetLatency( FLASH_Latency_1 );
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd( ENABLE );
        /* PLLCLK = 8MHz * 6 = 48 MHz */
        RCC_PLLConfig( RCC_PLLSource_HSE, RCC_PLLMul_6 );
        /* Enable PLL */
        RCC_PLLCmd( ENABLE );
        /* Wait till PLL is ready */
        while( RCC_GetFlagStatus( RCC_FLAG_PLLRDY ) == RESET )
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

        /* Wait till PLL is used as system clock source */
        while( RCC_GetSYSCLKSource(  ) != 0x08 )
        {
        }
    }

    /* clocks */
//    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
}

STATIC void
vNVICConfiguration( void )
{
    STM_RTU_NVICInit(USART2_IRQn,1);
	  STM_RTU_NVICInit(DMA1_Channel2_3_IRQn,2);
	  STM_RTU_NVICInit(DMA1_Channel4_5_6_7_IRQn,2);
    STM_RTU_NVICInit(USART3_4_IRQn,0);
	  STM_RTU_NVICInit(TIM3_IRQn,3);
	  STM_RTU_NVICInit(TIM2_IRQn,2);
    STM_RTU_NVICInit(TIM7_IRQn,2);
}

STATIC void
vGPIOConfiguration( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure all unused GPIO port pins in Analog Input mode (floating input */
    /* trigger OFF), this will reduce the power consumption and increase the device */
    /* immunity against EMI/EMC */
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD |
                            RCC_AHBPeriph_GPIOF, ENABLE );

   /* GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init( GPIOA, &GPIO_InitStructure );*/

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12
                          |GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3
                          |GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
                          |GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init( GPIOF, &GPIO_InitStructure );

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD |
                            RCC_AHBPeriph_GPIOF, DISABLE );

//    /* Enable clocks on GPIOA and GPIOC */
//    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE );
//
//    /* Connect pin to Periph*/
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_1);
//
//    /* Configure USART1 Tx (PA9) as alternate function push-pull */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_Init( GPIOA, &GPIO_InitStructure );
//    /* Configure USART1 Rx (PA10) as input floating */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//    GPIO_Init( GPIOA, &GPIO_InitStructure );
//    /* Configure USART1 DE (PA12) as output */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init( GPIOA, &GPIO_InitStructure );

//    /* Configure USART2 Tx (PA.2) as alternate function push-pull */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_Init( GPIOA, &GPIO_InitStructure );
//    /* Configure USART2 Rx (PA.3) as input floating */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//    GPIO_Init( GPIOA, &GPIO_InitStructure );

}

void
vApplicationStackOverflowHook( xTaskHandle * pxTask, signed portCHAR * pcTaskName )
{
//    ( void )IntMasterDisable(  );
    /* Let the watchdog trigger a reset here. */
    for( ;; );
}
