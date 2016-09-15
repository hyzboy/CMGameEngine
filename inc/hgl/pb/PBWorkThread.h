#ifndef HGL_PROTOCOL_BUFFER_WORK_THREAD_INCLUDE
#define HGL_PROTOCOL_BUFFER_WORK_THREAD_INCLUDE

#include<hgl/pb/PBUserManage.h>
#include<hgl/pb/mp.h>
namespace hgl
{
	namespace pb
	{
		template<typename BC,typename ID,typename USERINFO,typename USER_OUTPUT,typename USER_MANAGE>
		class PBWorkThread:public BC
		{
		protected:

			USER_MANAGE *user_manage=nullptr;

			ID user_id=-1;
			USERINFO *user_info=nullptr;

			PBMessageProc mp;

			SendControl<ID> *send_control;

		protected:

			virtual ID CreateUserID()=0;					///<请自行实现
			virtual USER_OUTPUT *CreateUserOutput()=0;		///<请自行实现

			/**
			 * 注册当前用户到管理器
			 */
			bool Registry()
			{
				user_id=CreateUserID();

				user_info=new USERINFO(user_id);

				UserInstance<USERINFO> *ui=new UserInstance<USERINFO>(user_info);
				USER_OUTPUT *uo=CreateUserOutput();

				if(user_manage->Registry(user_id,ui,uo))
					return(true);

				delete uo;
				delete ui;

				RETURN_FALSE;
			}

		public:

			using BC::BC;

			virtual ~PBWorkThread()
			{
				if(user_id>0)
					user_manage->Unregistry(user_id);

				SAFE_CLEAR(user_info);
			}

			void SetUserManage(USER_MANAGE *um)
			{
				user_manage=um;
				send_control=user_manage->CreateSendControl();
			}

			virtual bool Proc(int msg_id,void *msg_data,int msg_size)
			{
				user_info->Proc(msg_id);

				return mp.Proc(msg_id,msg_data,msg_size);
			}
		};//class PBWorkThread
	}//namespace pb
}//namespace hgl
#endif//HGL_PROTOCOL_BUFFER_WORK_THREAD_INCLUDE
