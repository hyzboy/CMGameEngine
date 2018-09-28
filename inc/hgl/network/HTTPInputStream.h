#ifndef HGL_NETWORK_HTTP_INPUT_STREAM_INCLUDE
#define HGL_NETWORK_HTTP_INPUT_STREAM_INCLUDE

#include<hgl/io/InputStream.h>
#include<hgl/type/BaseString.h>
#include<hgl/network/IP.h>
namespace hgl
{
	namespace network
	{
        using namespace io;
		class TCPClient;

		/**
		* HTTPInputStream流是一个针对HTTP服务器的流式访问类，用它可以从HTTP服务器上下载文件，它从InputStream类派生。<br>
		* 需要注意的是，这个类只能读，不能写。Position和Size也只能读不能修改<br>
		*/
		class HTTPInputStream:public io::InputStream												///HTTP流式访问类
		{
			TCPClient *tcp;

            InputStream *tcp_is;

		private:

			char *http_header;
            uint http_header_size;

			int PraseHttpHeader();

		protected:

			int64 pos;
			int64 filelength;

		public:

			HTTPInputStream();
			~HTTPInputStream();

            bool    Open(IPAddress *,const UTF8String &);                                           ///<打开一个网址
            void    Close() override;																///<

			int64	Read(void *,int64) override;													///<读取数据
			int64	Peek(void *,int64) override{return 0;}											///<预览数据
			int64	ReadFully(void *buf,int64 buf_size)override{return Read(buf,buf_size);}			///<充分读取,保证读取到指定长度的数据(不计算超时)

			bool	CanRestart()const override{return false;}										///<是否可以复位
			bool	CanSeek()const override{return false;}											///<是否可以定位
			bool	CanSize()const override{return false;}											///<是否可以取得尺寸
			bool	CanPeek()const override{return false;}											///<是否可以预览数据

			bool	Restart() override{return false;}												///<复位访问指针
			int64	Skip(int64) override{return 0;}													///<跳过指定字节不访问
			int64	Seek(int64,SeekOrigin=soBegin) override {return false;}							///<移动访问指针
			int64	Tell()const override{return pos;}												///<返回当前访问位置
			int64	GetSize()const override{return filelength;}										///<取得流长度
			int64	Available()const override{return filelength-pos;}								///<剩下的可以不受阻塞访问的字节数
		};//class HTTPInputStream
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_NETWORK_HTTP_INPUT_STREAM_INCLUDE
