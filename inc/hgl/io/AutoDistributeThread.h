#ifndef HGL_AUTO_DISTRIBUTE_THREAD_INCLUDE
#define HGL_AUTO_DISTRIBUTE_THREAD_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/io/DataDistribute.h>
namespace hgl
{
	namespace io
	{
		class DataInputStream;

		/**
		 * 数据自动分发处理线程类
		 */
		class AutoDistributeThread:public Thread													///数据自动分发处理线程模板类
		{
		private:

			bool IsExitDelete()const{return true;}													///<返回在退出线程时，是否删除本对象

		protected:

			DataDistribute *auto_data_dist;															///<数据包分发管理

		public:

			AutoDistributeThread()
			{
				auto_data_dist=nullptr;
			}

			/**
			 * 初始化自动分发
			 * @param _dis 数据输入流
			 * @param start 数据类型起始ID
			 * @param end 数据类型结束ID
			 * @param block 是否阻塞输入
			 * @return 是否初始化成功
			 */
			virtual bool Init(DataInputStream *_dis,int start,int end,bool block)
			{
				if(!_dis)
					return(false);

				if(start<end)
					return(false);

				auto_data_dist=CreateDataDistribute(_dis,start,end,block);

				return(true);
			}

			virtual ~AutoDistributeThread()
			{
				SAFE_CLEAR(auto_data_dist);
			}

			virtual bool Start()																	///<开始运行当前线程
			{
				if(!auto_data_dist)
					return(false);

				return Thread::Start();
			}

			virtual bool Execute()																	///<线程执行函数
			{
				return(auto_data_dist->Update(-1)==DataDistribute::ddecOK);
			}
		};//class AutoDistributeThread:public Thread
	}//namespace io
}//namespace hgl

// 自动分发范例
/*
	首先在消息定义处复制下列宏,DST/dst为自定义前缀

	#define DST_PACKET_DEFINE(code,data_struct)			HGL_DATA_PACKET_TYPEDEF(data_struct,dst##code)					//用于数据包的定义

	#define DST_RECV_FUNC_DEF(name)						HGL_DATA_PACKET_RECV_FUNC_DEF(dst,name)							//用于自动分发类.H文件中对事件函数的声明
	#define DST_RECV_FUNC_REALIZE(obj_name,name)		HGL_DATA_PACKET_RECV_FUNC_REALIZE(dst,obj_name,name)			//用于自动分发类.cpp文件中对事件函数的实现
	#define DST_RECV_REGISTRY(obj,obj_name,name)		HGL_DATA_PACKET_RECV_REGISTRY(obj,obj_name,dst,name)			//用于自动分发类.cpp文件中对事件的注册

	enum DataServerToOtherServerCommand
	{
		dstNone=0,

		dstCreateAccount,
		dstLogin,

		dstEnd
	};//enum DataServerToOtherServerCommand


	//封包定义
	struct UsernamePassword
	{
	public:

		bool Read(DataInputStream *);	///封包解晰函数
	};//struct UsernamePassword

	DST_PACKET_DEFINE(CreateAccount,UsernamePassword);
	DST_PACKET_DEFINE(Login,		UsernamePassword);

	上面两行展开为：

	typedef UsernamePassword dstCreateAccountPacket;
	typedef UsernamePassword dstLoginPacket;

	即重定义UsernamePassword结构为这两个消息的对应封包结构

	执行类
	UserThread.h

	class UserThread:public TCPLEAutoDistributeThread
	{
		DST_RECV_FUNC_DEF(CreateAccount);
		DST_RECV_FUNC_DEF(Login);

		以上两行展开为

		bool OnRecvCreateAccount(dstCreateAccountPacket *);
		bool OnRecvLogin(dstLoginPacket *);
	};

	UserThread.cpp

	UserThread::UserThread()
	{
		Init(dis,dstNone+1,dstEnd-1);		//初始化自动分发类

		DST_RECV_REGISTRY(this,UserThread,CreateAccount);		//设置dstCreateAccount处理函数为UserThread::OnRecvCreateAccount
		DST_RECV_REGISTRY(this,Userthread,Login);				//设置dstLogin处理函数为UserThread::OnRecvLogin

		以上两行宏展开较为复杂，便不在这里说明
	}

	DST_RECV_FUNC_REALIZE(UserThread::CreateAccount)			//展开为bool UserThread::OnRecvCreateAccount(dstCreateAccountPacket *pack)
	{
		if(pack->....)		//
	}
*/
#endif//HGL_NETWORK_AUTO_DISTRIBUTE_THREAD_INCLUDE
