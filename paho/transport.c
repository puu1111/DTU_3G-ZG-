/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - "commonalization" from prior samples and/or documentation extension
 *******************************************************************************/

#include "transport.h"

#if !defined(SOCKET_ERROR)
	/** error in socket operation */
	#define SOCKET_ERROR -1
#endif

#define INVALID_SOCKET SOCKET_ERROR
#include "3gmodule.h"
#include <stdio.h>

/**
This simple low-level implementation assumes a single connection for a single thread. Thus, a static
variable is used for that connection.
On other scenarios, the user must solve this by taking into account that the current implementation of
MQTTPacket_read() has a function pointer for a function call to get the data to a buffer, but no provisions
to know the caller or other indicator (the socket id): int (*getfn)(unsigned char*, int)
*/
static int mysock = INVALID_SOCKET;
extern uint8_t TxBuffer[];
extern BOOL runningFlag;
int transport_sendPacketBuffer(unsigned char* buf, int buflen)
{
    char cmd[18];
    int rc = 0;
   
//    sprintf(cmd, "AT+QSSLSEND=0,%d", buflen);
//    // bsp_DelayMS(500);
//    if(FALSE == sendATCommand(cmd,">",1,0))
//         rc = 1;
//  
//	if(FALSE == sendPacket(buf, "SEND OK", buflen))
//        rc = 2;
  
  
  if(FALSE == sendPacket(buf, "NO CARRIER", buflen))  rc = 2;
   if(FALSE == transport_close(  ))
        rc = 3;
	return rc;
}


int transport_getdata(unsigned char* buf, int count)
{
//	int rc = recv(mysock, buf, count, 0);
//	printf("received %d bytes count %d\n", rc, (int)count);
	return 0;
}

int transport_getdatanb(void *sck, unsigned char* buf, int count)
{
//	int sock = *((int *)sck); 	/* sckif: pointer to whatever the system may use to identify the transport */
//	/* this call will return after the timeout set on initialization if no bytes;
//	   in your system you will use whatever you use to get whichever outstanding
//	   bytes your socket equivalent has ready to be extracted right now, if any,
//	   or return immediately */
//	int rc = recv(sock, buf, count, 0);
//	if (rc == -1) {
//		/* check error conditions from your system here, and return -1 */
//		return 0;
//	}
	return 0;
}

/**
return >=0 for a socket descriptor, <0 for an error code
@todo Basically moved from the sample without changes, should accomodate same usage for 'sock' for clarity,
removing indirections
*/
int transport_open(char* addr, int port)
{
    char cmd[70];// = malloc(80);

    if(NULL != cmd)
    {
        sprintf(cmd, "AT+QSSLOPEN=1,1,0,\"%s\",%d,2", addr, port);
      if(FALSE == sendATCommand(cmd, "CONNECT", 1,0))  
     // if(FALSE == sendATCommand(cmd, "OK", 1,3))
				{	
            transport_close(  );
					  mysock=FALSE;
				}	
        else
            mysock = TRUE;
        //free(cmd);
    }
	return mysock;
}

int transport_close(void)
{
//    USART_Cmd(USART3, ENABLE);
//   strcpy((char*)TxBuffer, "+++");
//   runningFlag = TRUE;
//   DMA1_Channel7->CCR &= (uint16_t)(~0x01);
//	 DMA1_Channel7->CNDTR = 3;
//	 DMA1_Channel7->CCR |= 0x01;
//	 USART3->CR3=1<<7;	
//   while(runningFlag);
  
    return sendATCommand("AT+QSSLCLOSE=0", "OK", 1,1);
}
