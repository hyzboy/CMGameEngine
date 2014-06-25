#ifndef HGL_DFS_DATA_SERVER_TO_NODE_COMMAND_INCLUDE
#define HGL_DFS_DATA_SERVER_TO_NODE_COMMAND_INCLUDE

namespace hgl
{
	namespace io
	{
		enum DataServerToCommand
		{
			dstNone=0,

			dstLogin,						///<登陆成功
			// int64 node_id;				///<你的节点ID
			// int32 version;				///<版本号
			// UTF8String node_name;		///<服务器节点名称

			dstLoadFile=0x200,				///<加载文件结果
			// int64 version;				///<服务器端的版本号
			// int32 file_length;			///<文件长度,如为-1表示错误
			// char data[file_length];

			dstSaveFile,					///<可以保存一个文件
			// int64 version;				///<服务器版本

			dstDeleteFile,					///<删除文件结果
			// bool result;					///<是否成功

			dstRankList=0x400,				///<排行榜列表
			// int32 count;					///<结果数量
			// int32 start_no;				///<起始排名
			// struct
			// {
			//	int64 key;					///<记录者
			//	int64 value;				///<分值
			//	int32 size;					///<私有数据长度
			//	char data[size];			///<私有数据长度
			// }[count];

			dstRankRecord,					///<排行榜记录
			//	int32 no;					///<名次
			//	int64 value;				///<分值
			//	int32 size;					///<私有数据长度
			//	char data[size];			///<私有数据长度

//			dstMailCount=0x500,				///<请求邮件数量结果
			//int32 count;					///<邮件数量
			//int32 index[count];			///<邮件索引

//			dstMailRecv,					///<邮件接收
			// int32 count;					///<邮件数量
			// struct
			// {
			//	int32 index;				///<邮件索引
			//	int32 size;					///<邮件长度
			//	char data[size];			///<邮件数据
			// }[count];

			dstEnd
		};//enum DataServerToCommand
	}//namespace io
}//namespace hgl
#endif//HGL_DFS_DATA_SERVER_TO_NODE_COMMAND_INCLUDE
