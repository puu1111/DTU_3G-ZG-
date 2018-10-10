/**
  ******************************************************************************
  * @file    USART/USART_HyperTerminalInterrupt/stm32f0xx_it.c
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mbport.h"
#include "N_6000.h"
#include "stm32f0xx_it.h"
#include "main.h"
#include "N_6205.h"
/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup HyperTerminal_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TXBUFFERSIZE   300
#define RXBUFFERSIZE   332
#define DRufLarryNum  30
#define Mode_3GMOD   0

#define Mode_TCP     1
#define UART_Mode   Mode_3GMOD
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer[TXBUFFERSIZE];
uint8_t RxBuffer[RXBUFFERSIZE];
//uint8_t RxBuffer1[RXBUFFERSIZE];
uint16_t NbrOfDataToTransfer;
uint16_t NbrOfDataToRead;
__IO uint16_t TxCount = 0;
uint16_t RxCount = 0;
uint16_t RxCount11=0;
uint16_t N_RxCount = 0;
uint16_t N_TxCount = 0;
__IO uint8_t RxCount1 = 0;
BOOL runningFlag = FALSE;
BOOL TestFlag = FALSE;
BOOL bRecord = FALSE;
BOOL TCP_Finsh=0;
uint8_t proC = 0;
uint8_t rnCount = 0;
uint8_t myRN = 0;
uint8_t myRN1 = 0;
uint8_t Message_status=0;
uint8_t Message_class[6]={0};
 uint8_t  jds=0;
 uint8_t WorkMode=0;
extern uint8_t N_RxBuffer[DRufLarryNum][RXBUFFERSIZE];
extern const char N_6000Message[30][20];
extern const char N_6000Device[33][8];
extern struct serial srl;
extern BOOL HW_N3030_SendFlag;
extern BOOL configMode;
extern UCHAR DTU_TestSta;
/* Private function prototypes -----------------------------------------------*/
void USARTx_IRQHandler(USART_TypeDef* uartx);
extern void vUARTRXCmd(USART_TypeDef *comX,FunctionalState NewState);
extern void vMBPTimerISR(void);
extern void vMBPUSART1ISR(UCHAR data);
extern void UartDMAPro(void);
	/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void DMA1_Channel2_3_IRQHandler(void)
{
	  //static uint8_t DMA_status=0; 
	  if((DMA1->ISR)&DMA1_FLAG_TC2)    	//判断传输完成中断位是否置位
		{
				NbrOfDataToTransfer=0;
		} 
		DMA1->IFCR|=DMA1_FLAG_TC2; 	//清除完成传输中断
	
}	

void DMA1_Channel4_5_6_7_IRQHandler(void)
{
	  static uint8_t DMA_status=0;
	 
	  if((DMA1->ISR)&DMA1_FLAG_TC7)    	//判断传输完成中断位是否置位
		{
				NbrOfDataToTransfer=0;
			  DMA1->IFCR|=DMA1_FLAG_TC7; 	//清除完成传输中断
			  runningFlag = FALSE;
		} 
		//DMA1->IFCR|=DMA1_FLAG_TC2; 	//清除完成传输中断
	  else if((DMA1->ISR)&DMA1_FLAG_TC4)    	//判断传输完成中断位是否置位
		{
				N_TxCount=0;
			  DMA1->IFCR|=DMA1_FLAG_TC4; 	//清除完成传输中断
		} 
}	

void HAL_UART_TCP(USART_TypeDef* uartx)
{
	  if(USART_GetITStatus(uartx, USART_IT_RXNE) != RESET)
    {
    /* Read one byte from the receive data register */
				RxBuffer[0] = USART_ReceiveData(uartx);		
			 	runningFlag = FALSE;
        USART_Cmd(uartx, DISABLE);		
     }

     if(USART_GetITStatus(uartx, USART_IT_TXE) != RESET)
     {
    /* Write one byte to the transmit data register */
    //USART_ReceiveData(uartx);
	    	USART_SendData(uartx, TxBuffer[TxCount++]);

				if(TxCount == NbrOfDataToTransfer)
				{
					/* Disable the uartx Transmit interrupt */
					USART_ITConfig(uartx, USART_IT_TXE, DISABLE);
					TxCount = 0;
					runningFlag = FALSE;
				}
    }
}

void
TIM3_IRQHandler( )
{
	 if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET) 
	 {
			TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
			vMBPTimerISR(  );
	 } 
}

