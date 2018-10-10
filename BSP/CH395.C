/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395������ʾ
**********************************************************************************/

/**********************************************************************************
CH395 TCP/IP Э����ӿ�
MSC51 ��ʾ����������ʾSocket0������TCP Clientģʽ�£���Ƭ���յ����ݺ󣬰�λȡ����
�ϴ���MCS51@24MHZ,KEIL 3.51
**********************************************************************************/
/* ͷ�ļ�����*/
//#include <reg52.h>
#include "stdio.h"
#include "string.h"
#include "CH395INC.H"
#include "CH395.H"
#include "CH395CMD.H"
#include "CH395UART.h"	

/**********************************************************************************/
/* ���������ļ� */


#define    CH395_DEBUG           0
#define DEF_KEEP_LIVE_IDLE                           (15*1000)        /* ����ʱ�� */
#define DEF_KEEP_LIVE_PERIOD                         (20*1000)        /* ���Ϊ15�룬����һ��KEEPLIVE���ݰ� */                  
#define DEF_KEEP_LIVE_CNT                            200                /* ���Դ���  */

/* ���ñ������� */
UINT8  MyBuffer[512];                                           /* ���ݻ����� */
struct _SOCK_INF  SockInf;                                      /* ����Socket��Ϣ */
struct _CH395_SYS  CH395Inf;                                    /* ����CH395��Ϣ */

/* CH395��ض��� */
const UINT8 CH395IPAddr[4] = {192,168,1,7};                         /* CH395IP��ַ */
const UINT8 CH395GWIPAddr[4] = {192,168,1,1};                        /* CH395���� */
const UINT8 CH395IPMask[4] = {255,255,255,0};                        /* CH395�������� */

/* socket ��ض���*/
const UINT8  Socket0DesIP[4] = {192,168,1,33};                      /* Socket 0Ŀ��IP��ַ */
const UINT16 Socket0DesPort = 1031;                                  /* Socket 0Ŀ�Ķ˿� */
const UINT16 Socket0SourPort = 1030;                                 /* Socket 0Դ�˿� */

UINT8 RXdata[40];                        /* CH395�������� */


/**********************************************************************************
* Function Name  : mStopIfError
* Description    : ����ʹ�ã���ʾ������룬��ͣ��
* Input          : iError
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(UINT8 iError)
{
    if (iError == CMD_ERR_SUCCESS) return;                           /* �����ɹ� */
#if CH395_DEBUG
    printf("Error: %02X\n", (UINT16)iError);                         /* ��ʾ���� */
#endif
    while ( 1 ) 
    {
        mDelaymS(200);
        mDelaymS(200);
    }
}

/**********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : ��ʼ��CH395Inf����
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* ��CH395Infȫ������*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* ��IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* ������IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* ����������д��CH395Inf�� */
}

/**********************************************************************************
* Function Name  : InitSocketParam
* Description    : ��ʼ��socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitSocketParam(void)
{
	  memset(&SockInf,0,sizeof(SockInf));                              /* ��SockInf[0]ȫ������*/
    memcpy(SockInf.IPAddr,Socket0DesIP,sizeof(Socket0DesIP));        /* ��Ŀ��IP��ַд�� */
    SockInf.DesPort = Socket0DesPort;                                /* Ŀ�Ķ˿� */
    SockInf.SourPort = Socket0SourPort;                              /* Դ�˿� */
    SockInf.ProtoType = PROTO_TYPE_UDP;                              /* TCPģʽ */
	
//    memset(&SockInf,0,sizeof(SockInf));                              /* ��SockInf[0]ȫ������*/
//    memcpy(SockInf.IPAddr,Socket0DesIP,sizeof(Socket0DesIP));        /* ��Ŀ��IP��ַд�� */
//    SockInf.DesPort = Socket0DesPort;                                /* Ŀ�Ķ˿� */
//    SockInf.SourPort = Socket0SourPort;                              /* Դ�˿� */
//    SockInf.ProtoType = PROTO_TYPE_TCP;                              /* TCPģʽ */
//    SockInf.TcpMode = TCP_CLIENT_MODE;
}

