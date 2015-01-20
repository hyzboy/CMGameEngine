#ifndef HGL_DFS_TO_RANK_SERVER_CMD_INCLUDE
#define HGL_DFS_TO_RANK_SERVER_CMD_INCLUDE

namespace hgl
{
	namespace dfs
	{
		enum ToRankServerCmd
		{
			trsNone=0,

			trsLogin,										///<登陆
			//NODE_ID(int64) node_id;						///<自己的节点ID，如为-1,表示新用户
			//UTF8String node_name;							///<自己的节点名称
			//UTF8String rank_name;							///<排行榜名称

			//新用户返回：
			//UTF8String Server名称
			//int64 node_id
			//旧用名无返回

			trsUpdateRecord,								///<更新排行榜记录
			//int64 key;									///<记录者
			//int64 value;									///<分值
			//int32 size;									///<私有数据长度
			//char data[size];								///<私有数据

			trsAcquireList,									///<请求排行榜列表
			//int32 start;									///<起始排名
			//int32 count;									///<获取数量

			trsAcquireRecord,								///<请求排行榜单条记录
			//int64 key;									///<记录者
		};//enum ToRankServerCmd
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_TO_RANK_SERVER_CMD_INCLUDE
