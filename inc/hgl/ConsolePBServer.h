#ifndef HGL_CONSOLE_PB_SERVER_INCLUDE
#define HGL_CONSOLE_PB_SERVER_INCLUDE

#include<hgl/Console.h>
#include<hgl/network/SocketThreadServer.h>
#include<hgl/network/TCPServer.h>
#include<hgl/pb/PBTCPSocketThread.h>

using namespace hgl;
using namespace hgl::pb;

namespace hgl
{
    #define HGL_PB_WORK_THREAD_CLASS(USER_ID,USER_INFO,TEAM_NUMBER,WORK_THREAD_NAME)	\
    using UserManage=TCPSocketUserTeamManage<USER_ID,USER_INFO,TEAM_NUMBER>;	\
    using WORK_THREAD_NAME##BASE=LETCPPBSocketThread<USER_ID,USER_INFO,TEAM_NUMBER>;	\
    template<> UserManage *WORK_THREAD_NAME##BASE::user_manage=nullptr;	\
    class WORK_THREAD_NAME:public WORK_THREAD_NAME##BASE

    #define HGL_CONSOLE_PB_SERVER_APPLICATION(app_name,app_code,USER_THREAD_NAME,IP_ADDRESS)	using namespace hgl;    \
                                                                                                using namespace hgl::pb;    \
    \
    HGL_CONSOLE_MAIN_FUNC()	\
    {	\
	    sii.info.ProjectName=U8_TEXT(app_name);	\
	    sii.info.ProjectCode=OS_TEXT(app_code);	\
	    \
	    if(!app.Init(&sii))	\
		    return(-1);	\
	    \
	    using ConsoleStartServer=SocketThreadServer<ConsoleFlowObject,TCPServer,USER_THREAD_NAME>;	\
	    \
	    UserManage user_manage;	\
	    \
	    USER_THREAD_NAME::InitUserManage(&user_manage);	\
	    \
	    app.SetStart(new ConsoleStartServer(IP_ADDRESS));	\
	    \
	    return app.Run();	\
    }
}//namespace hgl
#endif//HGL_CONSOLE_PB_SERVER_INCLUDE
