#ifndef HGL_CONSOLE_SERVER_INCLUDE
#define HGL_CONSOLE_SERVER_INCLUDE

#include<hgl/Console.h>
#include<hgl/network/TCPServer.h>
#include<hgl/network/SocketThreadServer.h>

namespace hgl
{
	#define HGL_CONSOLE_SERVER_APPLICATION(name,code,server_class,init_func,thread_class,ip_address)	using namespace hgl;	\
																									\
																									HGL_CONSOLE_MAIN_FUNC()\
																									{	\
																										sii.info.ProjectName=U8_TEXT(name);	\
																										sii.info.ProjectCode=OS_TEXT(code);	\
																										\
																										if(!app.Init(&sii))	\
																											return(-1);	\
																										\
																										using ConsoleStartServer=network::SocketThreadServer<ConsoleFlowObject,server_class,thread_class>;	\
																										\
																										if(!init_func(args))	\
																											return(-2);	\
																										\
																										app.SetStart(new ConsoleStartServer(ip_address));	\
																										\
																										return app.Run();	\
																									}

	#define HGL_CONSOLE_TCP_SERVER_APPLICATION(name,code,init_func,thread_class,ip_address)	HGL_CONSOLE_SERVER_APPLICATION(name,code,TCPServer,init_func,thread_class,ip_address)
// 	#define HGL_CONSOLE_SCTP_SERVER_APPLICATION(name,code,init_func,thread_class,ip_address)	HGL_CONSOLE_SERVER_APPLICATION(name,code,SCTPServer,init_func,thread_class,ip_address)
}//namespace hgl
#endif//HGL_CONSOLE_SERVER_INCLUDE
