#ifndef HGL_CONSOLE_SERVER_INCLUDE
#define HGL_CONSOLE_SERVER_INCLUDE

#include<hgl/Console.h>
#include<hgl/network/TCPServer.h>
#include<hgl/network/SocketThreadServer.h>

namespace hgl
{
	#define HGL_CONSOLE_SERVER_APPLICATION(name,code,server_class,thread_class,ip,port)	using namespace hgl;	\
																						\
																						HGL_CONSOLE_MAIN_FUNC()\
																						{	\
																							sii.info.ProjectName=OS_TEXT(name);	\
																							sii.info.ProjectCode=OS_TEXT(code);	\
																							\
																							if(!app.Init(&sii))	\
																								return(-1);	\
																							\
																							typedef SocketThreadServer<ConsoleFlowObject,server_class,thread_class> ConsoleStartServer;	\
																							\
																							app.SetStart(new ConsoleStartServer(ip,port));	\
																							\
																							return app.Run();	\
																						}

	#define HGL_CONSOLE_TCP_SERVER_APPLICATION(name,code,thread_class,ip,port)	HGL_CONSOLE_SERVER_APPLICATION(name,code,TCPServer,thread_class,ip,port)
// 	#define HGL_CONSOLE_SCTP_SERVER_APPLICATION(name,code,thread_class,ip,port)	HGL_CONSOLE_SERVER_APPLICATION(name,code,SCTPServer,thread_class,ip,port)
}//namespace hgl
#endif//HGL_CONSOLE_SERVER_INCLUDE
