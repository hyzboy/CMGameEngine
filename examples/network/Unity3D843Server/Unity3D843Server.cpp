#include<hgl/ConsoleServer.h>

using namespace hgl;
using namespace hgl::network;

class WorkThread:public Thread
{
	int sock;

	char buf[1024];
	int buf_size;
	int cur_size;

public:

	WorkThread(int s,const sockaddr_in &)
	{
		sock=s;
	}

	~WorkThread()
	{
		close(sock);
	}

	bool Execute()// HGL_OVERRIDE
	{
		buf_size=recv(sock,buf,1024,0);

		if(buf_size<=0)
		{
			LOG_HINT(OS_TEXT("Recv size:")+OSString(buf_size));
			return(false);
		}

		{
			buf[buf_size]=0;

			LOG_HINT(U8_TEXT("Recv message:")+buf);
		}



		return(false);
	}
};//class WorkThread

bool Init843Server(const hgl::OSStringList &args)
{
	if(args.Count()<3)
	{
		std::cout<<"Example:
	}
}

HGL_CONSOLE_TCP_SERVER_APPLICATION("纯文件下载HTTP服务器",
								   "DownloadHTTPServer",
								   Init843Server,			//全局初始化函数
								   WorkThread,				//工作线程
								   "0.0.0.0",				//监听IP
								   843);					//端口
