/*
//////////////////////////////////////////////////////////////////  
//SDK       post                   (masterz)  
///////////////////////////////////////////////////////////////////////////  
#include       "stdafx.h"  
#include       "winsock.h"  
#pragma       comment(lib,"ws2_32.lib")  
#define       winsock_version       0x0101  
void       main()  
{  
	//I       create           C:\Inetpub\wwwroot\test\test.asp       ,start       the       web       service  
	//start       my       program,       the       result       is       OK.  
	//If       it       works,it       is       written       by       masterz,otherwise       I       don't       know       who       write       it.  
	SOCKADDR_IN       saServer;  
	LPHOSTENT       lphostent;  
	WSADATA       wsadata;  
	SOCKET       hsocket;  
	int       nRet;  
	const       char*       host_name="127.0.0.1";  
	char*       req="POST       /test/test.asp       HTTP/1.0\r\n"  
		"From:       local\r\n"  
		"User-Agent:       post_test/1.0\r\n"  
		"Content-Type:       application/x-www-form-urlencoded\r\n"  
		"Content-Length:       20\r\n\r\n"  
		"type=12345&name=aaaa";  
	if(WSAStartup(winsock_version,&wsadata))  
		printf("can't       initial       socket");  
	lphostent=gethostbyname(host_name);  
	if(lphostent==nullptr)  
		printf("lphostent       is       null");  
	hsocket       =       socket(AF_INET,       SOCK_STREAM,       IPPROTO_TCP);  
	saServer.sin_family       =       AF_INET;  
	//       Use       def.       now,       need       to       handle       general       case  
	saServer.sin_port       =       htons(80);    
	saServer.sin_addr       =       *((LPIN_ADDR)*lphostent->h_addr_list);  
	nRet       =       connect(hsocket,       (LPSOCKADDR)&saServer,       sizeof(SOCKADDR_IN));  
	if       (nRet       ==       SOCKET_ERROR)  
	{  
		printf("can't       connect");  
		closesocket(hsocket);  
		return;  
	}  
	else  
		printf("connected       with       %s\n",host_name);  
	nRet       =       send(hsocket,       req,       strlen(req),       0);  
	if       (nRet       ==       SOCKET_ERROR)  
	{  
		printf("send()       failed");  
		closesocket(hsocket);  

	}  
	else  
		printf("send()       OK\n");  
	char       dest[1000];    
	nRet=1;  
	while(nRet>0)  
	{  
		nRet=recv(hsocket,(LPSTR)dest,sizeof(dest),0);  
		if(nRet>0)  
			dest[nRet]=0;  
		else  
			dest[0]=0;  
		printf("\nReceived       bytes:%d\n",nRet);  
		printf("Result:\n%s",dest);  
	}    
}     */