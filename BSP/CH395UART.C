/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395UART.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395芯片 UART串行连接的硬件抽象层 V1.0
*                      
*******************************************************************************/

/* 硬件相关宏定义 */
/* 本例中的硬件连接方式如下(实际应用电路可以参照修改下述定义及子程序) */
/* 单片机的引脚    CH395芯片的引脚
      TXD                  RXD
      RXD                  TXD       */
			
#include "stm32f0xx.h"		
#include "CH395INC.h"	

//extern UART_HandleTypeDef huart3;
extern uint8_t RxBuffer1[];
extern uint8_t TxBuffer[];
extern uint8_t RxBuffer[];
extern uint16_t TxCount;
extern uint16_t NbrOfDataToRead;
extern uint16_t NbrOfDataToTransfer;
extern uint8_t runningFlag;
#define	CH395_INT_PIN_WIRE	   GPIOA->IDR & GPIO_Pin_5 
//#define CH395_INT_WIRE             INT1                          /* 假定CH395的INT#引脚,如果未连接那么也可以通过查询串口中断状态码实现 */

#define UART_INIT_BAUDRATE         9600                          /* 默认通讯波特率9600bps,建议通过硬件引脚设定直接选择更高的CH395的默认通讯波特率 */
#define UART_WORK_BAUDRATE         115200                         /* 正式通讯波特率57600bps */
#define xEndCH395Cmd()     {}                                        /* 命令结束，仅在SPI模式下有效 */

/*******************************************************************************
* Function Name  : mDelayuS
* Description    : 延时指定微秒时间,根据单片机主频调整,不精确
* Input          : us---延时时间值
* Output         : None
* Return         : None
*******************************************************************************/
void mDelayuS(unsigned char us)
{
	  us*=2;
    while(us --);                                                    /* MCS51@24MHz */
}

/*******************************************************************************
* Function Name  : mDelaymS
* Description    : 延时指定毫秒时间,根据单片机主频调整,不精确
* Input          : ms---延时时间值
* Output         : None
* Return         : None
*******************************************************************************/
void mDelaymS(unsigned char ms)
{
    while(ms --) 
    {
        mDelayuS(250);
        mDelayuS(250);
        mDelayuS(250);
        mDelayuS(250);
    }
}

/*******************************************************************************
* Function Name  : CH395_PORT_INIT
* Description    : 硬件初始化部分
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT(void)
{
    /* 如果单片机只有一个串口,那么必须禁止通过串口输出监控信息 */
   	GPIO_InitTypeDef GPIO_InitStructure;

	 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		GPIO_SetBits(GPIOA, GPIO_Pin_4, GPIO_Pin_SET);
//		//HAL_GPIO_WritePin(GPIOA, GPIO_Pin_4, GPIO_PIN_SET);
}

/*******************************************************************************
* Function Name  : Set_MCU_BaudRate
* Description    : 设置单片机波特率
*                  将单片机切换到正式通讯波特率 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetMCUBaudRate(void)                                
{
//  TL2 = RCAP2L = 0 - 18432000/32/UART_WORK_BAUDRATE;               /* 18.432MHz晶振 */
	
	USART_InitTypeDef USART_InitStructure;

   USART_InitStructure.USART_BaudRate = 115200;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
	 USART_Init(USART3, &USART_InitStructure);
}

void USART3_Send_Byte( UINT8 dat )
{
	USART3->TDR = ( dat & (UINT16)0x01FF );								
	while( !( USART3->ISR & USART_FLAG_TXE ) ); 		
}

/********************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : 向CH395写命令
* Input          : cmd 8位的命令码
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd(unsigned char cmd)                                      
{
	  USART_Cmd(USART3, ENABLE);	
	  USART3_Send_Byte( SER_SYNC_CODE1 );							 /* ??????1?????? */
	  USART3_Send_Byte( SER_SYNC_CODE2 );							 /* ??????2?????? */
	  USART3_Send_Byte( cmd );							 		 /* ????? */
	//  mDelayuS( 2 );  	                                                        
}

/********************************************************************************
* Function Name  : xWriteCH395Data
* Description    : 向CH395写数据
* Input          : mdata 8位数据
* Output         : None
* Return         : None
*******************************************************************************/
void  xWriteCH395Data(unsigned char mdata)
{                                                                    /* 向CH395写数据 */
    USART_Cmd(USART3, ENABLE);			 
    USART3_Send_Byte(mdata);
}

/********************************************************************************
* Function Name  : xReadCH395Data
* Description    : 从CH395读数据
* Input          : None
* Output         : None
* Return         : 8位数据
*******************************************************************************/
unsigned char xReadCH395Data(void)                                          /* 从CH395读数据 */
{
	UINT32 i;
	for( i = 0; i < 0xFFFFF0; i ++ ) 
	{  
		/* ?????? */		
		if( USART3->ISR & USART_FLAG_RXNE ) 
		{  
			/* ????? */
			return( ( UINT8 )( USART3->RDR & (UINT16)0x01FF ) );/* ???? */
		}
	}
	return( 0 );  												 /* ???????? */
}

/*******************************************************************************
* Function Name  : Query395Interrupt
* Description    : ??CH395??(INT#???)
* Input          : None
* Output         : None
* Return         : ??????
*******************************************************************************/
UINT8 Query395Interrupt( void )
{
	return( CH395_INT_PIN_WIRE ? FALSE : TRUE );  
}
/**************************** endfile *************************************/