void
USART2_IRQHandler( )
{
	  uint8_t static tempflag=0;
	  uint8_t i;
	  uint8_t cR;	
	  uint32_t tempdata=0;	
    if(SYS_Init_Flag!=eInit_Config)
		{		 
				if( USART_GetITStatus( RTU_COM_2, USART_IT_TXE ) != RESET )
				{
						/* Handle transmission of data in modbus functions. Interrupt flags are
						 * cleared when new byte is written to transmit register.
						 */
					  USART_SendData(RTU_COM_2, TxBuffer[TxCount++]);  
						if(TxCount == NbrOfDataToTransfer)
						{
							/* Disable the uartx Transmit interrupt */
								USART_ITConfig(RTU_COM_2, USART_IT_TXE, DISABLE);
							  runningFlag = FALSE;
								TxCount = 0;							
						}
				}
				else if( USART_GetITStatus( RTU_COM_2, USART_IT_RXNE ) != RESET )
				{			
            if(N_6205Pro_Decode(&N_RxCount,HW_N6205MSG.ucRXbuffer,HW_N6205MSG.ucTXbuffer))
            {
               DMA1_Channel4->CCR &= (uint16_t)(~0x01);
               DMA1_Channel4->CNDTR = 6;
               DMA1_Channel4->CCR |= 0x01;
               USART2->CR3=1<<7;
               N_RxCount=0;
						}		         	
				}	
		    
		}	
    else
    {
        USARTx_IRQHandler(RTU_COM_2);
    }

}

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART3_4_IRQHandler(void)
{
	 if(UART_Mode==Mode_3GMOD)
	 {
			USARTx_IRQHandler(RTU_COM_3);
	 } 
	 else if(UART_Mode==Mode_TCP)
	 {
			HAL_UART_TCP(RTU_COM_3);
	 }	
  // USARTx_IRQHandler(RTU_COM_3);
}

void USARTx_IRQHandler(USART_TypeDef* uartx)
{
	 static uint8_t myRNCheck=1;
  if(USART_GetITStatus(uartx, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    uint8_t cR = USART_ReceiveData(uartx);

   // RxBuffer1[RxCount1++]=cR;
    
    if(cR=='>')
    {
        RxBuffer[0] = cR;
        bRecord = FALSE;
        RxCount1= 0;
        RxCount = 0;
        rnCount = 0;
        runningFlag = FALSE;
        USART_Cmd(uartx, DISABLE);
    }
    else
    {  
        if(bRecord)
        {
            RxBuffer[RxCount++] = cR;
        }
        RxCount1++;
        if(proC == '\r' && cR == '\n' && RxCount1>1)
        {
            if(!myRN1)
            {	
                rnCount++;
                if(rnCount == myRN)
                {
                    bRecord = TRUE;
                }
                else if(rnCount == myRN + 1)
                {
                    /* Disable the uartx Receive interrupt */
                  //	USART_ITConfig(uartx, USART_IT_RXNE, DISABLE);
                    bRecord = FALSE;
                    RxCount1= 0;
                    RxCount = 0;
                    rnCount = 0;
                    runningFlag = FALSE;
                     USART_Cmd(uartx, DISABLE);				  
                }
            }		
            else
            {  
               rnCount++;
               if(rnCount == myRN)
               {
                    bRecord = TRUE;
               }
               else if(rnCount == myRN1 + 1)
               {
                    /* Disable the uartx Receive interrupt */
                  //	USART_ITConfig(uartx, USART_IT_RXNE, DISABLE);
                    bRecord = FALSE;
                    RxCount1= 0;
                    RxCount = 0;
                    rnCount = 0;
                    runningFlag = FALSE;
                    USART_Cmd(uartx, DISABLE);
               }
            }	
        }
        if(DTU_TestSta==1)
        {
           bRecord = FALSE;
           RxCount1= 0;
           RxCount = 0;
           rnCount = 0;
           TestFlag = FALSE;
        }	
        proC = cR;
      }
  }

  if(USART_GetITStatus(uartx, USART_IT_TXE) != RESET)
  {
    /* Write one byte to the transmit data register */
    //USART_ReceiveData(uartx);
		USART_SendData(uartx, TxBuffer[TxCount++]);

    if(TxCount == NbrOfDataToTransfer)
    {
      /* Disable the uartx Transmit interrupt */
      USART_ITConfig(uartx, USART_IT_TXE, DISABLE);
      TxCount = 0;
      runningFlag = FALSE;
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
