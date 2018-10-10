#ifndef MODULE3G_H_INCLUDED
#define MODULE3G_H_INCLUDED

#include "stm320xx8_rtu.h"
#include "mbport.h"

BOOL init3GModule(void);
BOOL sendATCommand(const char* cmd, const char* rpl, uint8_t nRN,uint8_t nRN1);
BOOL sendPacket(const uint8_t* buf, const char* rpl, uint16_t len);
BOOL getCurrentTime(char* strTime, int len);
BOOL getRSSI(void);
BOOL            Delay(__IO uint32_t nCount);

#endif /* MODULE3G_H_INCLUDED */
