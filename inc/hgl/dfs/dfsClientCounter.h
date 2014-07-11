#ifndef HGL_DFS_CLIENT_COUNTER_INCLUDE
#define HGL_DFS_CLIENT_COUNTER_INCLUDE

#include<hgl/dfs/dfsClientConnect.h>
#include<hgl/type/Map.h>
namespace hgl
{
	class ThreadMutex;

	namespace dfs
	{
		/**
		 * 计数器
		 */
		struct Counter
		{
			int64 value;		//值
			double time;		//最后一次的查询时间
		};//struct Counter

		/**
		 * dfs计数器服务客户端
		 */
		class dfsClientCounter:public dfsClientConnect												///dfs计数器服务客户端
		{
			MapObject<UTF8String,Counter> counter_list;

		public:

			dfsClientCounter();
			~dfsClientCounter();

			int64 AcquireCounter(const UTF8String &,const uint32 count=1,const double t=-1);		///<请求一批计数
			int64 InquiryCounter(const UTF8String &,const double ,const double gt=60);			///<查询一个计数
		};//class dfsClientCounter
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_CLIENT_COUNTER_INCLUDE
