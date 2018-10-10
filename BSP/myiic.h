#ifndef __MYIIC_H
#define __MYIIC_H
#include "stm320xx8_rtu.h"
//////////////////////////////////////////////////////////////////////////////////	 
//
//STM32开发板
//IIC驱动 代码	   
//
//
//修改日期:2012/9/9
//版本：V1.0
//
// 
//									  
//////////////////////////////////////////////////////////////////////////////////
extern void AT24C02_SDA_IO_SET(unsigned char io_set);
//IO方向设置
#define SDA_IN   AT24C02_SDA_IO_SET(1)
#define SDA_OUT  AT24C02_SDA_IO_SET(0)

//IO操作函数	 
#define IIC_SCL(x)    GPIO_WriteBit(GPIOB,GPIO_Pin_6,x) //SCL
#define IIC_SDA(x)    GPIO_WriteBit(GPIOB,GPIO_Pin_7,x) //SDA	 
#define READ_SDA      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















