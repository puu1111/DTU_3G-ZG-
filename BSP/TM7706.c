/*
*********************************************************************************************************
*
*	ģ������ : TM7705 ����ģ��(2ͨ����PGA��16λADC)
*	�ļ����� : bsp_tm7705.c
*	��    �� : V1.0
*	˵    �� : TM7705ģ���CPU֮�����SPI�ӿڡ�����������֧��Ӳ��SPI�ӿں�����SPI�ӿڡ�
*			  ͨ�����л���
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-10-20  armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "spi.h"
#include "main.h"
#include "TM7706.h"

#define SOFT_SPI		/* ������б�ʾʹ��GPIOģ��SPI�ӿ� */
//#define HARD_SPI		/* ������б�ʾʹ��CPU��Ӳ��SPI�ӿ� */

/* ͨ��1��ͨ��2������,���뻺�壬���� */
#define __CH1_GAIN_BIPOLAR_BUF	(GAIN_1 | UNIPOLAR | BUF_EN)
#define __CH2_GAIN_BIPOLAR_BUF	(GAIN_1 | UNIPOLAR | BUF_EN)
#define __CH3_GAIN_BIPOLAR_BUF	(GAIN_1 | UNIPOLAR | BUF_EN)
/*
	TM7705ģ�����ֱ�Ӳ嵽STM32-V5������nRF24L01ģ�����ĸ�ӿ��ϡ�

    TM7705ģ��   STM32F407������
      SCK   ------  PB3/SPI3_SCK
      DOUT  ------  PB4/SPI3_MISO
      DIN   ------  PB5/SPI3_MOSI
      CS    ------  PF7/NRF24L01_CSN
      DRDY  ------  PH7/NRF24L01_IRQ
      RST   ------  PA4/NRF905_TX_EN/NRF24L01_CE/DAC1_OUT	(��λ RESET)
*/

#if !defined(SOFT_SPI) && !defined(HARD_SPI)
 	#error "Please define SPI Interface mode : SOFT_SPI or HARD_SPI"
#endif

#ifdef SOFT_SPI		/* ����SPI */
	/* ����GPIO�˿� */
	#define RCC_SCK 	RCC_AHBPeriph_GPIOB
	#define PORT_SCK	GPIOB
	#define PIN_SCK		GPIO_Pin_13

	#define RCC_DIN 	RCC_AHBPeriph_GPIOB
	#define PORT_DIN	GPIOB
	#define PIN_DIN		GPIO_Pin_14

	#define RCC_DOUT 	RCC_AHBPeriph_GPIOB
	#define PORT_DOUT	GPIOB
	#define PIN_DOUT	GPIO_Pin_15

	#define RCC_CS 		RCC_AHBPeriph_GPIOC
	#define PORT_CS		GPIOC
	#define PIN_CS0		GPIO_Pin_0
	#define PIN_CS1		GPIO_Pin_1
	#define PIN_CS2		GPIO_Pin_2

	#define RCC_DRDY 	RCC_AHBPeriph_GPIOC
	#define PORT_DRDY	GPIOC
	#define PIN_DRDY0	GPIO_Pin_3
	#define PIN_DRDY1	GPIO_Pin_4
	#define PIN_DRDY2	GPIO_Pin_5
	
	#define RCC_X 	RCC_AHBPeriph_GPIOB
	#define PORT_X	GPIOB
	#define PIN_X0	GPIO_Pin_0
	#define PIN_X1	GPIO_Pin_1
	#define PIN_X2	GPIO_Pin_2


	/* ���������0����1�ĺ� */
	//#define RESET_0()	GPIO_ResetBits(PORT_RESET, PIN_RESET)
	//#define RESET_1()	GPIO_SetBits(PORT_RESET, PIN_RESET)

	#define CS0_0()		GPIO_ResetBits(PORT_CS, PIN_CS0)
	#define CS0_1()		GPIO_SetBits(PORT_CS, PIN_CS0)
	
	#define CS1_0()		GPIO_ResetBits(PORT_CS, PIN_CS1)
	#define CS1_1()		GPIO_SetBits(PORT_CS, PIN_CS1)
	
	#define CS2_0()		GPIO_ResetBits(PORT_CS, PIN_CS2)
	#define CS2_1()		GPIO_SetBits(PORT_CS, PIN_CS2)

	#define SCK_0()		GPIO_ResetBits(PORT_SCK, PIN_SCK)
	#define SCK_1()		GPIO_SetBits(PORT_SCK, PIN_SCK)

	#define DI_0()		GPIO_ResetBits(PORT_DIN, PIN_DIN)
	#define DI_1()		GPIO_SetBits(PORT_DIN, PIN_DIN)

	#define DO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_DOUT, PIN_DOUT) == Bit_SET)

	#define DRDY0_IS_LOW()	(GPIO_ReadInputDataBit(PORT_DRDY, PIN_DRDY0) == Bit_RESET)
	#define DRDY1_IS_LOW()	(GPIO_ReadInputDataBit(PORT_DRDY, PIN_DRDY1) == Bit_RESET)
	#define DRDY2_IS_LOW()	(GPIO_ReadInputDataBit(PORT_DRDY, PIN_DRDY2) == Bit_RESET)
