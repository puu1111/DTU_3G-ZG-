/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395.C数据结构定义
*                      
**********************************************************************************/

#ifndef __CH395_H__
#define __CH395_H__

#include "stdint.h"

/* TCP模式，0为客户端，1为服务器 */
#define TCP_CLIENT_MODE                                      0
#define TCP_SERVER_MODE                                      1

struct _CH395_SYS
{
    uint8_t   IPAddr[4];                                           /* CH395IP地址 32bit*/
    uint8_t   GWIPAddr[4];                                         /* CH395网关地址 32bit*/
    uint8_t   MASKAddr[4];                                         /* CH395子网掩码 32bit*/
    uint8_t   MacAddr[6];                                          /* CH395MAC地址 48bit*/
    uint8_t   PHYStat;                                             /* CH395 PHY状态码 8bit*/
    uint8_t   MackFilt;                                            /* CH395 MAC过滤，默认为接收广播，接收本机MAC 8bit*/
    uint32_t  RetranCount;                                         /* 重试次数 默认为10次*/
    uint32_t  RetranPeriod;                                        /* 重试周期,单位MS,默认200MS */
    uint8_t   IntfMode;                                            /* 接口模式 */
    uint8_t   UnreachIPAddr[4];                                    /* 不可到达IP */
    uint16_t  UnreachPort;                                         /* 不可到达端口 */
};

struct _SOCK_INF
{
    uint8_t  IPAddr[4];                                           /* socket目标IP地址 32bit*/
    uint8_t  MacAddr[6];                                          /* socket目标地址 48bit*/
    uint8_t  ProtoType;                                           /* 协议类型 */
    uint8_t  ScokStatus;                                          /* socket状态，参考scoket状态定义 */
    uint8_t  TcpMode;                                             /* TCP模式 */
    uint32_t IPRAWProtoType;                                      /* IPRAW 协议类型 */
    uint16_t DesPort;                                             /* 目的端口 */
    uint16_t SourPort;                                            /* 目的端口 */
    uint16_t SendLen;                                             /* 发送数据长度 */
    uint16_t RemLen;                                              /* 剩余长度 */
    uint8_t  *pSend;                                              /* 发送指针 */                                               
}; 

extern void CH395_PRO(void);

#endif


/************************** endfile **************************************/
