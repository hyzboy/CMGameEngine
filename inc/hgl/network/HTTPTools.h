#ifndef HGL_NETWORK_HTTP_TOOLS_INCLUDE
#define HGL_NETWORK_HTTP_TOOLS_INCLUDE

#include<hgl/io/OutputStream.h>
namespace hgl
{
	namespace network
	{
		/**
		 * 网络通信中HTTP/HTTPS所使用的功能实现
		 */
		namespace http
		{
			int get(io::OutputStream *,const char *);										///<http/https get
		}//namespace http
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_HTTP_TOOLS_INCLUDE
