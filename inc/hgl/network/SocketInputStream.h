#ifndef HGL_NETWORK_SOCKET_INPUT_STREAM_INCLUDE
#define HGL_NETWORK_SOCKET_INPUT_STREAM_INCLUDE

#include<hgl/io/InputStream.h>
namespace hgl
{
	template<typename T> class MemBlock;

	namespace network
	{
		/**
		* Socket输入流，用于TCP／SCTP协议在无包封装处理的情况下
		*/
		class SocketInputStream:public io::InputStream
		{
		protected:

			int sock;
			MemBlock<char> *mb;

			int64 total;            //累计字节数

		public:

			SocketInputStream(int=0);
			~SocketInputStream();

			void	SetSocket(int s)
			{
				sock=s;
				total=0;
			}

			int64	GetTotal()const{return total;}											///<取得累计字节数

			void	Close(){}																///<关闭输入流

			int64	Read(void *,int64);														///<从socket中读取指定的字节数
			int64	Peek(void *,int64);														///<从socket中读取指定的字节数，但不从缓存队列中删除
			int64	ReadFully(void *,int64);												///<充分读取指定字节的数据

			bool	CanRestart()const{return false;}										///<是否可以复位
			bool	CanSeek()const{return false;}											///<是否可以定位
			bool	CanSize()const{return false;}											///<是否可以取得尺寸
			bool	CanPeek()const{return true;}											///<是否可以预览数据

			bool	Restart(){return false;}												///<复位访问指针
			int64	Skip(int64);															///<跳过指定字节不访问
			int64	Seek(int64,io::SeekOrigin=io::soBegin){return -1;}						///<移动访问指针
			int64	Tell()const{return -1;}													///<返回当前访问位置
			int64	GetSize()const{return -1;}												///<取得流长度
			int64	Available()const{return -1;}											///<剩下的可以不受阻塞访问的字节数
		};//class SocketInputStream
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_INPUT_STREAM_INCLUDE
