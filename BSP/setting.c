#include "setting.h"
#include "stm320518_i2c_ee.h"
#include "string.h"
#include "stdlib.h"
#include "24cxx.h" 
#define RCVBUFSIZE 256

uint16_t pos = 20;
uint8_t counter = 0;

struct serial srl;
struct cloudServer csr;
struct point* pPoints[20];
uint8_t Heart_time;
uint8_t hCCID[6];
uint8_t EppromFlag=0;
char ASCIICCID[17]={"iot/"};
extern uint8_t TxBuffer[];
extern uint16_t NbrOfDataToTransfer;
extern uint8_t WorkMode;
uint8_t ReceiveSetting(void)
{
	  char  rxBuf[RCVBUFSIZE] = {""};
		char tmpBuf[40];
	  char* p;
	  char id; 
		uint8_t len;
		uint8_t temppos;
	  time_t ts;
	  int n;
    struct point code;
	  uint16_t NumDataRead = 1;
   
		USART_Cmd(USART2, ENABLE);		
    USART_ITConfig(RTU_COM_2, USART_IT_RXNE, ENABLE);
    while(runningFlag);
		 USART_Cmd(USART2, ENABLE);
    runningFlag = TRUE;
    myRN = 0;
    bRecord = TRUE;
    memcpy(rxBuf, RxBuffer, strlen((char*)RxBuffer)-2);
		if(0 == strncmp((char*)rxBuf, "Finish\r\n", 8))
		{			
			 strcpy((char*)TxBuffer, "OK\r\n");
			 NbrOfDataToTransfer = 4;
			 runningFlag = TRUE;
			 USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
			 while(runningFlag);
			 runningFlag = TRUE;
			 pos=20;
			 return 0;
		} 
		else if(0 == strncmp((char*)rxBuf, "11|",3))
		{
		    temppos=0;
			// sprintf(tmpBuf, "%d",srl.workmode);
		  // len=strlen((char*)tmpBuf);
			// temppos=len;
//			 strcpy((char*)TxBuffer,(char*)tmpBuf);		 
//			 strcat((char*)TxBuffer+temppos, "|");
//			 temppos +=len;
//			 tmpBuf[0]=0;
			
			 sprintf(tmpBuf, "%d",srl.baudRate);
			 len=strlen((char*)tmpBuf);
			 strcpy((char*)TxBuffer+temppos,(char*)tmpBuf);
     	 temppos+=len;	
			 strcat((char*)TxBuffer+temppos, "|");
			 temppos++;
			 tmpBuf[0]=0;
			
			 sprintf(tmpBuf, "%d",srl.parity);
			 len=strlen((char*)tmpBuf);
			 strcpy((char*)TxBuffer+temppos,(char*)tmpBuf);	
       temppos+=len;						
			 strcat((char*)TxBuffer+temppos, "|");
			 temppos++;
			 tmpBuf[0]=0;
			 
			 sprintf(tmpBuf, "%d",srl.wordLength);
			 len=strlen((char*)tmpBuf);
			 strcpy((char*)TxBuffer+temppos,(char*)tmpBuf);		
			 temppos+=len;			
			 strcat((char*)TxBuffer+temppos, "|");
			 temppos++;
			 tmpBuf[0]=0;
			 
			 sprintf(tmpBuf, "%d",srl.stopBits);
			 len=strlen((char*)tmpBuf);
			 strcpy((char*)TxBuffer+temppos,(char*)tmpBuf);		
			 temppos+=len;				
			 strcat((char*)TxBuffer+temppos, "\r\n");
			 temppos+=2;		
			
			 NbrOfDataToTransfer = temppos;
			 runningFlag = TRUE;
			 USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
				while(runningFlag);
				runningFlag = TRUE;
					return 1;
		}	
		else if(0 == strncmp((char*)rxBuf, "12|", 3))
    {
			  temppos=0;
			
				len=strlen((char*)csr.aCCID);
        if(len>12)len=len>12?12:len;
				temppos +=len;
				strcpy((char*)TxBuffer,(char*)csr.aCCID);		 
				strcpy((char*)TxBuffer+temppos, "|");
				temppos++;
			  tmpBuf[0]=0;
      
        sprintf(tmpBuf, "%d",csr.filter_flag);
				len=strlen((char*)tmpBuf);
				strcpy((char*)TxBuffer+temppos,(char*)tmpBuf);		
				temppos+=len;			
				strcat((char*)TxBuffer+temppos, "|");
				temppos++;
				tmpBuf[0]=0;
			
			  sprintf(tmpBuf, "%d",csr.heartbeat_cycle);
				len=strlen((char*)tmpBuf);
				strcpy((char*)TxBuffer+temppos,(char*)tmpBuf);		
				temppos+=len;			
				strcat((char*)TxBuffer+temppos, "|");
				temppos++;
				tmpBuf[0]=0;
			
			  sprintf(tmpBuf, "%d",csr.indvalue_cycle);
				len=strlen((char*)tmpBuf);
				strcpy((char*)TxBuffer+temppos,(char*)tmpBuf);		
				temppos+=len;			
				strcat((char*)TxBuffer+temppos, "|");
				temppos++;
				tmpBuf[0]=0;				
		
				strcpy((char*)TxBuffer+temppos,(char*)(csr.name));		
				len=strlen((char*)csr.name);		
				temppos+=len;					
				strcat((char*)TxBuffer+len, "|");		
				temppos++;
			
			  sprintf(tmpBuf, "%d",csr.port);
				len=strlen((char*)tmpBuf);
				strcpy((char*)TxBuffer+temppos,(char*)tmpBuf);		
				temppos+=len;			
				strcat((char*)TxBuffer+temppos, "|");
				temppos++;
				tmpBuf[0]=0;
							
				strcpy((char*)TxBuffer+temppos,(char*)(csr.username));		
				len=strlen((char*)csr.username);	
				temppos+=len;										
				strcat((char*)TxBuffer+len, "|");		
				temppos++;
				
				strcpy((char*)TxBuffer+temppos,(char*)(csr.password));		
				len=strlen((char*)csr.password);	
				temppos+=len;										
				strcat((char*)TxBuffer+temppos, "\r\n");
				temppos+=2;		
				
				NbrOfDataToTransfer = temppos;
				runningFlag = TRUE;
				USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
				 while(runningFlag);
				runningFlag = TRUE;
					return 1;
		}				
    p = (char*)strtok((char*)rxBuf, "|");
    id = *p;
    switch(id)
    {
    case '1'://串口格式
        {
					  //p = (char*)strtok(NULL, "|");
           // srl.workmode = atoi(p);
            p = (char*)strtok(NULL, "|");
            srl.baudRate = atoi(p);
            p = (char*)strtok(NULL, "|");
            srl.parity = atoi(p);
            p = (char*)strtok(NULL, "|");
            srl.wordLength = atoi(p);
            p = (char*)strtok(NULL, "|");
            srl.stopBits = atoi(p);
					  AT24CXX_Write(pos,(uint8_t*)&srl, sizeof(struct serial));					 
					  //AT24CXX_Read( pos,(uint8_t*)&srl, sizeof(struct serial));
            pos += sizeof(struct serial);						
            strcpy((char*)TxBuffer, "OK\r\n");
            NbrOfDataToTransfer = 4;
            runningFlag = TRUE;
            USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
            while(runningFlag);
					  runningFlag = TRUE;
					  
        }
        break;
    case '2'://用户信息
        {
					    memset((uint8_t*)&csr,0,199); 
					  p = (char*)strtok(NULL, "|");
            strcpy(csr.aCCID, p);
            p = (char*)strtok(NULL, "|");
					  csr.filter_flag=atoi(p);
					  p = (char*)strtok(NULL, "|");
					  csr.heartbeat_cycle=atoi(p);
					  p = (char*)strtok(NULL, "|");
					  csr.indvalue_cycle=atoi(p);							
            p = (char*)strtok(NULL, "|");
            strcpy(csr.name, p);
            p = (char*)strtok(NULL, "|");
            csr.port = atoi(p);
            p = (char*)strtok(NULL, "|");
            strcpy(csr.username, p);
            p = (char*)strtok(NULL, "|");
            strcpy(csr.password, p);
					  p = (char*)strtok(NULL, "\n");				
					  AT24CXX_Write(pos,(uint8_t*)&csr, sizeof(struct cloudServer));
					
					   //AT24CXX_Read( pos,(uint8_t*)&csr, sizeof(struct cloudServer));
            pos += sizeof(struct cloudServer);
					  strcpy((char*)TxBuffer, "OK\r\n");
            NbrOfDataToTransfer = 4;
            runningFlag = TRUE;
            USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
             while(runningFlag);
					  runningFlag = TRUE;
						
        }
        break;
    case '3'://
        {
           static char jj=0;
					  int i;
					  char* p1;
					
            p = (char*)strtok(NULL, "|");
            n = atoi(p);
						for(i = 0; i < n; i++)
						{
								p = (char*)strtok(NULL, "|");
								p1 = p;
								code.regType = atoi(p1);
								p1 = (char*)strchr(p1, ',') + 1;
								code.slaveID = atoi(p1);
								p1 = (char*)strchr(p1, ',') + 1;
								code.startAddr = atoi(p1);
								p1 = (char*)strchr(p1, ',') + 1;
								code.dataType = atoi(p1);
								p1 = (char*)strchr(p1, ',') + 1;
								strcpy(code.pointCode, p1);
								AT24CXX_Write(pos,(uint8_t*)&code, sizeof(struct point));
								NumDataRead = sizeof(struct point);
								pPoints[i] = malloc(NumDataRead);
								AT24CXX_Read(pos,(uint8_t*)pPoints[i],NumDataRead);
								pos += sizeof(struct point);
						}								
						counter += n;
						AT24CXX_Write(0,&counter, 1);
						strcpy((char*)TxBuffer, "OK\r\n");
            NbrOfDataToTransfer = 4;
            runningFlag = TRUE;
            USART_ITConfig(RTU_COM_2, USART_IT_TXE, ENABLE);
             while(runningFlag);
					  runningFlag = TRUE;
						
        }
        break;

    default:
        break;
		return 1;
    }

}

