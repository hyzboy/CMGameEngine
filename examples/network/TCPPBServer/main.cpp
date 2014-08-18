#include<hgl/ConsolePBServer.h>
#include"msg.pb.h"

using USER_ID=uint32;
struct UserInfo:public PBUserInfo
{
public:

	UserInfo(const USER_ID id)
	{
	}
};//struct UserInfo

HGL_PB_WORK_THREAD_CLASS(USER_ID,UserInfo,256,UserWorkThread)
{
public:

	using UserWorkThreadBASE::UserWorkThreadBASE;

	USER_ID CreateUserID()
	{
		static atom<USER_ID> id_count=0;

		return(++id_count);
	}
};//class UserWorkThread

class UserThread:public UserWorkThread
{
public:

	UserThread(int sock,const sockaddr_in &sa):UserWorkThread(sock,sa)
	{
		mp.Init(0,100);

	#define SET_PB_EVENT(name)	{	\
									PBCmdFunc pcf;	\
									\
									SetEventCall(pcf,this,UserThread,on_##name);	\
									\
									mp.SetProc(c2s_##name::id,pcf,new c2s_##name);	\
								}

			SET_PB_EVENT(login);

	#undef SET_PB_EVENT
	}

	bool on_login(c2s_login *)
	{
		return Registry();
	}
};//class UserThread:public UserWorkThread

HGL_CONSOLE_PB_SERVER_APPLICATION(	"基于Google Protocol Buffer的TCP服务器范例",
									"TCPPBServer",
								   UserThread,
								   "0.0.0.0",8000)

