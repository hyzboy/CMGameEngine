#include<hgl/network/IOSocket.h>
#include<hgl/network/SocketManage.h>
#include"SocketManageBase.h"

namespace hgl
{
	namespace network
	{
		SocketManage::SocketManage(int mc)
		{
			max_connect=mc;

			time_out=HGL_NETWORK_TIME_OUT;

			cur_count=0;

			recv_event_list.Malloc(max_connect);
			send_event_list.Malloc(max_connect);
			error_event_list.Malloc(max_connect);

			recv_list.Malloc(max_connect);
			send_list.Malloc(max_connect);

			error_set.Malloc(max_connect);

			manage=CreateSocketManageBase(mc);
		}

		SocketManage::~SocketManage()
		{
			delete manage;
		}

		/**
		 * 设置socket超时时间
		 * @param to 超时时间(单位：秒)
		 */
		void SocketManage::SetTimeOut(double to)
		{
			if(time_out==to)return;

			time_out=to;
		}

		bool SocketManage::_Join(IOSocket *sock)
		{
			if(sock_list.Find(sock->ThisSocket)!=-1)
				RETURN_FALSE;

			if(!manage->Join(sock))
				RETURN_FALSE;

			sock_list.Add(sock->ThisSocket,SocketItem(sock));

			sock->RestartLastRecvTime();
			cur_count++;
			return(true);
		}

		/**
		 * 加入一个socket
		 */
		bool SocketManage::Join(IOSocket *sock)
		{
			if(!sock)
			{
				LOG_ERROR(OS_TEXT("SocketManage::Join() Add NULL IOSocket to SocketManage"));
				RETURN_FALSE;
			}

			if(sock->ThisSocket==-1)
			{
				LOG_ERROR(OS_TEXT("SocketManage::Join() Add Error IOSocket to SocketManage"));
				RETURN_FALSE;
			}

			lock.Lock();

			const bool result=_Join(sock);

			lock.Unlock();

			return result;
		}

		bool SocketManage::_Unjoin(IOSocket *sock)
		{
		    if(!manage)
			{
				LOG_ERROR(OS_TEXT("SocketManage::_Unjoin() manage=nullptr,socket:")+OSString(sock->ThisSocket));
		        RETURN_FALSE;
			}

			if(!sock_list.DeleteByIndex(sock->ThisSocket))
			{
				LOG_ERROR(OS_TEXT("SocketManage::_Unjoin() error,socket:")+OSString(sock->ThisSocket));
				RETURN_FALSE;
			}

			manage->Unjoin(sock);
			cur_count--;
			return(true);
		}

		/**
		 * 分离一个socket
		 */
		bool SocketManage::Unjoin(IOSocket *sock)
		{
			if(!sock)
			{
				LOG_ERROR(OS_TEXT("SocketManage::Unjoin() Remove NULL IOSocket from SocketManage"));
				RETURN_FALSE;
			}

			if(sock->ThisSocket==-1)
			{
				LOG_ERROR(OS_TEXT("SocketManage::Unjoin() Remove Error IOSocket from SocketManage"));
				RETURN_FALSE;
			}

			lock.Lock();

			const bool result=_Unjoin(sock);

			if(result)
			{
				int left_bytes;

				sock->ProcSend(-1,left_bytes);

				if(left_bytes>0)
				{
					LOG_ERROR(OS_TEXT("SocketManage::Unjoin() IOSocket left bytes:")+OSString(left_bytes));
				}
			}

			lock.Unlock();

			return result;
		}

		int SocketManage::_Unjoin(IOSocket **splist,const int spcount)
		{
			if(spcount<=0)
				return(spcount);

			if(!splist)
				return(-1);

			int total=0;
			int left_bytes;

			for(int i=0;i<spcount;i++)
			{
				if(_Unjoin(*splist))
				{
					(*splist)->ProcSend(-1,left_bytes);
					++total;
				}

				++splist;
			}

			return(total);
		}

		/**
		 * 分离一批socket
		 * @param splist Socket指针
		 * @param spcount Socket数量
		 * @return 成功分离的Socket数量
		 */
		int SocketManage::Unjoin(IOSocket **splist,const int spcount)
		{
			if(spcount<=0)
				return(spcount);

			if(!splist)
				return(-1);

			int result;

			LOG_INFO(OS_TEXT("SocketManage::Unjoin Begin count=")+OSString(spcount));

			lock.Lock();
				result=_Unjoin(splist,spcount);
			lock.Unlock();

			LOG_INFO(OS_TEXT("SocketManage::Unjoin End count=")+OSString(spcount));
			return(result);
		}

