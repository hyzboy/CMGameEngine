#ifndef HGL_DFS_CLIENT_RANK_INCLUDE
#define HGL_DFS_CLIENT_RANK_INCLUDE

#include<hgl/dfs/dfsClientConnect.h>
#include<hgl/type/List.h>
#include<hgl/type/Map.h>
#include<hgl/thread/RWLock.h>
namespace hgl
{
	class ThreadMutex;

	namespace dfs
	{
		#define RANK_ITEM_DATA_MAX_SIZE	(512-sizeof(int32)-(sizeof(int64)*2))

		/**
		 * 排行榜记录
		 */
		struct RankItem
		{
			int32	no;			//名次
			int64	id;
			int64	value;
			int32	size;

			char	data[RANK_ITEM_DATA_MAX_SIZE];
		};//struct RankItem

		/**
		 * 排行榜
		 */
		class RankList
		{
			List<RankItem> ri_list;

			RWLock lock;

		public:

			HGL_RWLOCK(lock);

			List<RankItem> &GetList(){return ri_list;}

			bool Read(DataInputStream *);
		};//struct RankList

		/**
		 * dfs排行榜服务客户端
		 */
		class dfsClientRank:public dfsClientConnect													///dfs排行榜服务客户端
		{
			UTF8String rank_name;

			RankList rank_list;

		private:

			bool SendPrivateLogin(DataOutputStream *);												///<发送私有Login数据

		public:

			dfsClientRank(const UTF8String &);
			~dfsClientRank();

			void		RankUpdateRecord	(int64,int64,char *,int32);								///<提交一条排行榜记录
			RankList *	RankAcquireList		(int32,int32);											///<获取排行榜
			void		RankAcquireRecord	(int64,RankItem &);										///<获取一个排行记录
		};//class dfsClientRank
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_CLIENT_RANK_INCLUDE
