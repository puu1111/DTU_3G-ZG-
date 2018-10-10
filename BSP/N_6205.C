#include "N_6205.h"
#include "main.h"
#include "setting.h"
#include <string.h>
#include <stdlib.h>


#if HW_N6205
#define PlayLoad 20
#define EventMaxBuf 255
#define dMSGNum HW_N6205MSG.ucALLNUM

extern uint8_t N6205RXTIMCNT;
extern BOOL ucN_6205RX_FLAG;
extern uint8_t hCCID[6];
extern uint16_t QSP_Vaule;
HW_N6205Status HW_N6205MSG;

uint8_t FirstHeart=0;
uint8_t NEWMSG_FLAG;
//uint8_t N_6205Buff[30][12];
uint8_t ucN_6205Send_FLAG=0;
uint8_t ucN_6205Heart_FLAG=0;
uint8_t ucN_6205Device_FLAG=0;

stWireless_Base stWirelessBase; 


uint8_t N_6205Pro_Decode(uint16_t *N_rxcn,uint8_t *rxbuf,uint8_t *txbuf)
{
	 static uint8_t Decode_STA=0;//解码状态.
   uint8_t tempcrc=0;//解码状态
   uint16_t tempnum=0;
   uint8_t i,j=0;
   uint8_t alarm_num;
	/* if(ucN_6205RX_FLAG==2)
	 {	  
	    *N_rxcn=0;
	 }
	 ucN_6205RX_FLAG=1;*/

	 rxbuf[*N_rxcn]=USART_ReceiveData(RTU_COM_2);

	 switch(Decode_STA)
	 {
			case 0:
            if(SYS_Init_Flag<eInit_Config && rxbuf[0]=='D')
            {
                //rxbuf[(*N_rxcn)++]=USART_ReceiveData(RTU_COM_2);
                (*N_rxcn)++;
                if(*N_rxcn==12)
                {
                    SYS_Init_Flag=2;
                    *N_rxcn=0;
                }              
            }  
            else
            {  
               if(SYS_Init_Flag<eInit_Config)
               {
                  if(!FirstHeart)
                  {
                      FirstHeart=1;
                      Uart_DMA_Config(DMA1_Channel4,(uint32_t)&USART2->TDR,(uint32_t *)HW_N6205MSG.ucTXbuffer,\
                      2,DMA_DIR_PeripheralDST);	         
                  }  
               }  
               if(rxbuf[*N_rxcn]==0x68) *N_rxcn=0;
               if(rxbuf[0]==0x68)
               {
                  if(*N_rxcn>0)
                  {						     
                      (*N_rxcn)++;
                      if((rxbuf[(*N_rxcn)-1])==0x16 && (*N_rxcn)==rxbuf[1])
                      {
                          for(i=0;i<rxbuf[1]-2;i++)
                          {
                              tempcrc+=rxbuf[i];
                          }
                                
                          if(tempcrc==rxbuf[(*N_rxcn)-2])
                          {
                              ucN_6205RX_FLAG=0;
                              Decode_STA=0;
                              ucN_6205Device_FLAG=1;
                              ucN_6205Heart_FLAG=Heart_STATE_ON;
                                  
                              if( GPIO_ReadOutputDataBit(LED2_GPIO_PORT,LED2_PIN))
                              {
                                  STM_RTU_LEDOff(LED2);
                              }
                              else
                              {
                                  STM_RTU_LEDOn(LED2);
                              } 
                             
                              stWirelessBase.D_AlarmId[dMSGNum][0]=0;
                              if(ucN_6205Send_FLAG==ZX_NoneFlag) ucN_6205Send_FLAG=ZX_EventFlag;	
                              if(rxbuf[2]!=eCmd_Heart&&rxbuf[2]!=eCmd_Init)
                              {  
                                if(HW_N6205MSG.ucOVERNUM==HW_N6205MSG.ucALLNUM)
                                {
                                   HW_N6205MSG.ucALLNUM=0;
                                   HW_N6205MSG.ucOVERNUM=0;
                                }	
                              }                           
                              if(rxbuf[2]>2&&rxbuf[2]<13)
                              {
                                  stWirelessBase.D_AlarmCnt=rxbuf[1]-13;//报警数量
                                  stWirelessBase.D_AlarmType[dMSGNum]=3;
                                  for(j=0;j<stWirelessBase.D_AlarmCnt;j++)
                                  {                           
                                      stWirelessBase.D_AlarmId[dMSGNum][j]=rxbuf[j+3];
                                  }      
                              } 
                             
                              switch(rxbuf[2])
                              { 
                                  case eCmd_Heart:
                                  {                                  
                                      ucN_6205Send_FLAG=ucN_6205Send_FLAG==ZX_RegisterFlag?ZX_RegisterFlag:ZX_NoneFlag;
                                      break;
                                  }    
                                  case eCmd_Init:
                                  {
                                      ucN_6205Send_FLAG=ZX_RegisterFlag;
                                      stWirelessBase.D_First=0;//rxbuf[3];
                                      for(i=4;i<130;i++)
                                      {
                                          if(rxbuf[i])
                                          {  
                                              stWirelessBase.D_Id[j]=i-3;
                                              stWirelessBase.D_type[j]=rxbuf[i];
                                              j++;
                                          } 
                                          stWirelessBase.D_Cnt=j;
                                      }
                                      break;
                                  }     
                                  
                                  case eCmd_FireAlarm: 
                                  {
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_Fire;
                                  } 
                                  break;                              
                                  case eCmd_Supervise:                                            
                                  {
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_Supervise;
                                  }
                                  break;
                                  case eCmd_SuperviseOFF:                          
                                  {
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_SuperviseOFF;
                                  }
                                  break;
                                  case eCmd_Troubled:
                                  {
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_DeviceCheck;
                                  }
                                  break;
                                  case eCmd_Normal:
                                  {
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_DeviceCheckoff;
                                  }
                                  break;  
                                  case eCmd_LowVolt:
                                  {
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_LowVolt;
                                 }
                                  break;  
                                  case eCmd_LowVoltOFF:
                                  {
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_LowVoltOFF;
                                  }
                                  break;  
                                  case eCmd_D_Pollute:
                                  {
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_Pollute;
                                  }
                                  break;
                                  case eCmd_Hornstrobe:
                                  {
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_Hornstrobe;
                                  }
                                  break;  
                                  case eCmd_HornstOFF:
                                  {
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_Hornstroff;
                                  }
                                  break;
                                
                                  case eCmd_Output:
                                  {
                                      stWirelessBase.D_AlarmCnt=1;
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_Output;
                                      stWirelessBase.D_AlarmType[dMSGNum]=1;
                                  } 
                                  break;  
                                  case eCmd_OutputOFF:
                                  {
                                      stWirelessBase.D_AlarmCnt=1;
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_OutputOFF;
                                      stWirelessBase.D_AlarmType[dMSGNum]=1;
                                  } 
                                  break;   
                                  case eCmd_Reset:
                                  {
                                      stWirelessBase.D_AlarmCnt=1;
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_Reset;
                                      stWirelessBase.D_AlarmType[dMSGNum]=1;
                                  } 
                                  break;   
                                  case eCmd_Check:
                                  {
                                       stWirelessBase.D_AlarmCnt=1;
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_Check;
                                      stWirelessBase.D_AlarmType[dMSGNum]=1;
                                  } 
                                  break;  
                                  case eCmd_Erasure:
                                  {
                                      stWirelessBase.D_AlarmCnt=1;
                                      stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_Erasure;
                                      stWirelessBase.D_AlarmType[dMSGNum]=1;
                                  } 
                                  break;  
                                                      
                                  case eCmd_SYS_Troubled:
                                  {
                                      switch(rxbuf[3])
                                      {
                                          case 0x80: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_T_Check;break;  
                                          case 0x40: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_T_MainPower;break;  
                                          case 0x20: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_T_MLowVolt;break;  
                                          case 0x10: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_T_StandbyPower;break; 
                                          case 0x08: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_T_SLowVolt;break;               
                                          case 0x04: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_T_Output;break;  
                                          case 0x02: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_T_Erasure;break;  
                                      }  
                                      stWirelessBase.D_AlarmCnt=1;
                                      stWirelessBase.D_AlarmType[dMSGNum]=1;
                                  }  
                                  break;                              
                                  case eCmd_SYS_Normal:
                                  {
                                      switch(rxbuf[3])
                                      {
                                          case 0x80: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_N_Check;break;  
                                          case 0x40: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_N_MainPower;break;  
                                          case 0x20: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_N_MLowVolt;break;  
                                          case 0x10: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_N_StandbyPower;break; 
                                          case 0x08: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_N_SLowVolt;break;               
                                          case 0x04: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_N_Output;break;  
                                          case 0x02: stWirelessBase.D_AlarmNum[dMSGNum]=eEvent_N_Erasure;break;  
                                      } 
                                      stWirelessBase.D_AlarmCnt=1;
                                      stWirelessBase.D_AlarmType[dMSGNum]=1;
                                  }                
                                  break;
                              }  
                              tempcrc=0;
                              txbuf[2]=rxbuf[2];  
                              txbuf[3]=0x55;
                              for(i=0;i<4;i++)
                              {
                                  tempcrc+=txbuf[i];
                              }             
                              txbuf[4]=tempcrc;
                            
                              if(rxbuf[2]!=eCmd_Heart&&rxbuf[2]!=eCmd_Init)
                              {  
                                HW_N6205MSG.ucALLNUM=HW_N6205MSG.ucALLNUM<EventMaxBuf?++HW_N6205MSG.ucALLNUM:0;								                       
                              }
                          } 
                          else
                          {
                               tempcrc=0;
                              txbuf[2]=rxbuf[2];  
                              txbuf[3]=0xAA;
                              for(i=0;i<4;i++)
                              {
                                  tempcrc+=txbuf[i];
                              }             
                              txbuf[4]=tempcrc;             
                          }                 
                         return 1;	              
                      }		
                  }	
                  else
                  {	
                      (*N_rxcn)++;
                  }	
               }	 
            }			
    }
    return 0;
}



