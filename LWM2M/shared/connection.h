/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *    
 *******************************************************************************/

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <stdio.h>
//#include <unistd.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include <sys/socket.h>
//#include <sys/stat.h>
#include <liblwm2m.h>

#define LWM2M_STANDARD_PORT_STR "5683"
#define LWM2M_STANDARD_PORT      5683
#define LWM2M_DTLS_PORT_STR     "5684"
#define LWM2M_DTLS_PORT          5684
#define LWM2M_BSSERVER_PORT_STR "5685"
#define LWM2M_BSSERVER_PORT      5685

struct sockaddr {  
      unsigned short sa_family;     /* address family, AF_xxx */  
      char sa_data[14];                  /* 14 bytes of protocol address */  
};  

struct in6_addr  
{  
    union  
    {  
        uint8_t __u6_addr8[16];   // 128 bit  
        #if defined __USE_MISC || defined __USE_GNU  
        uint16_t __u6_addr16[8];  // 64 bit  
        uint32_t __u6_addr32[4];  // 32 bit  
        #endif  
    } __in6_u;  
    #define s6_addr         __in6_u.__u6_addr8  
    #if defined __USE_MISC || defined __USE_GNU  
    # define s6_addr16      __in6_u.__u6_addr16  
    # define s6_addr32      __in6_u.__u6_addr32  
    #endif  
}; 

struct sockaddr_in6 {  
      sa_family_t sin6_family;         /* AF_INET6 */  
      in_port_t sin6_port;               /* transport layer port # */  
      uint32_t sin6_flowinfo;           /* IPv6 traffic class & flow info */  
      struct in6_addr sin6_addr;    /* IPv6 address */  
      uint32_t sin6_scope_id;        /* set of interfaces for a scope */  
};  

typedef struct _connection_t
{
    struct _connection_t *  next;
    int                     sock;
    struct sockaddr_in6     addr;
    size_t                  addrLen;
} connection_t;

int create_socket(const char * portStr, int ai_family);

connection_t * connection_find(connection_t * connList, struct sockaddr_storage * addr, size_t addrLen);
connection_t * connection_new_incoming(connection_t * connList, int sock, struct sockaddr * addr, size_t addrLen);
connection_t * connection_create(connection_t * connList, int sock, char * host, char * port, int addressFamily);

void connection_free(connection_t * connList);

int connection_send(connection_t *connP, uint8_t * buffer, size_t length);

#endif
