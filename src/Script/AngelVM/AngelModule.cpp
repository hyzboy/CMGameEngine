#include<hgl/script/AngelVM.h>
#include<hgl/io/OutputStream.h>
#include<hgl/io/InputStream.h>
#include<angelscript.h>
#ifdef USE_ANGEL_SCRIPT_JIT
#include"jit/as_jit.h"
#endif//USE_ANGEL_SCRIPT_JIT
#include<iostream>

namespace hgl
{
	class AngelByteCodeStream:public asIBinaryStream
	{
		io::InputStream *in;
		io::OutputStream *out;

	public:

		AngelByteCodeStream(io::InputStream *is)
		{
			in=is;
		}

		AngelByteCodeStream(io::OutputStream *os)
		{
			out=os;
		}

		void Read(void *ptr,asUINT size)
		{
			in->Read(ptr,size);
		}

		void Write(const void *ptr,asUINT size)
		{
			out->Write(ptr,size);
		}
	};//class AngelByteCodeStream
}//namespace hgl

namespace hgl
{
	AngelModule::~AngelModule()
	{
		//engine->DiscardModule(module->GetName());
	}

	const char *AngelModule::GetName()
	{
		if(!module)return(nullptr);

		return module->GetName();
	}

	/**
	 * 增加脚本
	 * @param section_name 片断名称(用于查错和调试)
	 * @param script_section 脚本内容
	 * @param script_length 脚本长度(-1表示自行strlen取长度)
	 * @return 是否添加成功
	 */
	bool AngelModule::AddScript(const char *section_name,const char *script_section,int script_length)
	{
		if(!script_section||!(*script_section)||script_length==0)
			return(false);

		if(script_length==-1)
			script_length=strlen(script_section);

		const int result=module->AddScriptSection(section_name,script_section,script_length);

		if(result>=asSUCCESS)
			return(true);

		LOG_ERROR(UTF8String("add script section \"")+section_name+UTF8String("\" return ")+UTF8String(result));

		return(false);
	}

	bool AngelModule::Build()
	{
		if(!module)
			return(false);

		const int result=module->Build();

		if(result>=asSUCCESS)
		{
			#ifdef USE_ANGEL_SCRIPT_JIT
			jit->finalizePages();
			#endif//USE_ANGEL_SCRIPT_JIT
			return(true);
		}

		LOG_ERROR(UTF8String("build script module \"")+module->GetName()+UTF8String("\" return ")+UTF8String(result));

		return(false);
	}

	/**
	 * 创建一个运行控制上下文件
	 */
	AngelContext *AngelModule::CreateContext()
	{
		asIScriptContext *ctx=engine->CreateContext();

		if(!ctx)
			return(nullptr);

		return(new AngelContext(module,ctx));
	}

	bool AngelModule::RunScript(const char *code)
	{
		UTF8String func_code="void ExecuteString()\n{\n";

		func_code+=code;
		func_code+=";\n}";

		asIScriptFunction *func=nullptr;

		if(module->CompileFunction("ExecuteString",func_code.c_str(),func_code.Length(),0,&func)<0)
			return(false);

		asIScriptContext *exec_ctx=engine->CreateContext();

		if(!exec_ctx)
		{
			func->Release();
			return(false);
		}

		if(exec_ctx->Prepare(func)<0)
		{
			func->Release();
			exec_ctx->Release();
			return(false);
		}

		exec_ctx->Execute();

		func->Release();
		exec_ctx->Release();

		return(true);
	}

	int AngelModule::SaveByteCode(io::OutputStream *os)
	{
		if(!os)return(-1);

		AngelByteCodeStream aos(os);

		return module->SaveByteCode(&aos);
	}

	int AngelModule::LoadByteCode(io::InputStream *is)
	{
		if(!is)return(-1);

		AngelByteCodeStream ais(is);

		return module->LoadByteCode(&ais);
	}
}//namespace hgl