/**********************************************************************************
* Function Name  : CH395SocketInitOpen
* Description    : ����CH395 socket ��������ʼ������socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInitOpen(void)
{
       UINT8 i;

    /* socket 0ΪTCP �ͻ���ģʽ */
    CH395SetSocketDesIP(0,SockInf.IPAddr);                           /* ����socket 0Ŀ��IP��ַ */         
    CH395SetSocketProtType(0,SockInf.ProtoType);                     /* ����socket 0Э������ */
    CH395SetSocketDesPort(0,SockInf.DesPort);                        /* ����socket 0Ŀ�Ķ˿� */
    CH395SetSocketSourPort(0,SockInf.SourPort);                      /* ����socket 0Դ�˿� */
	
	  i = CH395CloseSocket(0); 
	 // mStopIfError(i);
	
    i = CH395OpenSocket(0);                                          /* ��socket 0 */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
	
//    i = CH395TCPConnect(0);                                          /* TCP���� */
//    mStopIfError(i);                                                  /* ����Ƿ�ɹ� */
}



/*********************************************************************************
* Function Name  : keeplive_set
* Description    : ���ʱ����������
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void keeplive_set(void)
{
	CH395KeepLiveCNT(DEF_KEEP_LIVE_CNT);
	CH395KeepLiveIDLE(DEF_KEEP_LIVE_IDLE);
	CH395KeepLiveINTVL(DEF_KEEP_LIVE_PERIOD);
}

/**********************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket �ж�,��ȫ���ж��б�����
* Input          : sockindex
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInterrupt(UINT8 sockindex)
{
   UINT8  sock_int_socket;
   UINT8  i;
   UINT16 len;
   UINT16 tmp;

   sock_int_socket = CH395GetSocketInt(sockindex);                   /* ��ȡsocket ���ж�״̬ */
   if(sock_int_socket & SINT_STAT_SENBUF_FREE)                       /* ���ͻ��������У����Լ���д��Ҫ���͵����� */
   {
		  if(SockInf.SendLen >= SockInf.RemLen)SockInf.RemLen = 0;      /* ???????? */
       else
       {
            SockInf.pSend += SockInf.SendLen;                        /* ????????? */
            SockInf.RemLen -= SockInf.SendLen;                       /* ?????? */
            if(SockInf.RemLen > 2048)SockInf.SendLen = 2048;         /* ????????????? */
            else SockInf.SendLen = SockInf.RemLen;     
            CH395SendData(sockindex,SockInf.pSend,SockInf.SendLen); /* ???? */
       }
   }
   if(sock_int_socket & SINT_STAT_SEND_OK)                           /* ��������ж� */
   {
   }
   if(sock_int_socket & SINT_STAT_RECV)                              /* �����ж� */
   {
       len = CH395GetRecvLength(sockindex);                          /* ��ȡ��ǰ�����������ݳ��� */
#if CH395_DEBUG
       printf("receive len = %d\n",len);
#endif
       if(len == 0)return;
       if(len > 512)len = 512;                                       /* MyBuffer����������Ϊ512 */
       CH395GetRecvData(sockindex,len,MyBuffer);                     /* ��ȡ���� */
       for(tmp =0; tmp < len; tmp++)                                 /* ���������ݰ�λȡ�� */
       {
          MyBuffer[tmp] = MyBuffer[tmp];
       }
       CH395SendData(sockindex,MyBuffer,len);

   }
	if(sock_int_socket & SINT_STAT_CONNECT)                          /* �����жϣ�����TCPģʽ����Ч*/
	{
		CH395SetKeepLive(sockindex,1);								 /*��KEEPALIVE���ʱ��*/
		CH395SetTTLNum(sockindex,40); 								 /*����TTL*/
	}
   /*
   **�����Ͽ������жϺͳ�ʱ�ж�ʱ��CH395Ĭ���������ڲ������رգ��û�����Ҫ�Լ��رո�Socket����������óɲ������ر�Socket��Ҫ����
   **SOCK_CTRL_FLAG_SOCKET_CLOSE��־λ��Ĭ��Ϊ0��������ñ�־Ϊ1��CH395�ڲ�����Socket���йرմ������û��������жϺͳ�ʱ�ж�ʱ����
   **CH395CloseSocket������Socket���йرգ�������ر����SocketһֱΪ���ӵ�״̬����ʵ���Ѿ��Ͽ������Ͳ�����ȥ�����ˡ�
   */
	if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* �Ͽ��жϣ�����TCPģʽ����Ч */
	{
//		i = CH395CloseSocket(sockindex);                             
//		mStopIfError(i);
	}
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* ��ʱ�ж� */
   {   
		//printf("time out \n");
//		i = CH395CloseSocket(sockindex);                             
//		mStopIfError(i);
	    
   }
}