uint16_t makeN6205_CRT_DataPack(char* databuf,uint8_t MSG_TYPE)
{
	  static uint32_t meg_cnt=0;
	  uint16_t i,j=0;  
	  char *tempccid;
	  //csr.CCID3a9ec9000001
	  uint8_t tempCCID[6]={0x3a,0x9e,0xc9,0x00,0x00,0x01};  
	  uint8_t tempdata[10]={00,0x36,0x01};  
	  uint32_t tempCKS;
	  uint16_t datalen=0;
	  uint16_t channelAllnum=0;	
	  uint8_t tempCKSlarry[4];
	  uint8_t tempValueCKSlarry[4];
		uint16_t tempqsp; 
		memcpy(databuf,tempdata,3);
	  memcpy(databuf+5,hCCID,6);//CCID   
	  databuf[11]=MSG_TYPE==0x30?0x03:MSG_TYPE;    //消息类型
	  tempCKS=getTimeStamp();//时间戳
	  tempCKSlarry[0]=tempCKS>>24;
	  tempCKSlarry[1]=tempCKS>>16;
	  tempCKSlarry[2]=tempCKS>>8;
	  tempCKSlarry[3]=tempCKS;
	  memcpy(databuf+12,(uint8_t *)tempCKSlarry,4);
	  databuf[16]=0X01;
	  databuf[17]=0X00;
	  databuf[18]=0X00;
	  databuf[19]=0X00;//流水号使能
	/*	databuf[20]=0; //业务流水
		databuf[21]=0;
		databuf[22]=0;
		databuf[23]=0;*/
		meg_cnt++;
		
		
		switch(MSG_TYPE)
	  {	
			   case 0x01://心跳预处理
			   {
					  datalen=PlayLoad;
					  databuf[3]=datalen>>8;
					  databuf[4]=datalen;      //字节长度
					  return datalen;
				 }	
         
         case 0x30://心跳预处理
			   {
            databuf[PlayLoad]=0;//上报设备条数
						databuf[PlayLoad+1]=1;
           
            databuf[PlayLoad+2]=0;//上报设备条数
						databuf[PlayLoad+3]=11;
           
            if(!QSP_Vaule) tempqsp=65423;
            if(QSP_Vaule==99) tempqsp=65408;
            else tempqsp=65536+(QSP_Vaule*2-113);
            databuf[PlayLoad+4]=tempqsp>>8;//上报设备条数
						databuf[PlayLoad+5]=tempqsp;
           
					  datalen=PlayLoad+6;		
					  databuf[3]=datalen>>8;
					  databuf[4]=datalen;      //字节长度
            NEWMSG_FLAG=1;
					  return datalen;
				 }	
         
			   case 0x15://设备状态
			   {
					  if(ucN_6205Device_FLAG)
					  {	
								databuf[PlayLoad]=0;//上报设备条数
								databuf[PlayLoad+1]=1;
							
								databuf[PlayLoad+2]=0;//设备地址
								databuf[PlayLoad+3]=0;
								databuf[PlayLoad+4]=0;
								databuf[PlayLoad+5]=0;
								if(ucN_6205Heart_FLAG==Heart_STATE_ERROR||ucN_6205Heart_FLAG==Heart_STATE_UP)//在线状态
								{
										databuf[PlayLoad+6]=0;			
								}	
								else
								{
										databuf[PlayLoad+6]=1;
								}	
								datalen=PlayLoad+7;		
			
				    databuf[3]=datalen>>8;
					  databuf[4]=datalen;      //字节长度
						NEWMSG_FLAG=1;				
					  return datalen;
						}
            else
            {
						    return 0;
						}							
        }
         
        case 0x02://注册
			  {
				    j=PlayLoad;
					  databuf[j++]=0;
            databuf[j++]=1;//控制器数量       
            databuf[j++]=0;
            databuf[j++]=1;//回路数量               
            databuf[j++]=0;  
            databuf[j++]=stWirelessBase.D_Cnt;//器件数量
            databuf[j++]=stWirelessBase.D_First;//控制器地址
            //控制器地址、回路地址
            databuf[j++]=stWirelessBase.D_First;
            databuf[j++]=0;
            databuf[j++]=1;
            //控制器地址、回路地址、器件地址   
            for(i=0;i<stWirelessBase.D_Cnt;i++)
            {
                databuf[j++]=stWirelessBase.D_First;
                databuf[j++]=0;
                databuf[j++]=1;
                databuf[j++]=0;
                databuf[j++]=stWirelessBase.D_Id[i];
                databuf[j++]=0;
                databuf[j++]=stWirelessBase.D_type[i];
            }   
            datalen=j; //字节长度
					  databuf[3]=datalen>>8;
					  databuf[4]=datalen; 
            return datalen;
        }  
        
				case 0x1f:
				{
						databuf[PlayLoad]=stWirelessBase.D_AlarmCnt;//事件条数
				    j=PlayLoad+1;
					  for(i=0;i<stWirelessBase.D_AlarmCnt;i++)
            {
                databuf[j++]=stWirelessBase.D_AlarmType[HW_N6205MSG.ucOVERNUM];
                databuf[j++]=stWirelessBase.D_First;//机号
                databuf[j++]=1;//回路号
                databuf[j++]=stWirelessBase.D_AlarmId[HW_N6205MSG.ucOVERNUM][i];//器件号
                databuf[j++]=stWirelessBase.D_AlarmNum[HW_N6205MSG.ucOVERNUM]>>8;
                databuf[j++]=stWirelessBase.D_AlarmNum[HW_N6205MSG.ucOVERNUM];//事件码	  
                memcpy(databuf+j,(uint8_t *)tempCKSlarry,4);//时间
                j+=4;
            }  
						
					  datalen=j; //字节长度
					  databuf[3]=datalen>>8;
					  databuf[4]=datalen; 
					  if(HW_N6205MSG.ucOVERNUM<HW_N6205MSG.ucALLNUM||HW_N6205MSG.ucOVERNUM<EventMaxBuf)
						{	 
								HW_N6205MSG.ucOVERNUM++;
						}
            else
            {
						    HW_N6205MSG.ucOVERNUM=0;
						}   		
							NEWMSG_FLAG=1;
						return datalen;
				}	
		}		
}		
#endif