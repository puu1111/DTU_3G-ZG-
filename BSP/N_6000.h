#ifndef N_6000_H
#define N_6000_H

#include "HW_SYSConfig.h"

typedef struct
{
    uint8_t   ucSys_Flag;
    uint8_t   ucSys_Address;
    uint8_t   ucDev_Type;
	  uint8_t  ucDev_Address[4];
	  uint16_t  ucDev_Status;
	  uint8_t   ucDev_Notice[31];
	  uint8_t   ucDev_TimeDay[6];
	  uint8_t   ucSend_FLAG;
	  uint8_t   ucMSG_Type;
	  uint8_t   ucSYS_Address;
} HWDeviceStatus;

typedef struct
{
	  uint8_t   ucDecodeRNum;
	  uint8_t   ucDecodeWNum;
    uint8_t   ucDecodeNum;
    uint8_t   ucDEcodeTIM;
    uint8_t   ucDEcodeSTA;
} N_3030DecodeMEM;

extern N_3030DecodeMEM N_3030DM;
extern HWDeviceStatus N_6000DS;
extern HWDeviceStatus N_3030DS;
extern void N_3030ProCtr(void);
extern void Overtime_Pro(uint16_t *RxCount);
extern void N_6000Pro_Decode(uint16_t *N_rxcnt,uint8_t *RxBuffer);
extern uint8_t N_3030Pro(void);
void Decode_CTR(void);
uint8_t Address_Decode(uint8_t *tmpecR);
uint8_t TimeDay_Decode(uint8_t *tmpecR);
uint8_t TimeDay_Decode(uint8_t *tmpecR);
uint8_t Notice_Decode(uint8_t *tmpecR);
uint8_t Type_Decode(uint8_t *tmpecR);									
uint8_t Status_Decode(uint8_t *tmpecR); 
#endif
