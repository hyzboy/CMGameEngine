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
			/**
			 * 建立一个HTTP/HTTPS链接，并发送GET操作得到返回信息<br>
			 * 注：返回信息会包含HTTP信息头，用于下载文件时，需自行将信息头去掉
			 */
			int get(io::OutputStream *,const char *);										///<http/https get

			int get(void **ptr,const char *url)
			{
			}
		}//namespace http
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_HTTP_TOOLS_INCLUDE