#endif

#ifdef HARD_SPI		/* Ӳ��SPI */
	;
#endif

/* ͨ�żĴ���bit���� */
enum
{
	/* �Ĵ���ѡ��  RS2 RS1 RS0  */
	REG_COMM	= 0x00,	/* ͨ�żĴ��� */
	REG_SETUP	= 0x10,	/* ���üĴ��� */
	REG_CLOCK	= 0x20,	/* ʱ�ӼĴ��� */
	REG_DATA	= 0x30,	/* ���ݼĴ��� */
	REG_ZERO_CH1	= 0x60,	/* CH1 ƫ�ƼĴ��� */
	REG_FULL_CH1	= 0x70,	/* CH1 �����̼Ĵ��� */
	REG_ZERO_CH2	= 0x61,	/* CH2 ƫ�ƼĴ��� */
	REG_FULL_CH2	= 0x71,	/* CH2 �����̼Ĵ��� */

	/* ��д���� */
	WRITE 		= 0x00,	/* д���� */
	READ 		= 0x08,	/* ������ */

	/* ͨ�� */
	CH_1		= 0,	/* AIN1+  AIN1- */
	CH_2		= 1,	/* AIN2+  AIN2- */
	COMMON		= 2,	/* AIN1-  AIN1- */
	CH_3		= 3		/* AIN1-  AIN2- */
};

/* ���üĴ���bit���� */
enum
{
	MD_NORMAL		= (0 << 6),	/* ����ģʽ */
	MD_CAL_SELF		= (1 << 6),	/* ��У׼ģʽ */
	MD_CAL_ZERO		= (2 << 6),	/* У׼0�̶�ģʽ */
	MD_CAL_FULL		= (3 << 6),	/* У׼���̶�ģʽ */

	GAIN_1			= (0 << 3),	/* ���� */
	GAIN_2			= (1 << 3),	/* ���� */
	GAIN_4			= (2 << 3),	/* ���� */
	GAIN_8			= (3 << 3),	/* ���� */
	GAIN_16			= (4 << 3),	/* ���� */
	GAIN_32			= (5 << 3),	/* ���� */
	GAIN_64			= (6 << 3),	/* ���� */
	GAIN_128		= (7 << 3),	/* ���� */

	/* ����˫���Ի��ǵ����Զ����ı��κ������źŵ�״̬����ֻ�ı�������ݵĴ����ת�������ϵ�У׼�� */
	BIPOLAR			= (0 << 2),	/* ˫�������� */
	UNIPOLAR		= (1 << 2),	/* ���������� */

	BUF_NO			= (0 << 1),	/* �����޻��壨�ڲ�������������) */
	BUF_EN			= (1 << 1),	/* �����л��� (�����ڲ�������) */

	FSYNC_0			= 0,
	FSYNC_1			= 1		/* ������ */
};

/* ʱ�ӼĴ���bit���� */
enum
{
	CLKDIS_0	= 0x00,		/* ʱ�����ʹ�� ������Ӿ���ʱ������ʹ�ܲ����񵴣� */
	CLKDIS_1	= 0x10,		/* ʱ�ӽ�ֹ �����ⲿ�ṩʱ��ʱ�����ø�λ���Խ�ֹMCK_OUT�������ʱ����ʡ�� */

