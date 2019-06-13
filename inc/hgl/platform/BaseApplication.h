#ifndef HGL_BASE_APPLICATION_INCLUDE
#define HGL_BASE_APPLICATION_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/platform/SystemInfo.h>
namespace hgl
{
    class ConsoleSystemInitInfo;

    /**
    * 应用程序基类
    */
    class BaseApplication                                                                           ///应用程序基类
    {
    protected:

        ConsoleSystemInitInfo *sii;

        double cur_time;

        bool active;

    protected:

        ConsoleSystemInitInfo * GetSII      ()      {return sii;}

        double                  GetTime     ()      {return cur_time;}

        void                    UpdateTime  ();

        bool                    GetActive   ()      {return active;}
        void                    SetActive   (bool);

    public: //需要用户在启动游戏前设置的数据

        UTF8String                          ProjectName;                                            ///<游戏名称
        OSString                            ProjectCode;                                            ///<游戏代码

    public: //属性

        SystemInfo                          si;                                                     ///<系统信息

        Property<ConsoleSystemInitInfo *>   SII;                                                    ///<系统初始化信息
        Property<double>                    Time;                                                   ///<当前时间(秒)

        Property<bool>                      Active;                                                 ///<当前程序是否处于活动状态
        bool                                WaitActive;                                             ///<是否等待活动状态,默认为false

    public: //事件

        DefEvent(void,OnActive,(bool));                                                             ///<程序活动状态变更事件

    public:

        BaseApplication();
        virtual ~BaseApplication();

        virtual bool Init(ConsoleSystemInitInfo *);                                                 ///<初始化当前应用程序

        virtual int Run()=0;                                                                        ///<运行当前应用程序

        virtual void ExitApp()=0;                                                                   ///<退出游戏
    };//class BaseApplication
}//namespace hgl
#endif//HGL_BASE_APPLICATION_INCLUDE;
