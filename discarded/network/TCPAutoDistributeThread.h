#ifndef HGL_NETWORK_TCP_AUTO_DISTRIBUTE_THREAD_INCLUDE
#define HGL_NETWORK_TCP_AUTO_DISTRIBUTE_THREAD_INCLUDE

#include<hgl/io/AutoDistributeThread.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/network/TCPSocket.h>
namespace hgl
{
	namespace network
	{
		/**
		 * TCP输入自动分发线程模板类
		 */
		template<typename DIS> class TCPAutoDistributeThread:public io::AutoDistributeThread				///TCP输入自动分发线程模板类
		{
		protected:
			
			TCPSocket *sock;
			DIS *dis;
			
		public:
			
			TCPAutoDistributeThread()
			{
			}
			
			virtual ~TCPAutoDistributeThread()
			{
				SAFE_CLEAR(dis);
			}
			
			virtual bool Init(TCPSocket *_sock,int s,int e,bool block)
			{
				if(!_sock)
					return(false);
				
				if(s>e)
					return(false);
				
				sock=_sock;
				
				io::InputStream *is=sock->GetInputStream();
				
				if(!is)
					return(false);
				
				dis=new DIS(is);
				
				return io::AutoDistributeThread::Init(dis,s,e,block);
			}
		};//template<typename DIS> class TCPAutoDistributeThread:public io::AutoDistributeThread
		
		typedef TCPAutoDistributeThread<io::LEDataInputStream> TCPLEAutoDistributeThread;
		typedef TCPAutoDistributeThread<io::BEDataInputStream> TCPBEAutoDistributeThread;
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_TCP_AUTO_DISTRIBUTE_THREAD_INCLUDE
