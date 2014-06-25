#include<sys/time.h>
#include<errno.h>
#include<iostream>
#include<hgl/network/TCPClient.h>
#include<hgl/io/InputStream.h>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::network;

int main(int argc,char **argv)
{
	if(argc<3)
	{
		std::cout<<"RecvTest ip port"<<std::endl;
		return(0);
	}

	struct timespec s,e;
	uint64 st,et;
	TCPClient tcp;
	char buf[1024];
	int port;

	stoi(argv[2],port);

	tcp.Connect(argv[1],port);

	tcp.SetBlock(true,1.0);

	InputStream *is=tcp.GetInputStream();

	while(true)
	{
		clock_gettime(CLOCK_REALTIME,&s);

		int result=is->Read(buf,1024);

		clock_gettime(CLOCK_REALTIME,&e);

		int err=errno;

		st=(s.tv_sec*HGL_NANO_SEC_PER_SEC)+s.tv_nsec;
		et=(e.tv_sec*HGL_NANO_SEC_PER_SEC)+e.tv_nsec;

		std::cout<<"ReadFully(1024) time:"<<et-st<<",errno:"<<err<<std::endl;
	}
}
