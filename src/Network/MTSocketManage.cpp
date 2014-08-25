#include<hgl/network/MTSocketManage.h>

namespace hgl
{
	namespace network
	{
		MTSocketManage::MTSocketManage()
		{
			max_socket_count=0;
			thread_count=0;

			recv_manage=nullptr;
			send_manage=nullptr;
		}

		MTSocketManage::~MTSocketManage()
		{
			SAFE_CLEAR_OBJECT_ARRAY(recv_manage,thread_count);
			SAFE_CLEAR_OBJECT_ARRAY(send_manage,thread_count);
		}

		/**
		* 初始化多线程Socket I/O管理器
		* @param msc 最大socket数量
		* @param tc 线程数
		* @return 是否初始化成功
		*/
		bool MTSocketManage::Init(int msc,int tc)																///<初始化
		{
			if(msc<=0)return(false);
			if(tc<=0)return(false);

			max_socket_count=msc;
			thread_count=tc;

			recv_manage=new SocketManageThread *[thread_count];
			send_manage=new SocketManageThread *[thread_count];

			for(int i=0;i<thread_count;i++)
			{
				recv_manage[i]=CreateRecvSocketManageThread();
				send_manage[i]=CreateSendSocketManageThread();
			}

			return(true);
		}

		bool MTSocketManage::Start()
		{
			if(thread_count<=0)return(false);

			for(int i=0;i<thread_count;i++)
			{
				recv_manage[i]->Start();
				send_manage[i]->Start();
			}

			return Thread::Start();
		}

		bool MTSocketManage::Join(IOSocket *s)																	///<增加一个Socket
		{
			if(!s)return(false);

			int result;

			sock_set.Lock();
				result=sock_set->Add(s);
			sock_set.Unlock();

			if(result==-1)
			{
				LOG_ERROR(OS_TEXT("MTSocketManage::Join() Repeat join socket:")+OSString(s->ThisSocket));
				return(false);
			}

			const int thread_index=s->ThisSocket%thread_count;

			if(!recv_manage[thread_index]->Join(s))
				return(false);

			if(!send_manage[thread_index]->Join(s))
			{
				recv_manage[thread_index]->Unjoin(s);
				return(false);
			}

			return(true);
		}

		bool MTSocketManage::Unjoin(IOSocket *s)																///<移除一个Socket
		{
			if(!s)return(false);

			bool result;

			sock_set.Lock();
				result=sock_set->Delete(s);
			sock_set.Unlock();

			if(!result)
			{
				LOG_ERROR(OS_TEXT("MTSocketManage::Unjoin() don't find this socket:")+OSString(s->ThisSocket));
				return(false);
			}

			const int thread_index=s->ThisSocket%thread_count;

			recv_manage[thread_index]->Unjoin(s);
			send_manage[thread_index]->Unjoin(s);

			return(true);
		}

		void MTSocketManage::ProcError(SocketManageThread *smt,IOSocket **sock_list,const int count)
		{
			if(!sock_list||count<=0)return;

			error_sock_set.Add(sock_list,count);
		}

		bool MTSocketManage::Execute()
		{
			if(!error_sock_set.WaitProc())
				return(true);

			const int count=error_sock_set->GetCount();
			IOSocket **sp=error_sock_set->GetData();

			sock_set.Lock();
				for(int i=0;i<count;i++)
				{
					sock_set->Delete(*sp);
					ProcError(*sp);

					++sp;
				}
			sock_set.Unlock();

			error_sock_set->ClearData();

			return(true);
		}
	}//namespace network
}//namespace hgl
