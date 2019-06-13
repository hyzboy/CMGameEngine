#ifndef HGL_DevilVM_INCLUDE
#define HGL_DevilVM_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/object/Object.h>
#include<hgl/script/VM.h>
namespace hgl
{
    namespace io
    {
        class DataInputStream;
        class DataOutputStream;
    }//namespace io

    /**
    * 虚拟机状态
    */
    enum DevilVMState
    {
        dvsRun,     //运行
        dvsPause,   //暂停
        dvsStop,    //停止
    };//enum DevilVMState

    class DevilFunc;        //脚本函数

    /**
     * 虚拟机处理模块
     */
    class DevilModule
    {
    public: //事件

        DefEvent(bool,OnTrueFuncCall,(const u16char *));                        ///<真实函数呼叫

    public:

        DevilModule(){OnTrueFuncCall=nullptr;}
        virtual ~DevilModule()=default;

        virtual bool        MapProperty     (const u16char *,void *)=0;         ///<映射属性(真实变量的映射，在整个模块中全局有效)
        virtual bool        MapFunc         (const u16char *,void *)=0;         ///<映射C函数
//      virtual bool        MapFunc         (void *,const u16char *,void *)=0;  ///<映射C函数,并传入一个对像
        virtual bool        MapFunc         (const u16char *,void *,void *)=0;  ///<映射C++成员函数

        virtual bool        AddScript       (const u16char *,int=-1)=0;         ///<添加脚本并编译

        virtual DevilFunc * GetScriptFunc   (const UTF16String &);              ///<取得脚本函数

        virtual void        Clear           ()=0;                               ///<清除所有模块和映射

    public: //调试用函数

        #ifdef _DEBUG

        virtual void LogPropertyList()=0;                                       ///<输出属性变量列表
        virtual void LogMapFuncList()=0;                                        ///<输出映射函数列表
        virtual void LogScriptFuncList()=0;                                     ///<输出脚本函数列表

        #endif//_DEBUG
    };//class DevilModule

    /**
     * 虚拟机执行控制上下文
     */
    class DevilContext
    {
    protected:

        DevilVMState State;                                                     ///<虚拟机状态

    public:

        DevilContext(){State=dvsStop;}
        virtual ~DevilContext()=default;

        virtual bool Start(const u16char *)=0;                                  ///<开始运行虚拟机
        virtual bool Start(const u16char *,const u16char *)=0;                  ///<开始运行虚拟机
        virtual bool Start(DevilFunc *,...)=0;                                  ///<从指令函数开始运行虚拟机
        virtual bool Run(const u16char *func_name=0)=0;                         ///<运行虚拟机，如Start或End状态则从开始运行，Pause状态会继续运行
        virtual void Pause()=0;                                                 ///<暂停虚拟机，仅能从Run状态变为Pause，其它情况会失败
        virtual void Stop()=0;                                                  ///<终止虚拟机，从任何状况变为Start状态

        virtual bool Goto(const u16char *)=0;                                   ///<跳转到指定位置
        virtual bool Goto(const u16char *,const u16char *)=0;                   ///<跳转到指定位置

        virtual bool GetCurrentState(UTF16String &,int &)=0;                        ///<取得当前状态

        virtual bool SaveState(io::DataOutputStream *)=0;                       ///<保存状态
        virtual bool LoadState(io::DataInputStream *)=0;                        ///<加载状态
    };//class DevilContext

    const int DevilScriptMinLength=sizeof(u"func main(){}");                    ///<《魔鬼》最小脚本长度
}//namespace hgl
#endif//HGL_DevilVM_INCLUDE