	/*
		2.4576MHz��CLKDIV=0 ����Ϊ 4.9152MHz ��CLKDIV=1 ����CLK Ӧ�� ��0����
		1MHz ��CLKDIV=0 ���� 2MHz   ��CLKDIV=1 ����CLK ��λӦ��  ��1��
	*/
	CLK_4_9152M = 0x08,
	CLK_2_4576M = 0x00,
	CLK_1M 		= 0x04,
	CLK_2M 		= 0x0C,

	FS_50HZ		= 0x00,
	FS_60HZ		= 0x01,
	FS_250HZ	= 0x02,
	FS_500HZ	= 0x04,

	/*
		��ʮ�š����ӳ�Ӧ�������TM7705 ���ȵķ���
			��ʹ����ʱ��Ϊ 2.4576MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ 84H,��ʱ�������������Ϊ10Hz,��ÿ0.1S ���һ�������ݡ�
			��ʹ����ʱ��Ϊ 1MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ80H, ��ʱ�������������Ϊ4Hz, ��ÿ0.25S ���һ��������
	*/
	ZERO_0		= 0x00,
	ZERO_1		= 0x80
};

static void TM7705_SyncSPI(uint8_t num);
static void TM7705_Send8Bit(uint8_t _data);
static uint8_t TM7705_Recive8Bit(void);
static void TM7705_WriteByte(uint8_t _data,uint8_t num);
static void TM7705_Write3Byte(uint32_t _data,uint8_t num);
static uint8_t TM7705_ReadByte(uint8_t num);
static uint16_t TM7705_Read2Byte(uint8_t num);
static uint32_t TM7705_Read3Byte(uint8_t num);
static void TM7705_WaitDRDY(uint8_t num);
static void TM7705_ResetHard(void);
static void TM7705_Delay(void);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitTM7705
*	����˵��: ����STM32��GPIO��SPI�ӿڣ��������� TM7705
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/


