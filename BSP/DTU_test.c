#include "main.h"
#include "setting.h"
#include "DTU_test.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mbm.h"
#include "string.h"
#include "stdlib.h"
#include "stm320518_i2c_ee.h"
/* ----------------------- Mqtt includes ----------------------------------*/
#include "mqtt.h"
#include "24cxx.h" 
//typedef enum
//{	
//	 Status_RS485_9600=0,	 
//	 Status_RS485_19200=1,	
//	 Status_RS485_38400=2,	
//	 Status_RS485_115200=3,	
//	 Status_I2C=4,
//	 Status_3G_Mod=5,       
//	 Status_LED=6,     
//}eDTU_State;

extern uint8_t NbrOfDataToTransfer;
extern uint8_t TxBuffer[];
extern __IO uint8_t RxCount ;
extern BOOL TestFlag;
extern uint8_t proC ;
 UCHAR DTU_TestSta=0;   //test_Status
uint32_t baudrateval = 0;

extern void vUARTConfiguration(COM_TypeDef comX, uint32_t baudRate, uint32_t wordLength, uint32_t stopBits, uint32_t parity, FunctionalState rs485EN);
extern BOOL init3GMTest(void);
void AutoBauRate_StartBitMethod(void);
void AutoBauRate_StopBitMethod(void);
void I2C_TEST(UCHAR cnt)
{
	 
	  UCHAR tmpTBuf[255];
	  UCHAR tmpRBuf[255];
	  uint16_t NumDataRead = 255;
	  USHORT i;
	  for(i=0;i<255;i++)
	  {
		   tmpTBuf[i]=i+1;
		}
	
		
		AT24CXX_Write(0,(u8*)tmpTBuf,255);
		AT24CXX_Read(0,(u8*)tmpRBuf,255);
		//sEE_WriteBuffer(tmpTBuf, 0,  20);
		//sEE_ReadBuffer(tmpRBuf, 0,  &NumDataRead);

		 for(i=0;i<255;i++)
	   {
			  if(tmpTBuf[i]!=tmpRBuf[i])
				{
					  strcpy((char*)TxBuffer, "I2C_Flase\r\n");
            NbrOfDataToTransfer = 11;
            runningFlag = TRUE;
            USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
            while(runningFlag);
					  bRecord = TRUE;
					  runningFlag = TRUE;
				}	 
		 }
		 strcpy((char*)TxBuffer, "I2C_OK\r\n");
		 NbrOfDataToTransfer = 8;
		 runningFlag = TRUE;
		 USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
		 while(runningFlag);
     bRecord = TRUE;	 
		 runningFlag = TRUE;
		 memset(RxBuffer, 0, 255);
}	

STATIC void M3G_Mod_TEST(UCHAR cnt)
{
	   vUARTConfiguration(COM_3, 115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, DISABLE);
		 if(TRUE==init3GMTest())
		 {
				 strcpy((char*)TxBuffer, "3GM ");
				 strcat((char*)TxBuffer, "OK\r\n");
			   NbrOfDataToTransfer = strlen((char*)TxBuffer);
				 runningFlag = TRUE;
				 USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
				 while(runningFlag);		
			    runningFlag = TRUE;
		 }
		 else
		 {
					strcpy((char*)TxBuffer, "3GM ");
			    strcat((char*)TxBuffer, "Flase\r\n");
				  NbrOfDataToTransfer = strlen((char*)TxBuffer);
					runningFlag = TRUE;
					USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
					while(runningFlag);
			    runningFlag = TRUE;
		 }	
		 bRecord = TRUE;
		 myRN=0;
		//  proC = 0;
      memset(RxBuffer, 0, 255);		 
}

STATIC void DTU_485_Command(CHAR *str,CHAR *str1)
{
		if(0 == strncmp((char*)RxBuffer,str,12))
		{
		    strcpy((char*)TxBuffer, str1);
			  strcat((char*)TxBuffer, "OK\r\n");
			  NbrOfDataToTransfer = strlen((char*)TxBuffer);
				runningFlag = TRUE;
				USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
				while(runningFlag);
		}	 
		else
		{  
		    strcpy((char*)TxBuffer, str1);
			  strcat((char*)TxBuffer, "Flase\r\n");
				NbrOfDataToTransfer = strlen((char*)TxBuffer);
				runningFlag = TRUE;
				USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
			  while(runningFlag);
		}	 
		
		bRecord = TRUE;			     
		DTU_TestSta=1;	
}

STATIC void DTU_LED_Command(UCHAR cnt)
{
	  USHORT i,j;
	  	  STM_RTU_LEDOn(LED1);
	  STM_RTU_LEDOn(LED2);
	  STM_RTU_LEDOn(LED3);
	  STM_RTU_LEDOn(LED4);
	  STM_RTU_LEDOn(LED5);
	 

	  for(j=0;j<100;j++)
    {	
			for(i=0;i<50000;i++);
	  }
     STM_RTU_LEDOff(LED1);
	  STM_RTU_LEDOff(LED2);
	  STM_RTU_LEDOff(LED3);
	  STM_RTU_LEDOff(LED4);
	  STM_RTU_LEDOff(LED5);
	 
	  
		
		strcpy((char*)TxBuffer, "LED OK\r\n");
		NbrOfDataToTransfer = strlen((char*)TxBuffer);
		runningFlag = TRUE;
		USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
		while(runningFlag);
		runningFlag = TRUE;
		bRecord = TRUE; 
		memset(RxBuffer, 0, 255);
}

