#ifndef HGL_DFS_CLIENT_MAIL_INCLUDE
#define HGL_DFS_CLIENT_MAIL_INCLUDE

#include<hgl/dfs/dfsClientConnect.h>
#include<hgl/type/BaseString.h>
#include<hgl/MemBlock.h>
#include<hgl/type/List.h>
#include<hgl/type/Map.h>
namespace hgl
{
	namespace io
	{
		class OutputStream;
	}//namespace io

	namespace dfs
	{
		typedef MapObject<int32,MemBlock<char> > dfsMailList;					///<邮件列表

		/**
		 * dfs邮件服务客户端
		 */
		class dfsClientMail:public dfsClientConnect													///dfs邮件服务客户端
		{
		public:

			OutputStream *	MailSend	(const int64,int32=-1);									///<发送一封邮件
			bool			MailGetCount(const int64,List<int32> &);								///<获取邮件数量以及邮件编号
			bool			MailGet		(const int64,int32,int32,dfsMailList &);					///<获取邮件
			bool			MailGet		(const int64,int32,int32 *,dfsMailList &);				///<获取邮件
			bool			MailDelete	(const int64,int32,int32 *);								///<删除邮件
		};//class dfsClientMail
	}//namespace dfs
}//namespace hgl
#endif//HGL_DFS_CLIENT_MAIL_INCLUDE