void bsp_InitTM7705(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  uint8_t i,j;
#ifdef SOFT_SPI		/* ����SPI */
	CS0_1();
	CS1_1();
	CS2_1();
	SCK_1();
	DI_1();	
	
	/* ��GPIOʱ�� */
	RCC_AHBPeriphClockCmd(RCC_SCK | RCC_DIN | RCC_DOUT | RCC_CS | RCC_DRDY, ENABLE);
	
	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* ���������費ʹ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN;
	GPIO_Init(PORT_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CS0;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_CS1;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = PIN_CS2;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);


//	GPIO_InitStructure.GPIO_Pin = PIN_RESET;
//	GPIO_Init(PORT_RESET, &GPIO_InitStructure);

	/* ����GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = PIN_DOUT;
	GPIO_Init(PORT_DOUT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DRDY0;
	GPIO_Init(PORT_DRDY, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_DRDY1;
	GPIO_Init(PORT_DRDY, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_DRDY2;
	GPIO_Init(PORT_DRDY, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_X0;
	GPIO_Init(PORT_X, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_X1;
	GPIO_Init(PORT_X, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_X2;
	GPIO_Init(PORT_X, &GPIO_InitStructure);

#endif

	bsp_DelayMS(10);
	
	//TM7705_ResetHard();		/* Ӳ����λ */
	
	/*
		�ڽӿ����ж�ʧ������£������DIN �ߵ�ƽ��д�����������㹻����ʱ�䣨���� 32������ʱ�����ڣ���
		TM7705 ����ص�Ĭ��״̬��
	*/	
	bsp_DelayMS(5);

	TM7705_SyncSPI(0);		/* ͬ��SPI�ӿ�ʱ�� */
  TM7705_SyncSPI(1);
	TM7705_SyncSPI(2);
	bsp_DelayMS(5);

	/* ����ʱ�ӼĴ��� */
	TM7705_WriteByte(REG_CLOCK | WRITE | CH_1,0);			/* ��дͨ�żĴ�������һ����дʱ�ӼĴ��� */
	
	TM7705_WriteByte(CLKDIS_0 | CLK_4_9152M | FS_500HZ,0);	/* ˢ������50Hz */
	
	TM7705_WriteByte(REG_CLOCK | WRITE | CH_1,1);			/* ��дͨ�żĴ�������һ����дʱ�ӼĴ��� */
	
	TM7705_WriteByte(CLKDIS_0 | CLK_4_9152M | FS_500HZ,1);	/* ˢ������50Hz */
	
	TM7705_WriteByte(REG_CLOCK | WRITE | CH_1,2);			/* ��дͨ�żĴ�������һ����дʱ�ӼĴ��� */
	
//	TM7705_WriteByte(CLKDIS_0 | CLK_4_9152M | FS_50HZ,2);	/* ˢ������50Hz */
	TM7705_WriteByte(CLKDIS_0 | CLK_4_9152M | FS_500HZ,2);	/* ˢ������500Hz */
	
	/* ÿ���ϵ����һ����У׼ */
	for(i=0;i<3;i++)
  {
     for(j=1;j<4;j++)
     {
		    	TM7705_CalibSelf(j,i);
		 }		
	}	
	bsp_DelayMS(5);
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Delay
*	����˵��: CLK֮����ӳ٣�ʱ���ӳ�. ����STM32F407  168M��Ƶ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 5; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ResetHard
*	����˵��: Ӳ����λ TM7705оƬ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_ResetHard(void)
{
	//RESET_1();
	bsp_DelayMS(1);	
	//RESET_0();
	bsp_DelayMS(2);
//	RESET_1();
	bsp_DelayMS(1);
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_SyncSPI
*	����˵��: ͬ��TM7705оƬSPI�ӿ�ʱ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_SyncSPI(uint8_t num)
{
	/* AD7705���нӿ�ʧ�����临λ����λ��Ҫ��ʱ500us�ٷ��� */
	switch(num)
	{
		  case 0: CS0_0();break;
		  case 1: CS1_0();break;
		  case 2: CS2_0();break;
	}	
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);	
	switch(num)
	{
		  case 0: CS0_1();break;
		  case 1: CS1_1();break;
		  case 2: CS2_1();break;
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Send8Bit
*	����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
*	��    ��: _data : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_Send8Bit(uint8_t _data)
{
	uint8_t i;

	for(i = 0; i < 8; i++)
	{
		if (_data & 0x80)
		{
			DI_1();
		}
		else
		{
			DI_0();
		}
		SCK_0();
		_data <<= 1;
		TM7705_Delay();
		SCK_1();	
		TM7705_Delay();		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Recive8Bit
*	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static uint8_t TM7705_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;

	for (i = 0; i < 8; i++)
	{
		SCK_0();
		TM7705_Delay();		
		read = read<<1;
		if (DO_IS_HIGH())
		{
			read++;
		}
		SCK_1();		
		TM7705_Delay();
	}
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_WriteByte
*	����˵��: д��1���ֽڡ���CS����
*	��    ��: _data ����Ҫд�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_WriteByte(uint8_t _data,uint8_t num)
{
	switch(num)
	{
		  case 0: CS0_0();break;
		  case 1: CS1_0();break;
		  case 2: CS2_0();break;
	}	
	TM7705_Send8Bit(_data);
	switch(num)
	{
		  case 0: CS0_1();break;
		  case 1: CS1_1();break;
		  case 2: CS2_1();break;
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Write3Byte
*	����˵��: д��3���ֽڡ���CS����
*	��    ��: _data ����Ҫд�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_Write3Byte(uint32_t _data,uint8_t num)
{
	switch(num)
	{
		  case 0: CS0_0();break;
		  case 1: CS1_0();break;
		  case 2: CS2_0();break;
	}	
	TM7705_Send8Bit((_data >> 16) & 0xFF);
	TM7705_Send8Bit((_data >> 8) & 0xFF);
	TM7705_Send8Bit(_data);
	switch(num)
	{
		  case 0: CS0_1();break;
		  case 1: CS1_1();break;
		  case 2: CS2_1();break;
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadByte
*	����˵��: ��ADоƬ��ȡһ���֣�16λ��
*	��    ��: ��
*	�� �� ֵ: ��ȡ���֣�16λ��
*********************************************************************************************************
*/
static uint8_t TM7705_ReadByte(uint8_t num)
{
	uint8_t read;

	switch(num)
	{
		  case 0: CS0_0();break;
		  case 1: CS1_0();break;
		  case 2: CS2_0();break;
	}	
	read = TM7705_Recive8Bit();
	switch(num)
	{
		  case 0: CS0_1();break;
		  case 1: CS1_1();break;
		  case 2: CS2_1();break;
	}	
	
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Read2Byte
*	����˵��: ��2�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��ȡ�����ݣ�16λ��
*********************************************************************************************************
*/
static uint16_t TM7705_Read2Byte(uint8_t num)
{
	uint16_t read;

	switch(num)
	{
		  case 0: CS0_0();break;
		  case 1: CS1_0();break;
		  case 2: CS2_0();break;
	}	
	read = TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	switch(num)
	{
		  case 0: CS0_1();break;
		  case 1: CS1_1();break;
		  case 2: CS2_1();break;
	}	

	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_Read3Byte
*	����˵��: ��3�ֽ�����
*	��    ��: ��
*	�� �� ֵ: ��ȡ�������ݣ�24bit) ��8λ�̶�Ϊ0.
*********************************************************************************************************
*/
static uint32_t TM7705_Read3Byte(uint8_t num)
{
	uint32_t read;

	switch(num)
	{
		  case 0: CS0_0();break;
		  case 1: CS1_0();break;
		  case 2: CS2_0();break;
	}	
	
	read = TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	
	switch(num)
	{
		  case 0: CS0_1();break;
		  case 1: CS1_1();break;
		  case 2: CS2_1();break;
	}	
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_WaitDRDY
*	����˵��: �ȴ��ڲ�������ɡ� ��У׼ʱ��ϳ�����Ҫ�ȴ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void TM7705_WaitDRDY(uint8_t num)
{
	uint32_t i;

	for (i = 0; i < 4000000; i++)
	{
		   if(num==0)
			 { 
					 if (DRDY0_IS_LOW())
					 {
							 break;
					 }
			 }	
			 else if(num==1)
			 { 
					 if (DRDY1_IS_LOW())
					 {
							 break;
					 }
			 }	
       else	
       { 
					 if (DRDY2_IS_LOW())
					 {
							 break;
					 }
			 }					 
  }	 
	if (i >= 4000000)
	{
//		printf("TM7705_WaitDRDY() Time Out ...\r\n");		/* �������. �����Ŵ� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_WriteReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_WriteReg(uint8_t _RegID, uint32_t _RegValue,uint8_t num)
{
	uint8_t bits;

	switch (_RegID)
	{
		case REG_COMM:		/* ͨ�żĴ��� */		
		case REG_SETUP:		/* ���üĴ��� 8bit */
		case REG_CLOCK:		/* ʱ�ӼĴ��� 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH1:	/* CH1 �����̼Ĵ��� 24bit */
		case REG_ZERO_CH2:	/* CH2 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH2:	/* CH2 �����̼Ĵ��� 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* ���ݼĴ��� 16bit */
		default:
			return;
	}

	TM7705_WriteByte(_RegID | WRITE,num);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */

	if (bits == 8)
	{
		TM7705_WriteByte((uint8_t)_RegValue,num);
	}
	else	/* 24bit */
	{
		TM7705_Write3Byte(_RegValue,num);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*	�� �� ֵ: �����ļĴ���ֵ�� ����8λ�ļĴ�����ȡ32λ�βεĵ�8bit
*********************************************************************************************************
*/
uint32_t TM7705_ReadReg(uint8_t _RegID,uint8_t num)
{
	uint8_t bits;
	uint32_t read;

	switch (_RegID)
	{
		case REG_COMM:		/* ͨ�żĴ��� */
		case REG_SETUP:		/* ���üĴ��� 8bit */
		case REG_CLOCK:		/* ʱ�ӼĴ��� 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH1:	/* CH1 �����̼Ĵ��� 24bit */
		case REG_ZERO_CH2:	/* CH2 ƫ�ƼĴ��� 24bit */
		case REG_FULL_CH2:	/* CH2 �����̼Ĵ��� 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* ���ݼĴ��� 16bit */
		default:
			return 0xFFFFFFFF;
	}

	TM7705_WriteByte(_RegID | READ,num);	/* дͨ�żĴ���, ָ����һ����д��������ָ��д�ĸ��Ĵ��� */

	if (bits == 16)
	{
		read = TM7705_Read2Byte(num);
	}
	else if (bits == 8)
	{
		read = TM7705_ReadByte(num);
	}
	else	/* 24bit */
	{
		read = TM7705_Read3Byte(num);
	}
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_CalibSelf
*	����˵��: ������У׼. �ڲ��Զ��̽�AIN+ AIN-У׼0λ���ڲ��̽ӵ�Vref У׼��λ���˺���ִ�й��̽ϳ���
*			  ʵ��Լ 180ms
*	��    ��:  _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_CalibSelf(uint8_t _ch,uint8_t num)
{
	if (_ch == 1)
	{
		/* ��У׼CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1,num);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */		
		TM7705_WriteByte(MD_CAL_SELF | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0,num);/* ������У׼ */
		TM7705_WaitDRDY(num);	/* �ȴ��ڲ�������� --- ʱ��ϳ���Լ180ms */
	}
	else if (_ch == 2)
	{
		/* ��У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2,num);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��2 */
		TM7705_WriteByte(MD_CAL_SELF | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0,num);	/* ������У׼ */
		TM7705_WaitDRDY(num);	/* �ȴ��ڲ��������  --- ʱ��ϳ���Լ180ms */
	}
	else if (_ch == 3)
	{
		/* ��У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_3,num);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��2 */
		TM7705_WriteByte(MD_CAL_SELF | __CH3_GAIN_BIPOLAR_BUF | FSYNC_0,num);	/* ������У׼ */
		TM7705_WaitDRDY(num);	/* �ȴ��ڲ��������  --- ʱ��ϳ���Լ180ms */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_SytemCalibZero
*	����˵��: ����ϵͳУ׼��λ. �뽫AIN+ AIN-�̽Ӻ�ִ�иú�����У׼Ӧ������������Ʋ�����У׼������
*			 ִ����Ϻ󡣿���ͨ�� TM7705_ReadReg(REG_ZERO_CH1) ��  TM7705_ReadReg(REG_ZERO_CH2) ��ȡУ׼������
*	��    ��: _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_SytemCalibZero(uint8_t _ch,uint8_t num)
{
	if (_ch == 1)
	{
		/* У׼CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1,num);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0,num);/* ������У׼ */
		TM7705_WaitDRDY(num);	/* �ȴ��ڲ�������� */
	}
	else if (_ch == 2)
	{
		/* У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2,num);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0,num);	/* ������У׼ */
		TM7705_WaitDRDY(num);	/* �ȴ��ڲ�������� */
	}
		else if (_ch == 2)
	{
		/* У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_3,num);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH3_GAIN_BIPOLAR_BUF | FSYNC_0,num);	/* ������У׼ */
		TM7705_WaitDRDY(num);	/* �ȴ��ڲ�������� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_SytemCalibFull
*	����˵��: ����ϵͳУ׼��λ. �뽫AIN+ AIN-����������ѹԴ��ִ�иú�����У׼Ӧ������������Ʋ�����У׼������
*			 ִ����Ϻ󡣿���ͨ�� TM7705_ReadReg(REG_FULL_CH1) ��  TM7705_ReadReg(REG_FULL_CH2) ��ȡУ׼������
*	��    ��:  _ch : ADCͨ����1��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TM7705_SytemCalibFull(uint8_t _ch,uint8_t num)
{
	if (_ch == 1)
	{
		/* У׼CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1,num);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0,num);/* ������У׼ */
		TM7705_WaitDRDY(num);	/* �ȴ��ڲ�������� */
	}
	else if (_ch == 2)
	{
		/* У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2,num);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0,num);	/* ������У׼ */
		TM7705_WaitDRDY(num);	/* �ȴ��ڲ�������� */
	}
	else if (_ch == 3)
	{
		/* У׼CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_3,num);	/* дͨ�żĴ�������һ����д���üĴ�����ͨ��1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH3_GAIN_BIPOLAR_BUF | FSYNC_0,num);	/* ������У׼ */
		TM7705_WaitDRDY(num);	/* �ȴ��ڲ�������� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: TM7705_ReadAdc1
*	����˵��: ��ͨ��1��2��ADC����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t TM7705_ReadAdc(uint8_t _ch,uint8_t num)
{
	uint8_t i;
	uint16_t read = 0;
	
	/* Ϊ�˱���ͨ���л���ɶ���ʧЧ����2�� */
	for (i = 0; i < 2; i++)
	{
		TM7705_WaitDRDY(num);		/* �ȴ�DRDY����Ϊ0 */		

		if (_ch == 1)
		{
			TM7705_WriteByte(0x38,num);
		}
		else if (_ch == 2)
		{
			TM7705_WriteByte(0x39,num);
		}
		else if (_ch == 3)
		{
			TM7705_WriteByte(0x3B,num);
		}

		read = TM7705_Read2Byte(num);
	}
	return read;	
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/