/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395INC.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395Í·ÎÄ¼þ
*                   
**********************************************************************************/

#ifndef   __CH395UART_H__
#define   __CH395UART_H__

#include "CH395INC.H"
/* ********************************************************************************************************************* */

extern void mDelaymS(unsigned char ms);
extern UINT8 Query395Interrupt( void );
extern void CH395_PORT_INIT(void);
extern void xWriteCH395Cmd(unsigned char cmd);
extern void  xWriteCH395Data(unsigned char mdata);
extern unsigned char xReadCH395Data(void);
extern void mDelaymS(unsigned char ms);
extern void mDelayuS(unsigned char us);
#endif

/**************************** endfile *************************************/
