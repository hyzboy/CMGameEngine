#ifndef HGL_DFS_TO_ID_SERVER_CMD_INCLUDE
#define HGL_DFS_TO_ID_SERVER_CMD_INCLUDE

namespace hgl
{
	namespace dfs
	{
		enum ToIDServerCmd
		{
			tisNone=0,

			tisLogin,										///<登陆
			//NODE_ID(int64) node_id;						///<自己的节点ID，如为-1,表示新用户
			//UTF8String node_name;							///<自己的节点名称
			//UTF8String set_name;							///<集合名称

			//新用户返回：
			//UTF8String Server名称
			//int64 node_id
			//旧用名无返回

			tisCreateID,									///<产生一个ID
			//返回 int64

			tisAdd,											///<增加一个变量到合集
			//UTF8String name;								///<变量名称
			//返回 int64

			tisGet,											///<从合集获取一个变量的ID
			//UTF8String name;								///<变量名称
			//返回 int64

			tisDelete,										///<从合集中删除一个变量
			//UTF8String name;								///<变量名称
			//返回 bool

			tisLock,										///<建立一个锁
			//int64 id;										///<锁ID
			//返回 bool

			tisUnlock,										///<解除一个锁
			//int64 id;										///<锁ID
			//返回 bool

			tisIsLock,										///<是否锁状态
			//int64 id;										///<锁ID
			//返回 bool

			tisGetAndLock,									///<获取一个变量的ID并加锁
			//UTF8String name;								///<变量名称
			//返回
			// bool result;									///<是否成功
			// int64 id;									///<这个变量的id
		};//enum ToIDServerCmd
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_TO_ID_SERVER_CMD_INCLUDE
