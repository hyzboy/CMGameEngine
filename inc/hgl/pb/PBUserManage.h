#ifndef HGL_PB_USER_MANAGE_INCLUDE
#define HGL_PB_USER_MANAGE_INCLUDE

#include<hgl/thread/ThreadMutex.h>
#include<hgl/thread/RWLock.h>
#include<hgl/type/Map.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/MemBlock.h>
#include<hgl/pb/PBSendControl.h>
namespace hgl
{
	namespace pb
	{
		/**
		 * 用户信息类<br>
		 * 做为相关模板USERINFO参数的范例
		 */
		class PBUserInfo
		{
		public:

			virtual bool Proc(int cmd){}							///<要准备处理消息了
		};//class PBUserInfo

		/**
		 * 用户信息实例
		 */
		template<typename USERINFO> struct UserInstance
		{
			USERINFO *user_info;

		public:

			UserInstance(USERINFO *ui)
			{
				user_info=ui;
			}

			virtual ~UserInstance()=default;
		};//struct UserInstance

		/**
		* 用户输出基类
		*/
		struct UserOutput
		{
		public:

			UserOutput(){}
			virtual ~UserOutput()=default;

			virtual bool Send(void *,int)=0;
		};//struct UserOutput

		/**
		 * 使用DataOutputStream的用户输出类
		 */
		struct UserDOSOutput:public UserOutput
		{
			DataOutputStream *dos;

		public:

			UserDOSOutput():UserOutput()
			{
				dos=nullptr;
			}

			UserDOSOutput(DataOutputStream *o)
			{
				dos=o;
			}

			virtual ~UserDOSOutput()=default;

			virtual bool Send(void *data,int size) override
			{
				RETURN_BOOL(dos->WriteFully(data,size)==size);
			}
		};//struct UserDOSOutput

		template<typename UO>
		struct MTUserOutput:public UO
		{
			ThreadMutex lock;

		public:

			using UO::UO;
			virtual ~MTUserOutput()=default;

			virtual bool Send(void *data,int size) override
			{
				ThreadMutexLock tml(&lock);

				return UO::Send(data,size);
			}
		};//struct MTUserOutput

		using MTUserDOSOutput=MTUserOutput<UserDOSOutput>;

		/**
		 * 用户组
		 */
		template<typename ID,typename USERINFO,typename USER_OUTPUT> struct UserTeam
		{
		public:

			using USER_INSTANCE=UserInstance<USERINFO>;

		protected:

			MapObject<ID,USER_INSTANCE> user_list;
			MapObject<ID,USER_OUTPUT> out_list;

		public:

			virtual ~UserTeam()=default;

			/**
			 * 注册一个用户
			 * @param id 用户ID
			 * @param ui 用户信息对象
			 * @param uo 用户输出对象
			 * @return 是否注册成功
			 */
			bool Registry(const ID id,USER_INSTANCE *ui,USER_OUTPUT *uo)
			{
				if(id<=0)RETURN_FALSE;
				if(!ui)RETURN_FALSE;
				if(!uo)RETURN_FALSE;

				if(!user_list.Add(id,ui))
					RETURN_FALSE;

				out_list.Add(id,uo);

				return(true);
			}

			/**
			 * 解除一个用户
			 * @param id 用户ID
			 */
			void Unregistry(const ID id)
			{
				user_list.DeleteByIndex(id);
				out_list.DeleteByIndex(id);
			}

			/**
			 * 获取一个用户数据
			 * @param id 用户ID
			 * @param user_info 用来保存用户信息的指针
			 * @return 是否成功
			 */
			bool Get(const ID id,USERINFO *user_info)
			{
				USER_INSTANCE *ui=user_list[id];

				if(!ui)RETURN_FALSE;

				user_info->Set(ui->user_info);

				return(true);
			}

			/**
			 * 写入一个用户数据
			 * @param id 用户ID
			 * @param user_info 用户信息数据指针
			 * @return 是否成功
			 */
			bool Set(const ID id,USERINFO *user_info)
			{
				USER_INSTANCE *ui=user_list[id];

				if(!ui)RETURN_FALSE;

				ui->user_info->Set(user_info);

				return(true);
			}

			/**
			 * 给指定用户发送数据
			 * @param id 用户ID
			 * @param data 数据
			 * @param size 数据长度
			 * @return 是否成功
			 */
			bool Send(const ID id,void *data,int size)
			{
				USER_OUTPUT *uo;

				uo=out_list[id];

				if(!uo)
					RETURN_FALSE;

				RETURN_BOOL(uo->Send(data,size));
			}
		};//class UserTeam

