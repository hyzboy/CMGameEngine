#include<hgl/network/SCTPClient.h>
#include<hgl/thread/Thread.h>

using namespace hgl;
using namespace hgl::network;

class SendThread:public Thread
{
	SCTPO2OClient *sctp;
	uint16 stream;

public:

    SendThread(SCTPO2OClient *s,uint16 i)
	{
		sctp=s;
		stream=i;
	}

	bool Execute()
	{
		char buf[1024]="teststr[0,0]";

		int len=strlen(buf)+1;

		buf[8]='0'+stream;

		for(int i=0;i<10;i++)
		{
			if(!sctp->SendMsg(buf,len,stream))
				return(false);

			std::cout<<"send stream["<<stream<<"] : "<<buf<<std::endl;

			++buf[10];
		}

		return(false);
	}
};//class SendThread

int main(int argc,char **argv)
{
	SCTPO2OClient sctp;

	if(!sctp.Connect("127.0.0.1",10000))
	{
		std::cout<<"connect to server failed!"<<std::endl;
		return(-1);
	}

	//创建发送线程
	{
//		for(int i=0;i<1;i++)
		{
			SendThread *st=new SendThread(&sctp,0);

			st->Execute();
		}
	}

	MemBlock<char> mb;
	uint16 stream;
	int count=0;

	for(;;)
	{
		if(!sctp.RecvMsg(&mb,stream))
		{
			//std::cout<<"recv msg from server failed!"<<std::endl;
			break;
		}

		++count;
		std::cout<<"recv "<<count<<".stream:"<<stream<<","<<mb.GetData()<<std::endl;
	}

	sctp.CloseSocket();
	return(0);
}
