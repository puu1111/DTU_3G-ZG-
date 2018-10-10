#include "mbmul.h"
#include "mbmconfig.h"
/* ----------------------- Defines ------------------------------------------*/
#define MBP_DEBUG_TIMER_PERFORMANCE     ( 0 )

#define MAX_TIMER_HDLS                  ( 5 )
#define IDX_INVALID                     ( 255 )
#define EV_NONE                         ( 0 )

#define TIMER_TIMEOUT_INVALID           ( 65535U )
#define TIMER_PRESCALER                 ( 128U )
#define TIMER_XCLK                      ( 72000000U )

#define TIMER_MS2TICKS( xTimeOut )      ( ( TIMER_XCLK * ( xTimeOut ) ) / ( TIMER_PRESCALER * 1000U ) )
/* ----------------------- Defines ------------------------------------------*/


/* CRC 高位字节值表 */ 
const u8  auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 

/* CRC低位字节值表*/ 
const u8 auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;

typedef enum
{
    MBM_STATE_NONE,             /*!< Not yet started. */
    MBM_STATE_SEND,             /*!< Frame will be sent. */
    MBM_STATE_WAITING,          /*!< Waiting for an event. */
    MBM_STATE_DISASSEMBLE,      /*!< Disassembling the frame. */
    MBM_STATE_ERROR,            /*!< An error occurred. */
    MBM_STATE_DONE              /*!< We are done processing the request. */
} eMBMQueryState;


typedef struct
{
    UBYTE           ubFlag;
    USHORT          usNTimeOutMS;
    USHORT          usNTimeLeft;
} xTimerInternalHandle;
extern void bsp_DelayMS(uint16_t ms);
extern uint8_t TxBuffer[];
extern uint8_t RxBuffer[];
extern uint16_t NbrOfDataToTransfer;
/* ----------------------- Static variables ---------------------------------*/
STATIC xTimerInternalHandle arxTimerHdls[MAX_TIMER_HDLS];
STATIC UCHAR eState=0;
STATIC UCHAR MBcnt=0;
STATIC UCHAR MBErrorcnt=0;

u16 Crc_16(u8 *puchMsg, u16 usDataLen) 
{ 
	u16 uchCRCHi = 0xFF ;              /* 高CRC字节初始化  */ 
	u16 uchCRCLo = 0xFF ;              /* 低CRC 字节初始化 */ 
	u16 temp16;
	u32 uIndex ;                      /* CRC循环中的索引  */ 
	while (usDataLen--)                  /* 传输消息缓冲区   */ 
	{ 
		temp16=*puchMsg++;
		uIndex = uchCRCHi ^ temp16 ; /* 计算CRC          */ 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	return (uchCRCHi << 8 | uchCRCLo) ; 
}



void MBMSerialInit(UCHAR ucPort, ULONG ulBaudRate, eMBSerialParity eParity )
{
	  u16 usTimeoutMS;
	  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 
	  TIM_TimeBaseStructure.TIM_Prescaler = TIMER_PRESCALER;
    TIM_TimeBaseStructure.TIM_Period = TIMER_MS2TICKS( 1 );
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );

    /* Enable timer 3 */
    TIM_Cmd( TIM3, ENABLE );

		/* Enable timer 3 interrupt */
		TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );

    /* If baudrate > 19200 then we should use the fixed timer value 1750us. 
    * We can't match this exactly so we use 2000us. Otherwise use 3.5 timers
    * the character timeout. */
    if( ulBaudRate > 19200 )
    {
        usTimeoutMS = 2;
    }
    else
    {
				/* The number of ticks required for a character is given by 
				* xTicksCh = TIMER_TICKS_PER_SECOND * 11 / BAUDRATE
			  * The total timeout is given by xTicksCh * 3.5 = xTicksCh * 7/2.
			  */
        usTimeoutMS = ( USHORT ) ( ( 1000UL * 11UL * 7UL ) / ( 2 * ulBaudRate ) );
    }
		arxTimerHdls[0].ubFlag=0;
		arxTimerHdls[0].usNTimeOutMS = usTimeoutMS;
		arxTimerHdls[0].usNTimeLeft = usTimeoutMS;
		arxTimerHdls[1].ubFlag=0;
		arxTimerHdls[1].usNTimeOutMS = MBM_DEFAULT_RESPONSE_TIMEOUT;
		arxTimerHdls[1].usNTimeLeft = MBM_DEFAULT_RESPONSE_TIMEOUT;
		
}

