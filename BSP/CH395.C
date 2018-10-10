/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395功能演示
**********************************************************************************/

/**********************************************************************************
CH395 TCP/IP 协议族接口
MSC51 演示程序，用于演示Socket0工作在TCP Client模式下，单片机收到数据后，按位取反后
上传。MCS51@24MHZ,KEIL 3.51
**********************************************************************************/
/* 头文件包含*/
//#include <reg52.h>
#include "stdio.h"
#include "string.h"
#include "CH395INC.H"
#include "CH395.H"
#include "CH395CMD.H"
#include "CH395UART.h"	

/**********************************************************************************/
/* 包含命令文件 */


#define    CH395_DEBUG           0
#define DEF_KEEP_LIVE_IDLE                           (15*1000)        /* 空闲时间 */
#define DEF_KEEP_LIVE_PERIOD                         (20*1000)        /* 间隔为15秒，发送一次KEEPLIVE数据包 */                  
#define DEF_KEEP_LIVE_CNT                            200                /* 重试次数  */

/* 常用变量定义 */
UINT8  MyBuffer[512];                                           /* 数据缓冲区 */
struct _SOCK_INF  SockInf;                                      /* 保存Socket信息 */
struct _CH395_SYS  CH395Inf;                                    /* 保存CH395信息 */

/* CH395相关定义 */
const UINT8 CH395IPAddr[4] = {192,168,1,7};                         /* CH395IP地址 */
const UINT8 CH395GWIPAddr[4] = {192,168,1,1};                        /* CH395网关 */
const UINT8 CH395IPMask[4] = {255,255,255,0};                        /* CH395子网掩码 */

/* socket 相关定义*/
const UINT8  Socket0DesIP[4] = {192,168,1,33};                      /* Socket 0目的IP地址 */
const UINT16 Socket0DesPort = 1031;                                  /* Socket 0目的端口 */
const UINT16 Socket0SourPort = 1030;                                 /* Socket 0源端口 */

UINT8 RXdata[40];                        /* CH395子网掩码 */


/**********************************************************************************
* Function Name  : mStopIfError
* Description    : 调试使用，显示错误代码，并停机
* Input          : iError
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(UINT8 iError)
{
    if (iError == CMD_ERR_SUCCESS) return;                           /* 操作成功 */
#if CH395_DEBUG
    printf("Error: %02X\n", (UINT16)iError);                         /* 显示错误 */
#endif
    while ( 1 ) 
    {
        mDelaymS(200);
        mDelaymS(200);
    }
}

/**********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : 初始化CH395Inf参数
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* 将CH395Inf全部清零*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* 将IP地址写入CH395Inf中 */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* 将网关IP地址写入CH395Inf中 */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* 将子网掩码写入CH395Inf中 */
}

/**********************************************************************************
* Function Name  : InitSocketParam
* Description    : 初始化socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitSocketParam(void)
{
	  memset(&SockInf,0,sizeof(SockInf));                              /* 将SockInf[0]全部清零*/
    memcpy(SockInf.IPAddr,Socket0DesIP,sizeof(Socket0DesIP));        /* 将目的IP地址写入 */
    SockInf.DesPort = Socket0DesPort;                                /* 目的端口 */
    SockInf.SourPort = Socket0SourPort;                              /* 源端口 */
    SockInf.ProtoType = PROTO_TYPE_UDP;                              /* TCP模式 */
	
//    memset(&SockInf,0,sizeof(SockInf));                              /* 将SockInf[0]全部清零*/
//    memcpy(SockInf.IPAddr,Socket0DesIP,sizeof(Socket0DesIP));        /* 将目的IP地址写入 */
//    SockInf.DesPort = Socket0DesPort;                                /* 目的端口 */
//    SockInf.SourPort = Socket0SourPort;                              /* 源端口 */
//    SockInf.ProtoType = PROTO_TYPE_TCP;                              /* TCP模式 */
//    SockInf.TcpMode = TCP_CLIENT_MODE;
}

/**********************************************************************************
* Function Name  : CH395SocketInitOpen
* Description    : 配置CH395 socket 参数，初始化并打开socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInitOpen(void)
{
       UINT8 i;

    /* socket 0为TCP 客户端模式 */
    CH395SetSocketDesIP(0,SockInf.IPAddr);                           /* 设置socket 0目标IP地址 */         
    CH395SetSocketProtType(0,SockInf.ProtoType);                     /* 设置socket 0协议类型 */
    CH395SetSocketDesPort(0,SockInf.DesPort);                        /* 设置socket 0目的端口 */
    CH395SetSocketSourPort(0,SockInf.SourPort);                      /* 设置socket 0源端口 */
	
	  i = CH395CloseSocket(0); 
	 // mStopIfError(i);
	
    i = CH395OpenSocket(0);                                          /* 打开socket 0 */
    mStopIfError(i);                                                 /* 检查是否成功 */
	
//    i = CH395TCPConnect(0);                                          /* TCP连接 */
//    mStopIfError(i);                                                  /* 检查是否成功 */
}