		/**
		 * 多线程用户组
		 */
		template<typename ID,typename USERINFO,typename USER_OUTPUT> struct MTUserTeam
		{
		public:

			using USER_INSTANCE=UserInstance<USERINFO>;

		protected:

			RWLock user_lock;

			MapObject<ID,USER_INSTANCE> user_list;

			RWLock out_lock;

			MapObject<ID,USER_OUTPUT> out_list;

		public:

			virtual ~MTUserTeam()=default;

			/**
			 * 注册一个用户
			 * @param id 用户ID
			 * @param ui 用户信息对象
			 * @param uo 用户输出对象
			 * @return 是否注册成功
			 */
			bool Registry(const ID id,USER_INSTANCE *ui,USER_OUTPUT *uo)
			{
				if(id<=0)RETURN_FALSE;
				if(!ui)RETURN_FALSE;
				if(!uo)return(nullptr);

				user_lock.WriteLock();
					if(!user_list.Add(id,ui))
					{
						user_lock.WriteUnlock();
						RETURN_FALSE;
					}
				user_lock.WriteUnlock();

				out_lock.WriteLock();
					out_list.Add(id,uo);
				out_lock.WriteUnlock();

				return(true);
			}

			/**
			 * 解除一个用户
			 * @param id 用户ID
			 */
			void Unregistry(const ID id)
			{
				user_lock.WriteLock();
					user_list.DeleteByIndex(id);
				user_lock.WriteUnlock();

				out_lock.WriteLock();
					out_list.DeleteByIndex(id);
				out_lock.WriteUnlock();
			}

			/**
			 * 获取一个用户数据
			 * @param id 用户ID
			 * @param user_info 用来保存用户信息的指针
			 * @return 是否成功
			 */
			bool Get(const ID id,USERINFO *user_info)
			{
				OnlyReadLock orl(user_lock);

				USER_INSTANCE *ui=user_list[id];

				if(!ui)RETURN_FALSE;

				user_info->Set(ui->user_info);

				return(true);
			}

			/**
			 * 写入一个用户数据
			 * @param id 用户ID
			 * @param user_info 用户信息数据指针
			 * @return 是否成功
			 */
			bool Set(const ID id,USERINFO *user_info)
			{
				OnlyReadLock orl(user_lock);

				USER_INSTANCE *ui=user_list[id];

				if(!ui)RETURN_FALSE;

				ui->user_info->Set(user_info);

				return(true);
			}

			/**
			 * 给指定用户发送数据
			 * @param id 用户ID
			 * @param data 数据
			 * @param size 数据长度
			 * @return 是否成功
			 */
			bool Send(const ID id,void *data,int size)
			{
				USER_OUTPUT *uo;

				OnlyReadLock orl(out_lock);//我用这个列表，但不修改，所以这里是ReadLock

				uo=out_list[id];

				if(!uo)
					RETURN_FALSE;

				RETURN_BOOL(uo->Send(data,size));
			}
		};//class MTUserTeam

		/**
		 * 用户组管理
		 */
		template<typename ID,typename USERINFO,typename USERTEAM,typename USER_OUTPUT,uint TEAM_NUMBER> class _UserTeamManage
		{
		public:

			using USER_INSTANCE=UserInstance<USERINFO>;

		protected:

			USERTEAM team[TEAM_NUMBER];

		public:

			/**
			 * 注册一个用户
			 * @param id 用户ID
			 * @param ui 用户信息对象
			 * @param uo 用户输出对象
			 * @return 是否注册成功
			 */
			bool Registry(const ID id,USER_INSTANCE *ui,USER_OUTPUT *uo)
			{
				return team[id%TEAM_NUMBER].Registry(id,ui,uo);
			}

			/**
			 * 解除一个用户
			 * @param id 用户ID
			 */
			void Unregistry(const ID id)
			{
				return team[id%TEAM_NUMBER].Unregistry(id);
			}

			/**
			 * 获取一个用户数据
			 * @param id 用户ID
			 * @param user_info 用来保存用户信息的指针
			 * @return 是否成功
			 */
			bool Get(const ID id,USERINFO *user_info)
			{
				return team[id%TEAM_NUMBER].Get(id,user_info);
			}

			/**
			 * 写入一个用户数据
			 * @param id 用户ID
			 * @param user_info 用户信息数据指针
			 * @return 是否成功
			 */
			bool Set(const ID id,USERINFO *user_info)
			{
				return team[id%TEAM_NUMBER].Set(id,user_info);
			}

			/**
			 * 给指定用户发送数据
			 * @param id 用户ID
			 * @param data 数据
			 * @param size 数据长度
			 * @return 是否成功
			 */
			bool Send(const ID id,void *data,int size)
			{
				return team[id%TEAM_NUMBER].Send(id,data,size);
			}

			SendControl<ID> *CreateSendControl()
			{
				DefEvent(bool,send_func,(const ID id,void *,int));

				SetEventCall(send_func,this,_UserTeamManage,Send);

				return(new SendControl<ID>(send_func));
			}
		};//class _UserTeamManage

		/**
		 * 用户组管理
		 */
		template<typename ID,typename USERINFO,typename USER_OUTPUT,uint TEAM_NUMBER>
		using UserTeamManage=_UserTeamManage<ID,USERINFO,UserTeam<ID,USERINFO,USER_OUTPUT>,USER_OUTPUT,TEAM_NUMBER>;

		/**
		 * 多线程用户组管理
		 */
		template<typename ID,typename USERINFO,typename USER_OUTPUT,uint TEAM_NUMBER>
		using MTUserTeamManage=_UserTeamManage<ID,USERINFO,MTUserTeam<ID,USERINFO,USER_OUTPUT>,USER_OUTPUT,TEAM_NUMBER>;
	}//namespace pb
}//namespace hgl
#endif//HGL_PB_USER_MANAGE_INCLUDE
