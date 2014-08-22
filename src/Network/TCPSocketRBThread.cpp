#include<hgl/network/TCPSocketRBThread.h>
#include<hgl/network/SocketManage.h>
#include<hgl/network/TCPSocket.h>
#include<hgl/thread/Semaphore.h>
#include<hgl/Other.h>

namespace hgl
{
	namespace network
	{
		TCPSocketRBThread::TCPSocketRBThread(int max_user,const double time_out)
		{
			sock_manage=new SocketManageCB(max_user);
			sock_manage->SetTimeOut(time_out);

			join_sem=new Semaphore(max_user);
		}

		TCPSocketRBThread::~TCPSocketRBThread()
		{
			delete join_sem;
			delete sock_manage;
		}

		bool TCPSocketRBThread::Join(TCPSocketRB **tcp,int count)
		{
			if(!tcp||count<=0)return(false);

			join_list.Lock();
				join_list->Add(tcp,count);
			join_list.Unlock();

			join_sem->Release();
			return(true);
		}

		void TCPSocketRBThread::ProcJoin()
		{
			join_list.Lock();
			sock_lock.WriteLock();

			const int count=join_list->GetCount();
			TCPSocketRB **p=join_list->GetData();

			for(int i=0;i<count;i++)
			{
				if(sock_set.Add(*p)!=-1)
					sock_manage->Join(*p);

				p++;
			}

			join_list->ClearData();

			sock_lock.WriteUnlock();
			join_list.Unlock();
		}

		bool TCPSocketRBThread::Unjoin(TCPSocketRB **tcp,int count)
		{
			if(!tcp||count<=0)return(false);

			unjoin_set.Lock();
				unjoin_set->Add(tcp,count);
			unjoin_set.Unlock();

			return(true);
		}

		void TCPSocketRBThread::ProcUnjoin()
		{
			unjoin_set.Lock();
			sock_lock.WriteLock();
				if(unjoin_set->GetCount()>0)
				{
					sock_manage->Unjoin((IOSocket **)(unjoin_set->GetData()),unjoin_set->GetCount());
					sock_set.Delete(unjoin_set->GetData(),unjoin_set->GetCount());

					unjoin_set->DeleteClear();
				}
			sock_lock.WriteUnlock();
			unjoin_set.Unlock();
		}

		bool TCPSocketRBThread::Execute()
		{
			if(!sock_manage)
				return(false);

			if(sock_manage->GetCount()<=0)
			{
				join_sem->Acquire();
				ProcJoin();
			}
			else
			{
				if(join_sem->TryAcquire())
					ProcJoin();
			}

			ProcUnjoin();			//一次全部unjoin,减少SocketManage的Lock/Unlock

			sock_manage->Update(GetDoubleTime());
			return(true);
		}
	}//namespace network
}//namespace hgl