BOOL MBMPro(UCHAR ucFuncode,UCHAR ucSlaveAddress, USHORT usInputStartAddress, USHORT usNInputs,
                        USHORT *arubBufferOut)
{
	  USHORT CrcData;
		USHORT buf_count,temp;
	  UCHAR i,j=0;
	  switch(eState)
		{	
				case MBM_STATE_NONE:
				     	 TxBuffer[0]=ucSlaveAddress;
			         TxBuffer[1]=ucFuncode;
				       TxBuffer[2]=usInputStartAddress>>8;
			      	 TxBuffer[3]=usInputStartAddress;
				       TxBuffer[4]=usNInputs>>8;
			      	 TxBuffer[5]=usNInputs;
				       temp=Crc_16(TxBuffer,6);  
							 TxBuffer[6]=temp>>8;
							 TxBuffer[7]=temp;
				       eState=MBM_STATE_SEND;
				       break;
				case MBM_STATE_SEND:  
					     NbrOfDataToTransfer = 8;
					     USART_Cmd(USART2, ENABLE);
				       USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
					     USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
				       arxTimerHdls[1].ubFlag=1;
				       eState=MBM_STATE_WAITING;
				       break;
        case MBM_STATE_WAITING:
					     break;
        case MBM_STATE_DISASSEMBLE:	
					     switch(RxBuffer[1])
							 {	 
									 case 1:
									 case 2: 
									 case 5: 	 
											buf_count= RxBuffer[2];
											CrcData = Crc_16(RxBuffer, buf_count+3);					
											if(CrcData == RxBuffer[buf_count+4] + (RxBuffer[buf_count+3] << 8))
											{								
												  arubBufferOut[0]=RxBuffer[3];
												  MBcnt=0;	
													eState=MBM_STATE_NONE;
													bsp_DelayMS(110);
													return 0;
											}	
											else if(MBErrorcnt>10) 
											{
													MBErrorcnt=0;
													for(i=0;i<buf_count;i+=2)
													{
														  arubBufferOut[j]=0;
															j++;
													}  
													return 0;
											}	 	
									    break;
									 case 3:
									 case 4: 
									 case 6:
										  buf_count= RxBuffer[2];
											CrcData = Crc_16(RxBuffer, buf_count+3);					
											if(CrcData == RxBuffer[buf_count+4] + (RxBuffer[buf_count+3] << 8))
											{
													for(i=0;i<buf_count;i+=2)
													{
															arubBufferOut[j]=RxBuffer[i+3]*0x100+RxBuffer[i+4];
															j++;
													}
												  MBcnt=0;	
													eState=MBM_STATE_NONE;
													bsp_DelayMS(110);
													return 0;
											}	
											else if(MBErrorcnt>10) 
											{
													MBErrorcnt=0;
													for(i=0;i<buf_count;i+=2)
													{
														  arubBufferOut[j]=0;
															j++;
													}  
													return 0;
											}	 	
									    break;
							 }			 
              				 
		}	
    return 1;		
}	

BOOL MBMFun(UCHAR ucFuncode,UCHAR ucSlaveAddress, USHORT usInputStartAddress, USHORT usNInputs,
                        USHORT *arubBufferOut)
{
	 BOOL  eState = 1;
	 do
	 {	 
			eState=MBMPro(ucFuncode,ucSlaveAddress,usInputStartAddress,usNInputs,arubBufferOut);
	 }
	 while( eState==1 );
	  return eState;
}	 

void vMBPUSART1ISR(UCHAR data)
{
   RxBuffer[MBcnt++]=data;
	 arxTimerHdls[0].ubFlag=1;
	 arxTimerHdls[0].usNTimeLeft=arxTimerHdls[0].usNTimeOutMS+1;
}	

void vMBPTimerISR(void)
{
   if(arxTimerHdls[0].ubFlag) 
	 {
		  arxTimerHdls[0].usNTimeLeft--;
	    if(!arxTimerHdls[0].usNTimeLeft)
			{
				   MBErrorcnt=0;
				   arxTimerHdls[0].usNTimeLeft=arxTimerHdls[0].usNTimeOutMS;
				   arxTimerHdls[1].usNTimeLeft=MBM_DEFAULT_RESPONSE_TIMEOUT;
				   arxTimerHdls[1].ubFlag=0;
				   arxTimerHdls[0].ubFlag=0;
					 eState=MBM_STATE_DISASSEMBLE;
			}	
	 }
   else if(arxTimerHdls[1].ubFlag) 
   {
		  arxTimerHdls[1].usNTimeLeft--;
	    if(!arxTimerHdls[1].usNTimeLeft)
			{
				  if(MBErrorcnt>10)
					{
					    arxTimerHdls[1].usNTimeLeft=MBM_DEFAULT_RESPONSE_TIMEOUT;
							arxTimerHdls[1].ubFlag=0;
							eState=MBM_STATE_DISASSEMBLE;
						  MBErrorcnt=0;
					}	
					else
					{  
						  MBErrorcnt++;
							arxTimerHdls[1].usNTimeLeft=MBM_DEFAULT_RESPONSE_TIMEOUT;
							arxTimerHdls[1].ubFlag=0;
							eState=MBM_STATE_NONE;
					}	
			}	
	 }		 
}

