#ifndef HGL_DFS_TO_DATA_SERVER_COMMAND_INCLUDE
#define HGL_DFS_TO_DATA_SERVER_COMMAND_INCLUDE

namespace hgl
{
	namespace dfs
	{
		enum OtherServerToDataServerCommand
		{
			tdsNone=0,

			tdsLogin,										///<登陆
			//NODE_ID(int64) node_id;						///<自己的节点ID，如为-1,表示新用户
			//UTF8String node_name;							///<自己的节点名称

			tdsLoadFile=0x200,								///<读取文件
			//UTF8String filename;							///<文件名
			//int64 version;								///<客户端中已存在版本的版本号

			tdsSaveFile,									///<保存文件
			//UTF8String filename;							///<文件名
			//int64 version;								///<版本号
			//int32 file_length;							///<文件长度

			tdsDeleteFile,									///<删除文件
			//UTF8String filename;							///<文件名

			tdsMailSend=0x500,								///<发送一封邮件
			//int64 mail_index								///<邮箱名称
			//int32 index;									///<邮件索引
			//int32 size;									///<数据长度
			//char data[size];								///<数据

			tdsMailGetCount,								///<请求邮件数量
			//int64 mail_index								///<邮箱名称

			tdsMailAcquireScope,							///<请求邮件
			//int64 mail_index								///<邮箱名称
			//int32 start;									///<邮件起始编号
			//int32 number;									///<邮件数量

			tdsMailAcquire,									///<请求邮件
			//int64 mail_index								///<邮箱名称
			//int32 number;									///<邮件数量
			//int32 index[number];							///<邮件索引

			tdsMailDelete,									///<删除邮件
			//int64 mail_index								///<邮箱名称
			//int32 number;									///<邮件数量
			//int32 index[number];							///<邮件索引

			tdsEnd
		};//OtherServerToDataServerCommand
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_TO_DATA_SERVER_COMMAND_INCLUDE