		void SocketManage::_Clear()
		{
			manage->Clear();
			cur_count=0;
			sock_list.Clear();
		}

		void SocketManage::Clear()
		{
			lock.Lock();
			_Clear();
			lock.Unlock();
		}

		void SocketManage::ConvEventList(List<IOSocketClassEvent> &tar_list,const List<SocketEvent> &src_list)
		{
			tar_list.SetCount(src_list.GetCount());

			SocketEvent *src=src_list.GetData();
			IOSocketClassEvent *tar=tar_list.GetData();

			SocketItem si;

			for(int i=0;i<src_list.GetCount();i++)
			{
				if(sock_list.Get(src->sock,si))
				{
					tar->sock=si.sock;
					tar->size=src->size;
					++tar;
				}

				++src;
			}
		}

		bool SocketManage::Update(const double cur_time)
		{
			if(!manage)
				return(false);

			recv_list.ClearData();
			send_list.ClearData();
			error_set.ClearData();
			recv_event_list.ClearData();
			send_event_list.ClearData();
			error_event_list.ClearData();

			if(cur_count<=0)
				return(false);

			lock.Lock();
			{
				const int count=sock_list.GetCount();
				SocketList::IDItem **sil=sock_list.GetDataList();
				SocketItem *si;
				int left_bytes;

				for(int i=0;i<count;i++)
				{
					si=&((*sil)->second);
					++sil;

					if(si->sock->CheckRecvTimeOut(cur_time))//超时
					{
						LOG_ERROR(OS_TEXT("SocketManage::Update() IOSocket:")+OSString(si->sock->ThisSocket)+OS_TEXT(" TimeOut,auto disconnect."));
						error_set.Add(si->sock);
						continue;
					}

					left_bytes=si->sock->GetLeftSendBytes();

					if(left_bytes>0)			//有数据要发
					{
						if(!(si->w))
						{
							manage->Change(si->sock,true,true);		//改为有发送
							si->w=true;
						}
					}
					else
					{
						if(si->w)
						{
							manage->Change(si->sock,true,false);		//改为无发送
							si->w=false;
						}
					}
				}

				//断掉所有出错Socket
				if(error_set.GetCount()>0)
					_Unjoin(error_set.GetData(),error_set.GetCount());
			}
			lock.Unlock();

			if(manage->Update(recv_event_list,send_event_list,error_event_list,time_out)<=0)
				return(false);

			ConvEventList(recv_list,recv_event_list);		//转换接收消息Socket列表
			ConvEventList(send_list,send_event_list);		//转换发送消息Socket列表

			//转换出错Socket列表
			{
				const int count=error_event_list.GetCount();
				SocketEvent *se=error_event_list.GetData();
				SocketItem si;

				for(int i=0;i<count;i++)
				{
					if(sock_list.Get(se->sock,si))
						error_set.Add(si.sock);

					++se;
				}
			}

			//接收信息
			{
				int count=recv_list.GetCount();

				if(count>0)
				{
					IOSocketClassEvent *rce=recv_list.GetData()+count-1;

					while(count--)
					{
						if(error_set.Find(rce->sock)==-1)
						{
							if(rce->sock->ProcRecv(rce->size,cur_time)<=0)
							{
								LOG_ERROR(OS_TEXT("SocketManage::Update() Call IOSocket ProcRecv error,socket:")+OSString(rce->sock->ThisSocket));
								error_set.Add(rce->sock);
								recv_list.Delete(count);
							}
						}
						else
							recv_list.Delete(count);

						--rce;
					}
				}
			}

			//发送信息
			{
				int count=send_list.GetCount();

				if(count>0)
				{
					IOSocketClassEvent *sce=send_list.GetData()+count-1;

					while(count--)
					{
						int left_bytes;

						if(error_set.Find(sce->sock)==-1)
						{
							if(sce->sock->ProcSend(sce->size,left_bytes)<=0)
							{
								LOG_ERROR(OS_TEXT("SocketManage::Update() Call IOSocket ProcSend error,socket:")+OSString(sce->sock->ThisSocket));
								error_set.Add(sce->sock);
								send_list.Delete(count);
							}
							else
								send_list.Delete(count);
						}

						--sce;
					}
				}
			}

			//断掉所有出错Socket
			if(error_set.GetCount()>0)
				Unjoin(error_set.GetData(),error_set.GetCount());

			return(true);
		}
	}//namespace network
}//namespace hgl
