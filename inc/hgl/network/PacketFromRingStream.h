#ifndef HGL_NETWORK_TCP_SOCKET_RING_BUFFER_PACKET_RECV_INCLUDE
#define HGL_NETWORK_TCP_SOCKET_RING_BUFFER_PACKET_RECV_INCLUDE

#include<hgl/thread/RingBuffer.h>
#include<hgl/io/MemoryInputStream.h>
#include<hgl/MemBlock.h>

using namespace hgl::io;

namespace hgl
{
	/**
	 * 从环形流中获取包模板类
	 */
	template<typename DIS> class PacketFromRingStream
	{
		RingInputStream *ris;
		MemBlock<char> mb;

		MemoryInputStream *mis;
		DIS *dis;

	public:

		PacketFromRingStream(RingInputStream *_ris,int init_size=HGL_SIZE_1MB)
		{
			ris=_ris;
			mis=new MemoryInputStream();
			dis=new DIS(mis);

			mb.SetLength(init_size);
		}

		virtual ~PacketFromRingStream()
		{
			SAFE_CLEAR(dis);
			SAFE_CLEAR(mis);
		}

		char *GetData()
		{
			return mb.GetData();
		}

		size_t GetSize()
		{
			return mb.GetLength();
		}

		DIS *GetInputStream()
		{
			return dis;
		}

		/**
		* 获取数据输入流
		* @param recv_size 成功获取字节数
		* @param left_bytes 剩余字节数
		* @return 是否成功
		*/
		bool Recv(int &recv_size,int &left_bytes)
		{
			if(!ris)
			{
				recv_size=-1;
				return(false);
			}

			RingBufferSafeRead rbsr(ris);

			const int recv_max=rbsr.TryStart();

			if(recv_max<=0)
			{
				recv_size=0;
				return(false);
			}

			if(recv_max<sizeof(uint32))
			{
				recv_size=0;
				left_bytes=recv_max;
				return(false);
			}

			uint32 size;

			ris->Peek(mb.data(),sizeof(uint32));
			mis->Link(mb.data(),sizeof(uint32));

			if(!dis->ReadUint32(size))
			{
				recv_size=-1;
				left_bytes=recv_max;
				return(false);
			}

			if(size==0)			//心跳包
			{
				ris->Skip(sizeof(uint32));
				recv_size=sizeof(uint32);
				left_bytes=recv_max-sizeof(uint32);
				return(false);
			}

			if(recv_max<sizeof(uint32)+size)
			{
				recv_size=0;
				left_bytes=recv_max;
				return(false);
			}

			mb.SetLength(size);

			ris->Skip(sizeof(uint32));
			ris->Read(mb.data(),size);
			mis->Link(mb.data(),size);

			recv_size=sizeof(uint32)+size;
			left_bytes=recv_max-sizeof(uint32)-size;

			return(true);
		}
	};//class PacketFromRingStream
}//namespace hgl
#endif//HGL_NETWORK_TCP_SOCKET_RING_BUFFER_PACKET_RECV_INCLUDE
