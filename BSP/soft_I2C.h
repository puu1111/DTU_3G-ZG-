#ifndef __soft_i2c_H__
#define __soft_i2c_H__

#include "stm320xx8_rtu.h"
#define uchar unsigned char
#define uint  unsigned int


void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(void);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

//uchar WIN24C02_read(uint address);  //?24c02???address?????????

//void WIN24C02_write(uint address,uchar info);  //?24c02?address??????????info

void WIN24C02_init(void);   //24c02??????

void delay_nop(void);

void delay2(uint x);

//void start(void);

//void stop(void);

//void writex(uchar j);

//uchar readx(void);

//void clock(void);

#endif
