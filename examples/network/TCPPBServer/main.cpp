#include<hgl/ConsolePBServer.h>
#include"msg.pb.h"                                            //自动生成的头文件

using USER_ID=uint32;                                        //用户ID类型定义

/**
 * 用户信息结构定义
 */
struct UserInfo:public PBUserInfo
{
public:

    UserInfo(const USER_ID id)
    {
    }
};//struct UserInfo

//定义工作类
HGL_PB_WORK_THREAD_CLASS(USER_ID,                            // 用于分流的ID数据类型
                        UserInfo,                            // 用户信息类型
                        256,                                // 分组数量
                        UserThread)                            // 工作类名称
{
private:

    USER_ID CreateUserID()
    {
        static atom<USER_ID> id_count=0;

        return(++id_count);
    }

public:

    UserThread(int sock,const IPAddress *sa)
        :UserThreadBASE(sock,sa)                            //基类，用当前工作类名称+BASE做为基类名称
    {
        mp.Init(c2s_login::id,                //结起消息ID
                c2s_login::id);                //结束消息ID

    #define SET_PB_EVENT(name)    {    \
                                    PBCmdFunc pcf;    \
                                    \
                                    SetEventCall(pcf,this,UserThread,on_##name);    \
                                    \
                                    mp.SetProc(c2s_##name::id,pcf,new c2s_##name);    \
                                }

            SET_PB_EVENT(login);                    //设定login消息处理函数

    #undef SET_PB_EVENT
    }

    bool on_login(c2s_login *)
    {
        return Registry();
    }
};//class UserThread

//定义工作应用
HGL_CONSOLE_PB_SERVER_APPLICATION(    "基于Google Protocol Buffer的TCP服务器范例",
                                    "TCPPBServer",
                                   UserThread,
                                   CreateIPv4TCP(8000))

