#ifndef SETTING_H_INCLUDED
#define SETTING_H_INCLUDED

#include "main.h"

struct serial {
	  //int32_t workmode;
    int32_t baudRate;
    int8_t parity;
    int8_t wordLength;
    int8_t stopBits;
};

struct cloudServer {
	  char aCCID[12];
    uint8_t filter_flag;
	  char heartbeat_cycle ;
	  uint16_t indvalue_cycle ;
    char name[52];
    int16_t port;
     char username[33];
   char password[96];
};

struct point {
    int8_t regType;
    int8_t slaveID;
    int16_t startAddr;
    int8_t dataType;
    char pointCode[8];
};

extern uint8_t RxBuffer[];
extern BOOL runningFlag;
extern uint8_t myRN;
extern uint8_t myRN1;
extern BOOL bRecord;

extern struct serial srl;
extern struct cloudServer csr;
extern struct point* pPoints[];
extern uint8_t counter;

uint8_t ReceiveSetting(void);
uint8_t LoadSetting(void);

#endif /* SETTING_H_INCLUDED */
