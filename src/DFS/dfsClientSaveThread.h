#ifndef HGL_DFS_CLIENT_SAVE_THREAD_INCLUDE
#define HGL_DFS_CLIENT_SAVE_THREAD_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/dfs/dfsClientFile.h>
#include<hgl/type/Queue.h>
namespace hgl
{
	namespace io
	{
		class MemoryOutputStream;
	}//namespace io

	using namespace io;

	namespace dfs
	{
		class dfsClientConnect;

		struct dfsClientSaveItem
		{
			FileBlock *block;
			MemoryOutputStream *mos;
			uint32 attrib;

		public:

			bool operator == (const struct dfsClientSaveItem &csi)const
			{
				return(block==csi.block);		//只比较block
			}
		};//struct dfsClientSaveItem

		class dfsClientSaveThread:public Thread
		{
			UTF8String group_name;

			IPAddress *addr;
			uint64 node_id;

			dfsClientConnect *con;

		private:

			Semaphore *fb_sem;
			ThreadMutexObject<Queue<dfsClientSaveItem> > fb_list;

			dfs::ErrorCode SaveTo(DataInputStream *dis,DataOutputStream *dos,FileBlock *fb,MemoryOutputStream *mos,uint32);

		public:

			dfsClientSaveThread(const UTF8String &g,const IPAddress *,const uint64);
			~dfsClientSaveThread();

			void Add(dfsClientSaveItem &);

			bool Execute();
		};//class dfsClientSaveThread
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_CLIENT_SAVE_THREAD_INCLUDE
