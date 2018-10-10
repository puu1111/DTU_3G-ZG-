
////////////////////////////////////???//////////////////////////////////////////////////////////////////////

////////
//int main(void)
//{  uchar i;  
//   uchar data_24c02;
//   RCC_Configuration(); //????
//    GPIO_Configuration();//GPIO??
//   USARTx_configuration();//????


//     WIN24C02_init();
//     delayms(5000);//??


//  for(i=0;i<20;i++) //?EEPROM??
// { WIN24C02_write(0x00+i,i);delayms(100);}//????EEPROM
//   delayms(1000);//??
//   
//   while(1)//??3?????EEPROM???
//   {
//   for(i=0;i<20;i++)
//  {  data_24c02=WIN24C02_read(0x00+i);//??24C02?? 
//        USART_SendData(USART3 ,data_24c02);
//           while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
//  }
//   delayms(5000);//??
//   }

//}
#include "soft_i2c.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SCL           GPIO_Pin_6 //24C02 SCL
#define SDA           GPIO_Pin_7 //24C02 SDA

void GPIO_Configuration(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
}

void AT24C02_SDA_IO_SET(unsigned char io_set) //SDA????????
{
	 	GPIO_InitTypeDef GPIO_InitStructure;
    if(io_set==0)
  {
  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA ????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  }
 else if(io_set==1)
  {
  GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA ????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  //????
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  }
 else
  {;}
}

  

void delay2(uint x)
{
   uint i;
   for(i=0;i<x;i++);
}

void delay_nop(void)
{
  uint8_t i=7; //i=10??1.5us//???????? ,??????5????
   while(i--);

}

 

void IIC_Init()
{
   GPIO_Configuration();
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
  delay_nop();
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();
}



void IIC_Start()
{
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
   delay_nop();
   //SDA=0;
    GPIO_ResetBits(GPIOB, SDA);
   delay_nop();
   //SCL=0;
   GPIO_ResetBits(GPIOB, SCL);
   delay_nop();
}

void IIC_Stop()

{
   //SDA=0;
   GPIO_ResetBits(GPIOB, SDA);
   delay_nop();
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
   delay_nop();
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();
}

void IIC_Send_Byte(uchar j)

{
   uchar i,temp,temp1;

   temp=j;
  //AT24C02_SDA_IO_SET(0);
   for (i=0;i<8;i++)
   {
      temp1=temp & 0x80;
      temp=temp<<1;
     
      //SCL=0;
   GPIO_ResetBits(GPIOB, SCL);
      delay_nop();

   //SDA=CY;
  if(temp1==0x80)
   {GPIO_SetBits(GPIOB, SDA);}
  else
   {GPIO_ResetBits(GPIOB, SDA);}
     

      delay_nop();
     // SCL=1;
  GPIO_SetBits(GPIOB,SCL);
      delay_nop();
   }
 //AT24C02_SDA_IO_SET(0);
   //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
   delay_nop();
   //SDA=1;
   GPIO_SetBits(GPIOB,SDA);
   delay_nop();

}

uchar IIC_Read_Byte()
{
   uchar i,j,k=0;

   //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
    delay_nop(); 
 //SDA=1;
 GPIO_SetBits(GPIOB,SDA);

 AT24C02_SDA_IO_SET(1);
   for (i=0;i<8;i++)
   {
      delay_nop();
      //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
      delay_nop();
      //if (SDA==1) j=1;
  if( GPIO_ReadInputDataBit(GPIOB,SDA)==1 ) 
   {j=1;}
      else 
   {j=0;}
      k=(k<<1)|j;
      //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
   }
    AT24C02_SDA_IO_SET(0);
   delay_nop();
   return(k);

}

u8 IIC_Wait_Ack(void)

{
   uint i=0;
      AT24C02_SDA_IO_SET(1);
   //SCL=1;
   GPIO_SetBits(GPIOB,SCL);
   delay_nop();
   while ((GPIO_ReadInputDataBit(GPIOB,SDA)==1)&&(i<5000))i++;
   //SCL=0;
    GPIO_ResetBits(GPIOB, SCL);
   delay_nop();
   AT24C02_SDA_IO_SET(0);
   return 0;
}



//uchar WIN24C02_read(uint address)

//{
//   uchar i;
//   start();
//   writex(0xa0);
//   clock();
//   writex(address);
//   clock();
//   start();
//   writex(0xa1);
//   clock();
//   i=readx();
//   stop();
//   //delay2(10);
//   delay2(50);
//   return(i);

//}

//void WIN24C02_write(uint address,uchar info)

//{

//   start();
//   writex(0xa0);
//   clock();
//   writex(0);
//   writex(address);
//   clock();
//   writex(info);
//   clock();
//   stop();
//   //delay2(50);
//   delay2(250);

//}

