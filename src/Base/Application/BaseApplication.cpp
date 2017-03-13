#include<hgl/platform/BaseApplication.h>
#include<hgl/platform/ConsoleSystemInitInfo.h>
#include<hgl/Other.h>
#include<hgl/Info.h>

namespace hgl
{
    bool InitCore(SystemInfo &si,ConsoleSystemInitInfo *);
    void CloseCore();
}//namespace hgl

namespace hgl
{
    BaseApplication::BaseApplication()
    {
        hglSetProperty(        Active,  this,BaseApplication::GetActive,BaseApplication::SetActive);
        hglSetPropertyRead(    SII,     this,BaseApplication::GetSII    );
        hglSetPropertyRead(    Time,    this,BaseApplication::GetTime   );

        sii=nullptr;

        active=true;
        WaitActive=false;

        OnActive=nullptr;

        cur_time=0;
    }

    BaseApplication::~BaseApplication()
    {
        CloseCore();
    }

    void BaseApplication::UpdateTime()
    {
        cur_time    =GetDoubleTime();
    }

    bool BaseApplication::Init(ConsoleSystemInitInfo *_sii)
    {
        if(!_sii)
        {
            LOG_ERROR(OS_TEXT("sii为空!"));
            return(false);
        }

        sii=_sii;

        sii->ProcBlankValue();                //处理未填写的值

        return InitCore(si,sii);
    }

    void BaseApplication::SetActive(bool a)
    {
        active=a;

        SafeCallEvent(OnActive,(a));
    }
}//namespace hgl
