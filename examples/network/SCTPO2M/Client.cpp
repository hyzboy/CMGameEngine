#include<hgl/network/SCTPClient.h>

using namespace hgl;
using namespace hgl::network;

int main(int argc,char **argv)
{
	SCTPO2MSocket sctp;

	sockaddr_in sa;

	FillAddr(&sa,"127.0.0.1",10000);

	sctp.Create();

	if(!sctp.SendMsg(&sa,argv[1],strlen(argv[1])+1,1))
	{
		std::cout<<"send msg to server failed!"<<std::endl;
		return(-2);
	}

	char buf[1024];
	int len;
	uint16 stream;

	if(!sctp.RecvMsg(sa,buf,1024,len,stream))
	{
		std::cout<<"recv msg from server failed!"<<std::endl;
		return(-3);
	}

	std::cout<<buf<<std::endl;
	return(0);
}
