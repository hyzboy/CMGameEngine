#ifndef HGL_DFS_CLIENT_CONNECT_INCLUDE
#define HGL_DFS_CLIENT_CONNECT_INCLUDE

#include<hgl/type/BaseString.h>

namespace hgl
{
	namespace io
	{
		class InputStream;
		class OutputStream;

		class DataInputStream;
		class DataOutputStream;
	}

	using namespace io;

	class ThreadMutex;

	namespace dfs
	{
		struct dfsClientTCP;

		/**
		 * dfs客户端网络连接处理基类
		 */
		class dfsClientConnect																		///dfs客户端网络连接处理基类
		{
		protected:

			UTF8String ip;
			uint port;

			int64 node_id;
			UTF8String node_name;
			UTF8String server_node_name;

			dfsClientTCP *tcp;

		protected:

			ThreadMutex *lock;

			virtual bool SendPrivateLogin(DataOutputStream *){return true;}							///<发送私有Login数据
			bool Reconnect();

		public:

			void Disconnect();																		///<断开连接

		public:

			dfsClientConnect();
			virtual ~dfsClientConnect();

			virtual bool Init(const UTF8String &,uint,const UTF8String &,const int64 _node_id=-1);	///<初始化dfs客户端连接

			const	UTF8String &GetIP		()const{return ip;}
					uint		GetPort		()const{return port;}
			const	UTF8String &GetNodeName	()const{return node_name;}
					int64		GetNodeID	()const{return node_id;}

			ThreadMutex *GetLock(){return lock;}

			void Lock();
			void Unlock();

			bool Use();																				///<使用这个连接

			DataInputStream *GetDIS();
			DataOutputStream *GetDOS();
		};//class dfsConnect
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_CLIENT_CONNECT_INCLUDE
