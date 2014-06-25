#ifndef HGL_NETWORK_HTTP_INPUT_STREAM_INCLUDE
#define HGL_NETWORK_HTTP_INPUT_STREAM_INCLUDE

#include<hgl/io/InputStream.h>
#include "TCPClient.h"
namespace hgl
{
	namespace network
	{
		/**
		* HTTPInputStream流是一个针对HTTP服务器的流式访问类，用它可以从HTTP服务器上下载文件，它从InputStream类派生。<br>
		* 需要注意的是，这个类只能读，不能写。Position和Size也只能读不能修改<br>
		*/
		class HTTPInputStream:public io::InputStream												///HTTP流式访问类
		{
			TCPClient *tcp;

		private:

			char *http_header;
			int http_header_size;

			int PraseHttpHeader();

		protected:

			int64 pos;
			int64 filelength;

		public:

			HTTPInputStream();
			~HTTPInputStream();

			bool Open(const UTF8String &,const UTF8String &);										///<打加一个网址
			void Close();																			///<

			int64	Read(void *,int64)=0;															///<读取数据
			int64	Peek(void *,int64)=0;															///<预览数据
			int64	ReadFully(void *buf,int64 buf_size){return Read(buf,buf_size);}					///<充分读取,保证读取到指定长度的数据(不计算超时)

			bool	CanRestart()const{return false;}												///<是否可以复位
			bool	CanSeek()const{return false;}													///<是否可以定位
			bool	CanSize()const{return false;}													///<是否可以取得尺寸
			bool	CanPeek()const{return false;}													///<是否可以预览数据

			bool	Restart(){return false;}														///<复位访问指针
			int64	Skip(int64);																	///<跳过指定字节不访问
			int64	Seek(int64,SeekOrigin=soBegin){return false;}									///<移动访问指针
			int64	Tell()const;																	///<返回当前访问位置
			int64	GetSize()const;																	///<取得流长度
			int64	Available()const;																///<剩下的可以不受阻塞访问的字节数
		};//class HTTPInputStream
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_NETWORK_HTTP_INPUT_STREAM_INCLUDE
