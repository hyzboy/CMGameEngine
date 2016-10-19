#ifndef HGL_PROTOCOL_BUFFER_SOCKET_THREAD_INCLUDE
#define HGL_PROTOCOL_BUFFER_SOCKET_THREAD_INCLUDE

#include<hgl/network/DirectSocketIOUserThread.h>
#include<hgl/pb/PBWorkThread.h>
namespace hgl
{
	namespace pb
	{
		template<typename ID,typename USERINFO,uint TEAM_NUMBER>
		using TCPSocketUserTeamManage=UserTeamManage<ID,USERINFO,MTUserDOSOutput,TEAM_NUMBER>;

		template<typename BC,typename ID,typename USERINFO,typename USER_MANAGE>
		class TCPPBSocketThread:public PBWorkThread<BC,ID,USERINFO,MTUserDOSOutput,USER_MANAGE>
		{
			static USER_MANAGE *user_manage;

		private:

			MemBlock<char> msg_data;

		public:

			TCPPBSocketThread(int s,const IPAddress *sa):PBWorkThread<BC,ID,USERINFO,MTUserDOSOutput,USER_MANAGE>(s,sa)
			{
			}

			static void InitUserManage(USER_MANAGE *um)
			{
				user_manage=um;
			}

			virtual bool ProcStartThread() override
			{
				this->SetUserManage(user_manage);
			}

			virtual ID CreateUserID()=0;

			MTUserDOSOutput *CreateUserOutput() override
			{
				return(new MTUserDOSOutput(this->dos));
			}

			virtual bool Update()
			{
				int32 msg_id;
				int32 msg_size;

				if(!this->dis->ReadInt32(msg_size))RETURN_FALSE;

				if(msg_size==0)return(true);				//heart

				if(msg_size<sizeof(int32))RETURN_FALSE;

				if(!this->dis->ReadInt32(msg_id))RETURN_FALSE;

				msg_size-=sizeof(int32);

				if(msg_size>0)
				{
					msg_data.SetLength(msg_size);
					if(this->dis->ReadFully(msg_data.data(),msg_size)!=msg_size)
						RETURN_FALSE;
				}

				LOG_INFO("---------------proc message id:"+UTF8String(msg_id)+"---------------");

				RETURN_BOOL(this->Proc(msg_id,msg_data.data(),msg_size));
			}
		};//class TPBCPSocketThread

		template<typename ID,typename USERINFO,uint TEAM_NUMBER>
		using LETCPPBSocketThread=TCPPBSocketThread<LETCPSocketIOUserThread,ID,USERINFO,TCPSocketUserTeamManage<ID,USERINFO,TEAM_NUMBER>>;

		template<typename ID,typename USERINFO,uint TEAM_NUMBER>
		using BETCPPBSocketThread=TCPPBSocketThread<BETCPSocketIOUserThread,ID,USERINFO,TCPSocketUserTeamManage<ID,USERINFO,TEAM_NUMBER>>;
	}//namespace pb
}//namespace hgl
#endif//HGL_PROTOCOL_BUFFER_SOCKET_THREAD_INCLUDE
