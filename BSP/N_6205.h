#ifndef N_6205_H
#define N_6205_H

#include "HW_SYSConfig.h"

#if HW_N6205

typedef enum
{
	  eCmd_Heart=1,
	  eCmd_Init,
    eCmd_FireAlarm ,             /*!< Not yet started. */
    eCmd_Supervise ,          /*!< Waiting for an event. */
    eCmd_SuperviseOFF,            /*!< An error occurred. */
    eCmd_Troubled,
    eCmd_Normal,
    eCmd_LowVolt,
    eCmd_LowVoltOFF,  
    eCmd_D_Pollute,  
    eCmd_Hornstrobe,
    eCmd_HornstOFF,
    eCmd_Output,
    eCmd_OutputOFF,
    eCmd_Reset,
    eCmd_Check,
    eCmd_Erasure,
    eCmd_SYS_Troubled,
    eCmd_SYS_Normal,
} eCmdMGS;

typedef struct
{
    uint8_t  D_First;
    uint8_t  D_Cnt;
    uint8_t  D_Id[128];//设备编号
    uint8_t  D_type[128];//设备类型
    uint8_t  D_AlarmType[20];//事件码
    uint16_t D_AlarmNum[10];//事件码
    uint8_t  D_AlarmCnt;
    uint8_t  D_AlarmId[10][129];//事件器件号
}stWireless_Base; 

typedef enum
{
	  Heart_STATE_NONE,
	  Heart_STATE_UP,
    Heart_STATE_ON,             /*!< Not yet started. */
    Heart_STATE_WAITING,          /*!< Waiting for an event. */
    Heart_STATE_ERROR,            /*!< An error occurred. */
} eHeartQueryState;

typedef struct
{
	  uint8_t   ucAdd;
	  uint8_t   ucHeartFlag;
    uint8_t   ucFlag;
    uint8_t   ucALLNUM;
    uint8_t   ucOVERNUM;
	  uint8_t   ucRXbuffer[255];
	  uint8_t   ucTXbuffer[256];
} HW_N6205Status;

typedef enum
{
	  ZX_NoneFlag=0,
    ZX_HeartFlag = 0x01,//心跳
	  ZX_DeviceFlag =0x15,
    ZX_RegisterFlag = 0x2,//设备注册
	  ZX_ReSendFlag = 0x03,//发送
	  ZX_EventFlag = 0x1f,//事件
	  ZX_VauleFlag =0x20,//示值
    ZX_QSPVaule =0x30
} HW_N6205_Msg_Enum;


typedef enum
{
	  eEvent_Fire=1,
    eEvent_Trouble = 0x03,//心跳
	  eEvent_TroubleOFF =0x04,
    eEvent_Shield,
	  eEvent_ShieldOFF ,
	  eEvent_Supervise,          /*!< Waiting for an event. */
    eEvent_SuperviseOFF,            /*!< An error occurred. */
    eEvent_ON,            /*!< An error occurred. */
    eEvent_OFF,            /*!< An error occurred. */
    eEvent_Feedback,
    eEvent_Delay=13,
    eEvent_PowerOFF=15,
    eEvent_Auto=17,
    eEvent_Manual,
    eEvent_N_Run,
    eEvent_T_Run,
    eEvent_Pollute=21,
    eEvent_LowVolt=23,
    eEvent_LowVoltOFF,
    eEvent_Hornstrobe,
    eEvent_Hornstroff=26,
    eEvent_DeviceCheck=39,
    eEvent_DeviceCheckoff=40,
    
    eEvent_Output=227,
    eEvent_OutputOFF=228,
    eEvent_Reset=400,
    eEvent_Check=399,
    eEvent_Erasure=398,
    
    eEvent_T_MainPower=201,
    eEvent_N_MainPower=202,
    eEvent_T_StandbyPower=203,
    eEvent_N_StandbyPower=204,
    eEvent_T_Check=231,
    eEvent_N_Check,
    eEvent_T_MLowVolt,
    eEvent_N_MLowVolt, 
    eEvent_T_SLowVolt,
    eEvent_N_SLowVolt,
    eEvent_T_Output,
    eEvent_N_Output,
    eEvent_T_Erasure,
    eEvent_N_Erasure=240,
    
} eEvent_Enum;

extern HW_N6205Status HW_N6205MSG;
extern uint8_t ucN_6205Heart_FLAG;
extern uint8_t ucN_6205Send_FLAG;
extern uint8_t ucN_6205Device_FLAG;
uint8_t N_6205Pro_Decode(uint16_t *N_rxcn,uint8_t *RxBuffer,uint8_t *TxBuffer);
uint16_t makeN6205_CRT_DataPack(char* databuf,uint8_t MSG_TYPE);
#endif

#endif