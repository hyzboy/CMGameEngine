#ifndef HGL_NETWORK_JAVA_DIRECT_SOCKET_IO_USER_THREAD_INCLUDE
#define HGL_NETWORK_JAVA_DIRECT_SOCKET_IO_USER_THREAD_INCLUDE

#include<hgl/network/DirectSocketIOUserThread.h>
#include<hgl/io/JavaInputStream.h>
#include<hgl/io/JavaOutputStream.h>

namespace hgl
{
	using namespace hgl::io;

	namespace network
	{
		typedef DirectSocketIOUserThread<JavaInputStream,JavaOutputStream> JavaDirectSocketIOUserThread;
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_JAVA_DIRECT_SOCKET_IO_USER_THREAD_INCLUDE
