#include<hgl/network/SocketManageThread.h>
#include<hgl/network/Socket.h>

namespace hgl
{
	namespace network
	{
		SocketManageThread::SocketManageThread(SocketManageBase *smb)
		{
			sock_manage=smb;

			time_out=HGL_NETWORK_TIME_OUT;
		}

		SocketManageThread::~SocketManageThread()
		{
			SAFE_CLEAR(sock_manage);
		}

		void SocketManageThread::ProcSocketJoin()
		{
			Socket **sp=sock_join_set.GetData();
			int count=sock_join_set.GetCount();

			int total=0;

			for(int i=0;i<count;i++)
			{
				if(sock_set.Find(*sp)==-1)
				{
					if(sock_manage->Join((*sp)->ThisSocket))
					{
						sock_set.Add(*sp);
						++total;
					}
				}

				++sp;
			}
		}

		bool SocketManageThread::Join(Socket *s)
		{
			if(!s)RETURN_FALSE;

		}

		int SocketManageThread::Join(Socket **sl, int count)
		{
			if(!sl||count<=0)
				RETURN_FALSE;

			int total=0;

			for(int i=0;i<count;i++)
			{
				if(Join(*sl))
					++total;

				++sl;
			}

			return total;
		}

		bool SocketManageThread::Unjoin(Socket *s)
		{
			if(!s)RETURN_FALSE;
			if(s->ThisSocket==-1)RETURN_FALSE;

			sock_manage->Unjoin(s->ThisSocket);
			sock_set.Delete(s);
			return(true);
		}

		int SocketManageThread::GetCount() const
		{
			return sock_manage->GetCount();
		}

		void SocketManageThread::Clear()
		{
			sock_manage->Clear();
			sock_set.ClearData();
		}

		bool SocketManageThread::Execute()
		{
			int num=sock_manage->Update(event_list,error_list,time_out);

			if(num)
		}
	}//namespace network
}//namespace hgl
