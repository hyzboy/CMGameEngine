#include<hgl/platform/ConsoleApplication.h>
#include<hgl/object/ConsoleFlowControl.h>
#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/Info.h>

namespace hgl
{
    /**
    * 控制台应用程序基类构造函数
    * @param fc 自定义流程控制器
    * @param log 是否创建日志
    */
    ConsoleApplication::ConsoleApplication(ConsoleFlowControl *cus_control)
    {
        if(cus_control)
            flow=cus_control;
        else
            flow=new ConsoleFlowControl();
    }

    ConsoleApplication::~ConsoleApplication()
    {
        SAFE_CLEAR(flow);
    }

    bool InitOSupport(ConsoleSystemInitInfo *);

    bool ConsoleApplication::Init(ConsoleSystemInitInfo *_sii)
    {
        sii=_sii;

        if(sii)
        {
            if(!BaseApplication::Init(sii))
                return(false);

            if(!InitOSupport(sii))
                return(false);
        }

        if(flow)
            return(true);
        else
            return(false);
    }
}//namespace hgl
