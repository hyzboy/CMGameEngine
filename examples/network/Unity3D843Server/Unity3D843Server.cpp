#include<hgl/ConsoleServer.h>
#include<hgl/File.h>

using namespace hgl;
using namespace hgl::network;

static char *file_data=nullptr;
static size_t file_size=0;

class WorkThread:public Thread
{
	int sock;

public:

	WorkThread(int s,const IPAddress *)
	{
		sock=s;
	}

	~WorkThread()
	{
		SetSocketLinger(sock,30);
		CloseSocket(sock);
	}

	bool Execute()// HGL_OVERRIDE
	{
		int pos=0;
		int result;

		while(pos<file_size)
		{
			result=send(sock,file_data+pos,file_size-pos,0);

			if(result<=0)
				return(false);

			pos+=result;
		}

		return(false);
	}
};//class WorkThread

bool Init843Server(const hgl::OSStringList &)
{
	file_size=LoadFileToMemory(OS_TEXT("crossdomain.xml"),(void **)&file_data);

	if(file_size<=0)
		return(false);

	return(true);
}

HGL_CONSOLE_TCP_SERVER_APPLICATION("Unity3DWebPlayer专用843端口服务器",
								   "Unity3D843Server",
								   Init843Server,			//全局初始化函数
								   WorkThread,				//工作线程
								   CreateIPv4TCP(843))	    //监听端口