/**********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395ȫ���жϺ���
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395GlobalInterrupt(void)
{
   UINT16  init_status;
   UINT8   buf[10]; 
 

	init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                              /* ���ɴ��жϣ���ȡ���ɴ���Ϣ */
    {
        CH395CMDGetUnreachIPPT(buf);                                
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* ����IP��ͻ�жϣ����������޸�CH395�� IP������ʼ��CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* ����PHY�ı��ж�*/
    {
#if CH395_DEBUG
        printf("Init status : GINT_STAT_PHY_CHANGE\n");
#endif
    }
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* ����socket 0�ж�*/
    }
    if(init_status & GINT_STAT_SOCK1)                                
    {
        CH395SocketInterrupt(1);                                     /* ����socket 1�ж�*/
    }
    if(init_status & GINT_STAT_SOCK2)                                
    {
        CH395SocketInterrupt(2);                                     /* ����socket 2�ж�*/
    }
    if(init_status & GINT_STAT_SOCK3)                                
    {
        CH395SocketInterrupt(3);                                     /* ����socket 3�ж�*/
    }
    if(init_status & GINT_STAT_SOCK4)
    {
        CH395SocketInterrupt(4);                                     /* ����socket 4�ж�*/
    }
    if(init_status & GINT_STAT_SOCK5)                                
    {
        CH395SocketInterrupt(5);                                     /* ����socket 5�ж�*/
    }
    if(init_status & GINT_STAT_SOCK6)                                
    {
        CH395SocketInterrupt(6);                                     /* ����socket 6�ж�*/
    }
    if(init_status & GINT_STAT_SOCK7)                                
    {
        CH395SocketInterrupt(7);                                     /* ����socket 7�ж�*/
    }
}

/*********************************************************************************
* Function Name  : CH395Init
* Description    : ����CH395��IP,GWIP,MAC�Ȳ���������ʼ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
**********************************************************************************/
UINT8  CH395Init(void)
{
      UINT8 i;
    
    i = CH395CMDCheckExist(0x01);                      
    if(i != 0xFE)return CH395_ERR_UNKNOW;                            /* �����������޷�ͨ������0XFA */
                                                                     /* ����0XFAһ��ΪӲ��������߶�дʱ�򲻶� */
  CH395CMDReset();
	   mDelaymS(200);   
#if UART_WORK_BAUDRATE
    CH395CMDSetUartBaudRate(UART_WORK_BAUDRATE);                     /* ���ò����� */   
    mDelaymS(1);
    SetMCUBaudRate();
#endif

    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* ����CH395��IP��ַ */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* �������ص�ַ */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* �����������룬Ĭ��Ϊ255.255.255.0*/   
    mDelaymS(10);
    i = CH395CMDInitCH395();                                         /* ��ʼ��CH395оƬ */
    return i;
}

/**********************************************************************************
* Function Name  : mInitSTDIO
* Description    : ���ڳ�ʼ��,������ʹ��
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void mInitSTDIO( void )
{

}

/**********************************************************************************
* Function Name  : main
* Description    : main������
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395_PRO(void)
{
    UINT8 i;
 
    mDelaymS(100);
    CH395_PORT_INIT();
	  i=CH395CMDGetVer();
    InitCH395InfParam();                                             /* ��ʼ��CH395��ر��� */
    i = CH395Init();                                                 /* ��ʼ��CH395оƬ */
    mStopIfError(i);
     
	keeplive_set(); 												/* ���ʱ����������  */   
   while(1)
   {                                                                 /* �ȴ���̫�����ӳɹ�*/
       if(CH395CMDGetPHYStatus() == PHY_DISCONN)                     /* ��ѯCH395�Ƿ����� */
       {
           mDelaymS(200);                                            /* δ������ȴ�200MS���ٴβ�ѯ */
       }
       else 
       {
           break;
       }
   }
	InitSocketParam();                                                /* ��ʼ��socket��ر��� */
	CH395SocketInitOpen();
// //  CH395CMDGetUnreachIPPT(RXdata);
	 while(1)
   {
      if(Query395Interrupt())CH395GlobalInterrupt();
   }
}
