#include<hgl/ConsoleServer.h>

using namespace hgl;
using namespace hgl::network;

class HTTPThread:public Thread
{
	int sock;

	char buf[1024];
	int buf_size;
	int cur_size;

public:

	HTTPThread(int s,const sockaddr_in &)
	{
		sock=s;
	}

	~HTTPThread()
	{
		close(sock);
	}

	bool Execute()// override
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
};//class HTTPThread

bool InitHTTPServer(const hgl::OSStringList &args)
{
	if(args.Count()<3)
	{
		std::cout<<"Example: DownloadHTTPServer 0.0.0.0 80"<<std::endl;
		return(false);
	}

	return(true);
}

HGL_CONSOLE_TCP_SERVER_APPLICATION("纯文件下载HTTP服务器",
								   "DownloadHTTPServer",
								   InitHTTPServer,				//全局初始化函数
								   HTTPThread,					//工作线程
								   args[1],						//监听IP
								   ToInt(args[2].c_str()));		//端口
