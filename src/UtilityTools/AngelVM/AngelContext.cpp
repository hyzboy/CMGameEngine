#include<hgl/script/AngelVM.h>
#include<angelscript.h>
#include<stdarg.h>
#include<hgl/platform/Platform.h>

namespace hgl
{
	AngelContext::~AngelContext()
	{
		ctx->Release();
	}

	/**
	 * 取得指定名称的函数
	 * @param func_name 函数名称
	 */
	asIScriptFunction *AngelContext::GetFunc(const char *func_name)
	{
		return module->GetFunctionByName(func_name);
	}

	/**
	 * 重新开始执行
	 */
	bool AngelContext::Start(asIScriptFunction *f)
	{
		if(!f)
			return(false);

		func=f;

		return(ctx->Prepare(func)>=asSUCCESS);
	}

	/**
	 * 执行
	 */
	bool AngelContext::Run()
	{
		if(!func)return(false);

		const int result=ctx->Execute();

		if(result>=asEXECUTION_FINISHED)
		{
			func=nullptr;
			return(true);
		}

		if(result==asEXECUTION_EXCEPTION)
		{
			UTF8String str="AngelScript Execution exception:";

			str+=ctx->GetExceptionString();
			str+="\nfunc:";
			str+=func->GetDeclaration();
			str+="\nmodule:";
			str+=func->GetModuleName();
			str+="\nsection:";
			str+=func->GetScriptSectionName();
			str+="\nline:";
			str+=UTF8String(ctx->GetExceptionLineNumber());

			LOG_ERROR(str);
		}

		return(false);
	}

	/**
	 * 暂停
	 */
	bool AngelContext::Pause()
	{
		if(!func)return(false);
		return(ctx->Suspend()>=asEXECUTION_FINISHED);
	}

	/**
	 * 停止
	 */
	bool AngelContext::Stop()
	{
		if(!func)return(false);

		func=nullptr;
		return(ctx->Abort()>=asEXECUTION_FINISHED);
	}

	bool AngelContext::_Run(va_list &va)
	{
		if(!func)return(false);

		const uint param_count=func->GetParamCount();
		asDWORD flag;
		int type;
		int result;

		for(uint i=0;i<param_count;i++)
		{
#if	ANGELSCRIPT_VERSION<22900
			type=func->GetParamTypeId(i,&flag);			//Deprecated since 2.29.0, 2014-04-06
#else
			const char *param_name;

			if(func->GetParam(i,&type,&flag,&param_name,nullptr)!=asSUCCESS)
			{
				LOG_INFO(UTF8String("无法自动识别脚本函数")+UTF8String(func->GetDeclaration())+UTF8String("的第")+UTF8String(i+1)+UTF8String("个参数,参数名称：")+UTF8String(param_name)+UTF8String("，请联系开发者已便取得支持."));
				va_end(va);
				return(false);
			}
#endif//

			if(flag==asTM_OUTREF
			 ||flag==asTM_INOUTREF)
			{
				result=ctx->SetArgAddress(i,va_arg(va,void *));
			}
			else
			if(type==asTYPEID_BOOL
			 ||type==asTYPEID_INT8 ||type==asTYPEID_UINT8 	)result=ctx->SetArgByte 	(i,va_arg(va,int	));else
			if(type==asTYPEID_INT16||type==asTYPEID_UINT16	)result=ctx->SetArgWord 	(i,va_arg(va,int	));else
			if(type==asTYPEID_INT32||type==asTYPEID_UINT32	)result=ctx->SetArgDWord	(i,va_arg(va,uint32	));else
			if(type==asTYPEID_INT64||type==asTYPEID_UINT64	)result=ctx->SetArgQWord	(i,va_arg(va,uint64	));else
#if HGL_32_BITS
			if(type==asTYPEID_FLOAT							)result=ctx->SetArgFloat	(i,va_arg(va,float	));else
			if(type==asTYPEID_DOUBLE						)result=ctx->SetArgDouble	(i,va_arg(va,double	));else
#else
			if(type==asTYPEID_FLOAT							)result=ctx->SetArgFloat	(i,va_arg(va,double	));else
			if(type==asTYPEID_DOUBLE						)result=ctx->SetArgDouble	(i,va_arg(va,double	));else
#endif//HGL_32_BITS
			if((type&0xFFFF0000)==asTYPEID_APPOBJECT		)result=ctx->SetArgObject	(i,va_arg(va,void *	));else
			{
				LOG_INFO(UTF8String("无法自动识别脚本函数")+UTF8String(func->GetDeclaration())+UTF8String("的第")+UTF8String(i+1)+UTF8String("个参数的类型")+UTF8String(type)+UTF8String("，请联系开发者已便取得支持."));
				va_end(va);
				return(false);
			}

			if(result!=asSUCCESS)
			{
				LOG_INFO(UTF8String("脚本函数")+UTF8String(func->GetDeclaration())+UTF8String("的第")+UTF8String(i+1)+UTF8String("个参数")+UTF8String(type)+UTF8String("设置失败，返回值：")+UTF8String(result)+UTF8String("，请联系开发者已便取得支持."));
				va_end(va);
				return(false);
			}
		}

		va_end(va);

		return Run();
	}

	bool AngelContext::Run(asIScriptFunction *f,...)
	{
		if(!f)return(false);

		if(!Start(f))
			return(false);

		va_list va;
		va_start(va,f);

		return _Run(va);
	}

	bool AngelContext::Run(const char *func_name,...)
	{
		if(!Start(func_name))
			return(false);

		va_list va;
		va_start(va,func_name);

		return _Run(va);
	}
}//namespace hgl