STATIC void DTU_485_TEST(UCHAR cnt)
{
	  USHORT i;
	  CHAR DTU_Command[8][20]={{"DTU:485 9600"} ,{"DTU:485 19200"},{"DTU:485 38400"},{"DTU:485 115200"},
		{"485 9600 "},{"485 19200 "},{"485 38400 "},{"485 115200 "}};	
		
	  if(!DTU_TestSta)
		{	
			  DTU_TestSta=1;
			  strcpy((char*)TxBuffer, "DTU:485 OK\r\n");
			  NbrOfDataToTransfer = 10;
				runningFlag = TRUE;
				USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
				while(runningFlag);  	
			  
		}
    else if(DTU_TestSta==1)
    {			
			//	USART_ITConfig(RTU_COM_2, USART_IT_RXNE, DISABLE);
			  TestFlag=TRUE;
				AutoBauRate_StartBitMethod();
			  AutoBauRate_StopBitMethod();
			  for(i=0;i<10000;i++);
			  //USART_Cmd(USART1, ENABLE);
			//  USART_ITConfig(RTU_COM_2, USART_IT_RXNE, DISABLE);
			
			  DTU_TestSta=2;
			  bRecord = TRUE;
			  runningFlag = TRUE;
			  RxCount=0;
			 	//USART_ITConfig(RTU_COM_2, USART_IT_RXNE, ENABLE);
			 // USART_ReceiveData(USART1);
			  while(runningFlag);
		}
    else
    {
			  switch(baudrateval)
				{
					  case 4: DTU_485_Command(DTU_Command[0],DTU_Command[4]);
					          
                  					
						        break;
						case 3:  DTU_485_Command(DTU_Command[1],DTU_Command[5]);
					          	
						        break;
							      
            case 2: DTU_485_Command(DTU_Command[2],DTU_Command[6]);
					       		
						        break;
            case 1:	 DTU_485_Command(DTU_Command[3],DTU_Command[7]);
					          DTU_TestSta=0;	
					           memset(RxBuffer, 0, 255);
						        break;							
				}	
						
		}			
}

STATIC void (*DTU_ALL_Code[7])(UCHAR cnt)={DTU_485_TEST,I2C_TEST,M3G_Mod_TEST,DTU_LED_Command};

void AutoBauRate_StopBitMethod(void)
{
	  //USART_AutoBaudRateConfig(USART1, USART_AutoBaudRate_StartBit);  
    USART_AutoBaudRateCmd(USART1, DISABLE);  
}	

void AutoBauRate_StartBitMethod(void)
{   
	
	uint8_t  tmp[25];
	RCC_ClocksTypeDef RCC_Clocks;
	
  USART_AutoBaudRateConfig(USART1, USART_AutoBaudRate_StartBit);  
 
  USART_AutoBaudRateCmd(USART1, ENABLE);  
 
  while(USART_GetFlagStatus(USART1, USART_FLAG_REACK) == RESET)
  {}    
 
  while(USART_GetFlagStatus(USART1, USART_FLAG_TEACK) == RESET)
  {}  
  
  /* Loop until the end of Autobaudrate phase */
  while(USART_GetFlagStatus(USART1, USART_FLAG_ABRF) == RESET)
  {}  
  
  /* If AutoBaudBate error occurred */
  if (USART_GetFlagStatus(USART1, USART_FLAG_ABRE) != RESET)
  {
  }
  else
  {
    
    /* Wait until RXNE flag is set */

    
		//将收到的数据发送到串口
		while(TestFlag);	
//    USART_SendData(USART1, 0xaa); 
//    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//    {}

   // USART_Cmd(USART1, DISABLE);
		//计算波特率
		RCC_GetClocksFreq(&RCC_Clocks);	
    baudrateval = RCC_Clocks.SYSCLK_Frequency / USART1->BRR/2;
		if(baudrateval>110000)
		{
		   baudrateval=1;
			 USART1->BRR= 208;
		}
    else if(baudrateval>30000)
    {
		   baudrateval=2;
			 USART1->BRR=625;
		}		
    else if(baudrateval>18000)
    {
		    baudrateval=3;
			  USART1->BRR=1250;
		}	
    else if(baudrateval>9000)
    {
		    baudrateval=4;
			  USART1->BRR=2500;
		}			
	
	
  }  
}
		

void DTU_ALL_TEST(void)
{
	  UCHAR i,j;
	  CHAR DTU_Command[10][20]={{"DTU:485"} ,{"DTU:I2C"},{"DTU:3GM"},{"DTU:LED"}};	
		
    
	  for(i=0;i<4;i++)
	  {
			  if(FALSE == runningFlag)
				{			
					   USART_Cmd(USART2, ENABLE);	
						if(0 == strncmp((char*)RxBuffer, DTU_Command[i], strlen(DTU_Command[i])))
						{
						     DTU_ALL_Code[i](i);
						     if(i)
								 {	 
								  	DTU_TestSta=0;		
		             }							 
						}	
				}		
		}

		
//	  switch(DTU_TestSta)
//		{
//			 case  Status_RS485_9600
//			 case  Status_RS485_9600
//			 case  Status_RS485_9600
//			 case  Status_RS485_9600
//			 case  Status_RS485_Bound:    break;
//			 case  Status_I2C: I2C_TEST();break;													 
//			 case  Status_LED:	break;	 
//       case  Status_RTC:	break;			     
//       case  Status_3G_Mod:  
//				                 
//												 break;														 
//		}	
}
