#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

using namespace std;

int main(int argc,char **argv)
{
	if(argc<3)
	{
		cout<<"tcptest ip port"<<endl;
		return(1);
	}

	int port=atoi(argv[2]);

	cout<<"ip: "<<argv[1]<<endl;
	cout<<"port: "<<port<<endl;

	int sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	if(sock<0)
	{
		cout<<"create socket error,errno="<<errno<<endl;
		return(-2);
	}

	sockaddr_in sin;

	memset(&sin,0,sizeof(sockaddr_in));

	sin.sin_family=AF_INET;
	sin.sin_port  =port;
	sin.sin_addr.s_addr=inet_addr(argv[1]);

	if(connect(sock,(struct sockaddr *)&sin,sizeof(sockaddr_in))<0)
	{
		cout<<"connect to "<<argv[1]<<":"<<argv[2]<<" error,errno="<<errno<<endl;
	}
	else
	{
		cout<<"connect to "<<argv[1]<<":"<<argv[2]<<" ok"<<endl;
	}

	close(sock);
	return(0);
}

