#ifndef HGL_DFS_CLIENT_ID_INCLUDE
#define HGL_DFS_CLIENT_ID_INCLUDE

#include<hgl/dfs/dfsClientConnect.h>
namespace hgl
{
	namespace io
	{
		class DataOutputStream;
	}//namespace io

	namespace dfs
	{
		struct IDLockTeam;
		class dfsClientIDConnectPool;

		#define DFS_ID_KEY_SIZE	32		//数据合集ID字节数

		typedef char dfsIDKey[32];

		inline bool StringToKey(dfsIDKey &dsi,const UTF8String &name)
		{
			memset(dsi,0,DFS_ID_KEY_SIZE);

			if(name.Length()>DFS_ID_KEY_SIZE)
				return(false);

			memcpy(dsi,name.c_str(),name.Length());
			return(true);
		}

		inline bool StringToKey(dfsIDKey &dsi,const UTF16String &name)
		{
			memset(dsi,0,DFS_ID_KEY_SIZE);

			u16char *sp=name.c_str();
			char *p=dsi;
			int n=0;

			while(true)
			{
				if(*sp==0)
					break;

				if(*sp<0x100)
				{
					*p=*sp;
					++p;
					++sp;
					++n;
				}
				else
				{
					if(n>=DFS_ID_KEY_SIZE-1)
						break;

					*((u16char *)p)=*sp;
					++p;
					++p;
					++sp;

					n+=2;
				}

				if(n==DFS_ID_KEY_SIZE)break;
			}

			return(true);
		}

		class dfsClientID:public dfsClientConnect
		{
			UTF8String set_name;

			IDLockTeam *lock_list;

			dfsClientIDConnectPool *conn_pool;

		private:

			bool SendPrivateLogin(DataOutputStream *);												///<发送私有Login数据

		public:

			dfsClientID(const UTF8String &);
			~dfsClientID();

			bool Init(const IPAddress *,const UTF8String &,const int64 _node_id=-1);			  ///<初始化dfs id客户端连接

			int64 CreateID();																		///<单纯产生一个ID

			int64 Add(const dfsIDKey &);															///<增加一个key，并返回它对应的ID
			int64 Get(const dfsIDKey &);															///<取得一个key的ID
			bool Delete(const dfsIDKey &);															///<删除一个key

			bool Lock(const int64);																///<锁定一个ID
			bool Unlock(const int64);																///<解决一个ID
			bool IsLock(const int64,bool &);														///<获取这个ID是否加锁

			bool GetAndLock(const dfsIDKey &,int64 &);												///<取得一个key的ID并加锁这个ID
		};//class dfsClientID
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_CLIENT_ID_INCLUDE
