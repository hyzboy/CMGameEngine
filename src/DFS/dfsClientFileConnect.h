#ifndef HGL_DFS_CLIENT_FILE_CONNECT_INCLUDE
#define HGL_DFS_CLIENT_FILE_CONNECT_INCLUDE

#include<hgl/dfs/dfsClientConnect.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/type/Pool.h>
namespace hgl
{
	namespace dfs
	{
		class dfsClientFileConnect:public dfsClientConnect
		{
			UTF8String set_name;

		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using dfsClientConnect::dfsClientConnect;
#else
			dfsClientFileConnect():dfsClientConnect(){}
#endif//HGL_CONSTRUCTION_REUSE

			void SetName(const UTF8String &gn)
			{
				set_name=gn;
			}

			bool SendPrivateLogin(io::DataOutputStream *dos)
			{
				return dos->WriteUTF8String(set_name);
			}
		};//class dfsClientFileConnect

		class dfsClientFileConnectPool:public MTObjectPool<dfsClientFileConnect>
		{
			UTF8String set_name;

			UTF8String ip;
			uint port;
			UTF8String node_name;
			int64 node_id;

		private:

			dfsClientFileConnect *Create()
			{
				dfsClientFileConnect *obj=new dfsClientFileConnect();

				obj->SetName(set_name);

				if(!obj->Init(ip,port,node_name,node_id))
				{
					delete obj;
					return NULL;
				}

				return obj;
			}

		public:

			dfsClientFileConnectPool(const UTF8String &sn,const UTF8String &i,uint p,const UTF8String &nn,int64 ni)
			{
				set_name	=sn;

				ip			=i;
				port		=p;
				node_name	=nn;
				node_id		=ni;
			}
		};//class dfsClientFileConnectPool
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_CLIENT_FILE_CONNECT_INCLUDE
