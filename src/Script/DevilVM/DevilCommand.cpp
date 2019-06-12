#include<hgl.h>
#include"DevilCommand.h"
#include"DevilContext.h"
#include"DevilFunc.h"

namespace hgl
{
#if HGL_CPU == HGL_CPU_X86_32
    void *CallCDeclFunction(void *,const void *,int);                                           ///<呼叫C函数
    void *CallThiscallFunction(void *,const void *,const void *,int);                           ///<呼叫C++函数

    bool DevilFuncMap::Call(const DevilSystemFuncParam *call_param,const int param_size,void *return_result)
    {
        if(base)        //有this指针
            *(void **)return_result=CallThiscallFunction(func,base,call_param,param_size);
        else
            *(void **)return_result=CallCDeclFunction(func,call_param,param_size);

        return(true);
    }
#elif HGL_CPU == HGL_CPU_X86_64
    extern "C" void *CallX64(void *func,int argc,const void *argv, const void *argv_float);

    bool DevilFuncMap::Call(const DevilSystemFuncParam *call_param,const int param_size,void *return_result)
    {
        //X86-64位情况下，this放入参数的第一个，在解析代码时已经确定，所以无需再次处理
        *(void **)return_result=CallX64(func,param_size,call_param,call_param);

        return(true);
    }
#endif//
}//namespace hgl

//namespace hgl
//{
//  DevilSystemFuncCallDynamic::DevilSystemFuncCallDynamic(DevilFuncMap *dfm)
//  {
//      func=dfm;
//
//      param_size=dfm->param.Count*sizeof(uint);
//      param=new uint[dfm->param.Count];
//  }
//
//  DevilSystemFuncCallDynamic::~DevilSystemFuncCallDynamic()
//  {
//      delete[] param;
//  }
//
//  bool DevilSystemFuncCallDynamic::Run(DevilScriptContext *context)
//  {
//
//  }
//}//namespace hgl

namespace hgl
{
    DevilScriptFuncCall::DevilScriptFuncCall(DevilScriptModule *dm,DevilFunc *df)
    {
        module=dm;
        func=df;
    }

    bool DevilScriptFuncCall::Run(DevilScriptContext *context)
    {
        context->ScriptFuncCall(func);

        return(true);
    }
}//namespace hgl

namespace hgl
{
    DevilGoto::DevilGoto(DevilScriptModule *dm,DevilFunc *df,const UTF16String &flag)
    {
        module=dm;

        func=df;

        name=flag;

        index=-1;
    }

    void DevilGoto::UpdateGotoFlag()
    {
        func->goto_flag.Get(name,index);        // 由于跳转标识有可能在这个GOTO之后定义，所以必须等这个函数解晰完了，再调用SetLine

        if(index==-1)
            LOG_ERROR(u"在函数<"+func->func_name+u">没有找到跳转标识:"+name);
    }

    bool DevilGoto::Run(DevilScriptContext *context)
    {
        #ifdef _DEBUG
            LOG_INFO(u"在函数<"+func->func_name+u">中跳转:"+name);
        #endif//_DEBUG

        return context->Goto(func,index);
    }
}//namespace hgl

namespace hgl
{
    DevilCompGoto::DevilCompGoto(DevilScriptModule *dm,DevilCompInterface *dci,DevilFunc *f)
    {
        module=dm;
        comp=dci;
        func=f;

        index=-1;
    }

    DevilCompGoto::~DevilCompGoto()
    {
        delete comp;
    }

    void DevilCompGoto::UpdateGotoFlag()
    {
        func->goto_flag.Get(else_flag,index);

        if(index==-1)
            LOG_ERROR(u"在函数<"+func->func_name+u">没有找到跳转标识:"+else_flag);
    }

    bool DevilCompGoto::Run(DevilScriptContext *context)
    {
        if(comp->Comp())return(true);

        if(index==-1)           //不含else的if脚本，else_flag自动为end_flag
            return(false);

        return context->Goto(func,index);
    }
}//namespace hgl

namespace hgl
{
    DevilReturn::DevilReturn(DevilScriptModule *dm)
    {
        module=dm;
    }

    bool DevilReturn::Run(DevilScriptContext *context)
    {
        return context->Return();
    }
}//namespace hgl