uint8_t LoadSetting(void)
{   
	  uint8_t i=0,j=0;
    uint8_t tempEpprom=0;
	  // uint8_t aCCID[6]={0x3a,0x99,0xc9,0x00,0x00,0x02};
    uint16_t NumDataRead = 1;
    AT24CXX_Read(3, &EppromFlag,1);
   
    AT24CXX_Read(0, &counter,NumDataRead);
    NumDataRead = sizeof(struct serial);
    AT24CXX_Read( pos,(uint8_t*)&srl, NumDataRead);
    pos += sizeof(struct serial);
    NumDataRead = sizeof(struct cloudServer);
    AT24CXX_Read(pos,(uint8_t*)&csr,  NumDataRead);
    pos += sizeof(struct cloudServer);
    //WorkMode=srl.workmode;
	//ASCIIaCCID[]
	  Heart_time=csr.heartbeat_cycle?csr.heartbeat_cycle:120;
	  memcpy(ASCIICCID+4,csr.aCCID,12);
	  //strcat((char*)ASCIICCID, "/u");
		for(i=0;i<12;i+=2)
    {		 
				if(csr.aCCID[i]>=0x61)
				{
						hCCID[j]=csr.aCCID[i]-0x57;
				}	
				else if(csr.aCCID[i]>=0x41)
				{
				   	hCCID[j]=csr.aCCID[i]-0x37;
				}	
				else if(csr.aCCID[i]>=0x30)
				{
				   	hCCID[j]=csr.aCCID[i]-0x30;
				}	
				if(csr.aCCID[i+1]>=0x61)
				{  
					  hCCID[j]<<=4;
						hCCID[j]+=csr.aCCID[i+1]-0x57;
				}	
				else if(csr.aCCID[i+1]>=0x41)
				{
					  hCCID[j]<<=4;
				   	hCCID[j]+=csr.aCCID[i+1]-0x37;
				}	
				else if(csr.aCCID[i+1]>=0x30)
				{
					  hCCID[j]<<=4;
				   	hCCID[j]+=csr.aCCID[i+1]-0x30;
				}	
				j++;
		}	
    for(i = 0; i < counter; i++)
    {
        NumDataRead = sizeof(struct point);
        pPoints[i] = malloc(NumDataRead);
        AT24CXX_Read(pos,(uint8_t*)pPoints[i],NumDataRead);
        pos += sizeof(struct point);
    }
		pos=20;
    return EppromFlag;
}
