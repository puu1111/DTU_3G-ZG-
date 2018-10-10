
#include <string.h>
#include "MQTTPacket.h"
#include "transport.h"
#include "mqtt.h"
#include "setting.h"
#include "datapacket.h"
#include <stdio.h>
extern char ASCIICCID[17];
#if HW_N6000 || HW_N3030

#elif HW_N6205
uint8_t mqttbuf[600];
//#define M2MECLIPSE
BOOL mqtt_pub(char* payload,uint16_t TxLen)
{
	
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	MQTTString topicString = MQTTString_initializer;
	struct cloudServer tempID;
	char idStr[30]="3a9ec9000001";
	char* Ptemp;
	char* host;
	int port;
	int rc = 0;
	int mysock = 0;
	int buflen = 600;
	int payloadlen = TxLen;//strlen(payload);
	int len = 0;

	//memset(mqttbuf, 0xFF, mqttbuflen);

	
#ifdef M2MECLIPSE
	host = "dtu.mqtt.iot.gz.baidubce.com";
	port =1884; 
	data.clientID.cstring=idStr; 
  topicString.cstring = data.clientID.cstring; //"yzk/control";
#else
	host = csr.name; //"yzk.mqtt.iot.gz.baidubce.com";
	port = csr.port; //1883;
	//csr.CCID[12]=0;
	ASCIICCID[16]=0;
	data.clientID.cstring=ASCIICCID+4;
  topicString.cstring = data.clientID.cstring; //"yzk/control";
#endif
	// data.clientID.cstring=ASCIICCID;

  getRSSI();
	mysock = transport_open(host,port);
	if(mysock <= 0)
  {  
		return FALSE;
  } 

	//sprintf( idStr + 3, "%d", getTimeStamp() % 100000);
/*#ifndef M2MECLIPSE
	memcpy(tempID.username, csr.username, strlen(csr.username));
	Ptemp = (char*)strtok((char*)tempID.username, "/");
	Ptemp = (char*)strtok(NULL, "/");    
	strcpy((char*)idStr, Ptemp);
	 data.clientID.cstring=idStr;
#endif	*/
	
	
	//data.clientID.cstring = idStr;
	/*memcpy(tempID.username, csr.username, strlen(csr.username));
	Ptemp = (char*)strtok((char*)tempID.username, "/");
	Ptemp = (char*)strtok(NULL, "/");    
	strcpy((char*)idStr, Ptemp);
	 data.clientID.cstring=idStr;*/
	data.keepAliveInterval = 10;
	data.cleansession = 1;

	
#ifdef M2MECLIPSE
	data.username.cstring = "dtu/dtu";
	data.password.cstring = "W/uUPcT3Qxk25PvnpSW9XYhCmD5Lfr8lAPfAt9EGgFA=";
#else
	data.username.cstring = csr.username; //"yzk/control";
	data.password.cstring = csr.password; //"wJ8Mz/vmxgWtMq7QZ4P8M7ismt+PdhYceSigVb5p5Ew=";
#endif
	data.MQTTVersion = 4;
  
	len = MQTTSerialize_connect((unsigned char *)mqttbuf, buflen, &data);

	
	strcat((char*)topicString.cstring, "/u");
	len += MQTTSerialize_publish((unsigned char *)(mqttbuf + len), buflen - len, 0, 0, 0, 0, topicString, (unsigned char *)payload, payloadlen);

	len += MQTTSerialize_disconnect((unsigned char *)(mqttbuf + len), buflen - len);

	rc = transport_sendPacketBuffer(mqttbuf, len);
//	free(buf);
	if (rc == 0)
		return TRUE;
	else
		return FALSE;
}

#endif
