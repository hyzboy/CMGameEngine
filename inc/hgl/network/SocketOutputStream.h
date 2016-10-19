#ifndef HGL_NETWORK_SOCKET_OUTPUT_STREAM_INCLUDE
#define HGL_NETWORK_SOCKET_OUTPUT_STREAM_INCLUDE

#include<hgl/io/OutputStream.h>
namespace hgl
{
	namespace network
	{
		/**
		* Socket输出流，用于TCP／SCTP协议在无包封装处理的情况下
		*/
		class SocketOutputStream:public io::OutputStream
		{
		protected:

			int sock;

			int64 total;            //累计字节数

		public:

			SocketOutputStream(int s=0)
			{
				SetSocket(s);
			}

			~SocketOutputStream()=default;

			void    SetSocket(int s)
			{
				sock=s;
				total=0;
			}

			void	Close(){}																///<关闭输出流

			int64	GetTotal()const{return total;}											///<取得累计字节数

			int64	Write(const void *,int64);												///<向socket中写入指定的字节数
			int64	WriteFully(const void *,int64);											///<充分写入指定字节的数据

			bool	CanRestart()const{return false;}										///<是否可以复位
			bool	CanSeek()const{return false;}											///<是否可以定位
			bool	CanSize()const{return false;}											///<是否可以取得尺寸

			bool	Restart(){return false;}												///<复位访问指针
			int64	Seek(int64,io::SeekOrigin=io::soBegin){return -1;}						///<移动访问指针
			int64	Tell()const{return -1;}													///<返回当前访问位置
			int64	GetSize()const{return -1;}												///<取得流长度
			int64	Available()const{return -1;}											///<剩下的可以不受阻塞写入的字节数
		};//class SocketOutputStream
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_OUTPUT_STREAM_INCLUDE