/*********************************************************************************
* Function Name  : keeplive_set
* Description    : 保活定时器参数设置
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
* Description    : CH395 socket 中断,在全局中断中被调用
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

   sock_int_socket = CH395GetSocketInt(sockindex);                   /* 获取socket 的中断状态 */
   if(sock_int_socket & SINT_STAT_SENBUF_FREE)                       /* 发送缓冲区空闲，可以继续写入要发送的数据 */
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
   if(sock_int_socket & SINT_STAT_SEND_OK)                           /* 发送完成中断 */
   {
   }
   if(sock_int_socket & SINT_STAT_RECV)                              /* 接收中断 */
   {
       len = CH395GetRecvLength(sockindex);                          /* 获取当前缓冲区内数据长度 */
#if CH395_DEBUG
       printf("receive len = %d\n",len);
#endif
       if(len == 0)return;
       if(len > 512)len = 512;                                       /* MyBuffer缓冲区长度为512 */
       CH395GetRecvData(sockindex,len,MyBuffer);                     /* 读取数据 */
       for(tmp =0; tmp < len; tmp++)                                 /* 将所有数据按位取反 */
       {
          MyBuffer[tmp] = MyBuffer[tmp];
       }
       CH395SendData(sockindex,MyBuffer,len);

   }
	if(sock_int_socket & SINT_STAT_CONNECT)                          /* 连接中断，仅在TCP模式下有效*/
	{
		CH395SetKeepLive(sockindex,1);								 /*打开KEEPALIVE保活定时器*/
		CH395SetTTLNum(sockindex,40); 								 /*设置TTL*/
	}
   /*
   **产生断开连接中断和超时中断时，CH395默认配置是内部主动关闭，用户不需要自己关闭该Socket，如果想配置成不主动关闭Socket需要配置
   **SOCK_CTRL_FLAG_SOCKET_CLOSE标志位（默认为0），如果该标志为1，CH395内部不对Socket进行关闭处理，用户在连接中断和超时中断时调用
   **CH395CloseSocket函数对Socket进行关闭，如果不关闭则该Socket一直为连接的状态（事实上已经断开），就不能再去连接了。
   */
	if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* 断开中断，仅在TCP模式下有效 */
	{
//		i = CH395CloseSocket(sockindex);                             
//		mStopIfError(i);
	}
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* 超时中断 */
   {   
		//printf("time out \n");
//		i = CH395CloseSocket(sockindex);                             
//		mStopIfError(i);
	    
   }
}

/**********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395全局中断函数
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395GlobalInterrupt(void)
{
   UINT16  init_status;
   UINT8   buf[10]; 
 

	init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                              /* 不可达中断，读取不可达信息 */
    {
        CH395CMDGetUnreachIPPT(buf);                                
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* 产生IP冲突中断，建议重新修改CH395的 IP，并初始化CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* 产生PHY改变中断*/
    {
#if CH395_DEBUG
        printf("Init status : GINT_STAT_PHY_CHANGE\n");
#endif
    }
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* 处理socket 0中断*/
    }
    if(init_status & GINT_STAT_SOCK1)                                
    {
        CH395SocketInterrupt(1);                                     /* 处理socket 1中断*/
    }
    if(init_status & GINT_STAT_SOCK2)                                
    {
        CH395SocketInterrupt(2);                                     /* 处理socket 2中断*/
    }
    if(init_status & GINT_STAT_SOCK3)                                
    {
        CH395SocketInterrupt(3);                                     /* 处理socket 3中断*/
    }
    if(init_status & GINT_STAT_SOCK4)
    {
        CH395SocketInterrupt(4);                                     /* 处理socket 4中断*/
    }
    if(init_status & GINT_STAT_SOCK5)                                
    {
        CH395SocketInterrupt(5);                                     /* 处理socket 5中断*/
    }
    if(init_status & GINT_STAT_SOCK6)                                
    {
        CH395SocketInterrupt(6);                                     /* 处理socket 6中断*/
    }
    if(init_status & GINT_STAT_SOCK7)                                
    {
        CH395SocketInterrupt(7);                                     /* 处理socket 7中断*/
    }
}

/*********************************************************************************
* Function Name  : CH395Init
* Description    : 配置CH395的IP,GWIP,MAC等参数，并初始化
* Input          : None
* Output         : None
* Return         : 函数执行结果
**********************************************************************************/
UINT8  CH395Init(void)
{
      UINT8 i;
    
    i = CH395CMDCheckExist(0x01);                      
    if(i != 0xFE)return CH395_ERR_UNKNOW;                            /* 测试命令，如果无法通过返回0XFA */
                                                                     /* 返回0XFA一般为硬件错误或者读写时序不对 */
  CH395CMDReset();
	   mDelaymS(200);   
#if UART_WORK_BAUDRATE
    CH395CMDSetUartBaudRate(UART_WORK_BAUDRATE);                     /* 设置波特率 */   
    mDelaymS(1);
    SetMCUBaudRate();
#endif

    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* 设置CH395的IP地址 */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* 设置网关地址 */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* 设置子网掩码，默认为255.255.255.0*/   
    mDelaymS(10);
    i = CH395CMDInitCH395();                                         /* 初始化CH395芯片 */
    return i;
}

/**********************************************************************************
* Function Name  : mInitSTDIO
* Description    : 串口初始化,仅调试使用
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void mInitSTDIO( void )
{

}

/**********************************************************************************
* Function Name  : main
* Description    : main主函数
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
    InitCH395InfParam();                                             /* 初始化CH395相关变量 */
    i = CH395Init();                                                 /* 初始化CH395芯片 */
    mStopIfError(i);
     
	keeplive_set(); 												/* 保活定时器参数设置  */   
   while(1)
   {                                                                 /* 等待以太网连接成功*/
       if(CH395CMDGetPHYStatus() == PHY_DISCONN)                     /* 查询CH395是否连接 */
       {
           mDelaymS(200);                                            /* 未连接则等待200MS后再次查询 */
       }
       else 
       {
           break;
       }
   }
	InitSocketParam();                                                /* 初始化socket相关变量 */
	CH395SocketInitOpen();
// //  CH395CMDGetUnreachIPPT(RXdata);
	 while(1)
   {
      if(Query395Interrupt())CH395GlobalInterrupt();
   }
}

