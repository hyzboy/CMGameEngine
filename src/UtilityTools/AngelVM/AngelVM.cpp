#include<hgl/script/AngelVM.h>
#include<angelscript.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/LogInfo.h>
#include<hgl/Str.h>
#include<stdlib.h>
#include<iostream>

#ifdef USE_ANGEL_SCRIPT_JIT
#include"jit/as_jit.h"
#endif//USE_ANGEL_SCRIPT_JIT

BEGIN_AS_NAMESPACE
void RegisterScriptArray(asIScriptEngine *engine, bool defaultArray);
END_AS_NAMESPACE

namespace hgl
{
	using namespace AngelScript;

	void RegistryMath(asIScriptEngine *);
	void RegistryString(asIScriptEngine *);

	namespace
	{
		static int angel_count=0;
		static ThreadMutex angel_lock;

		void AngelScriptLog(const UTF8String &str)
		{
			LOG_INFO(UTF8String("AngelScript Log: ")+str);
		}
	}

	bool AngelVM::IsSupportMultiThread()
	{
		const char *str=asGetLibraryOptions();

		return(!hgl::strstr(str,"AS_NO_THREADS"));
	}

	AngelVM::AngelVM()
	{
		angel_lock.Lock();
		if(angel_count++==0)
			asPrepareMultithread();

		engine=asCreateScriptEngine(ANGELSCRIPT_VERSION);
		angel_lock.Unlock();

		if(!engine)
		{
			LOG_ERROR(OS_TEXT("Create AngelScriptEngine")+OSString(ANGELSCRIPT_VERSION)+OS_TEXT(" Failed."));
			return;
		}

		engine->SetMessageCallback(asMETHOD(AngelVM, MessageCallback), this, asCALL_THISCALL);

#ifdef USE_ANGEL_SCRIPT_JIT
		jit=new asCJITCompiler();

		engine->SetEngineProperty(asEP_INCLUDE_JIT_INSTRUCTIONS, 1);
		engine->SetJITCompiler(jit);
#endif//USE_ANGEL_SCRIPT_JIT

		RegistryMath(engine);
		RegistryString(engine);
		RegisterScriptArray(engine,true);

		engine->RegisterGlobalFunction("void Log(const string &in)", asFUNCTION(AngelScriptLog), asCALL_CDECL);

		global_module=new AngelModule(engine,engine->GetModule(nullptr,asGM_ALWAYS_CREATE)
#ifdef USE_ANGEL_SCRIPT_JIT
		,jit
#endif//USE_ANGEL_SCRIPT_JIT
		);
	}

	AngelVM::~AngelVM()
	{
		const int mcount=MList.GetCount();

		for(int i=0;i<mcount;i++)
		{
			UTF8String name;

			MList.GetIndex(i,name);

			engine->DiscardModule(name);
		}

		delete global_module;

		angel_lock.Lock();

		if(engine)
			engine->Release();

#ifdef USE_ANGEL_SCRIPT_JIT
		if(jit)
			delete jit;
#endif//USE_ANGEL_SCRIPT_JIT

		if(--angel_count==0)
		{
			asUnprepareMultithread();
			asThreadCleanup();
		}
		angel_lock.Unlock();
	}

	void AngelVM::MessageCallback(const asSMessageInfo *msg)
	{
		const char *type = "ERR ";

		if( msg->type == asMSGTYPE_WARNING )
			type = "WARN";
		else if( msg->type == asMSGTYPE_INFORMATION )
			type = "INFO";

		LOG_INFO(UTF8String("AngelScript Message: ")+UTF8String(msg->section)+UTF8String(" (")+UTF8String(msg->row)+UTF8String(",")+UTF8String(msg->col)+UTF8String(") : ")+UTF8String(type)+UTF8String(" : ")+UTF8String(msg->message));
	}

	/**
	 * 映射一个属性(全局)
	 * @param value_name 属性描述(如：“int test”)
	 * @param value_address 属性地址
	 * @return 是否映射成功
	 */
	bool AngelVM::BindProperty(const char *value_name,void *value_address)
	{
		if(!engine||!value_name||!value_address)
			return(false);

		const int result=engine->RegisterGlobalProperty(value_name,value_address);

		if(result>=asSUCCESS)
			return(true);

		LOG_ERROR(OS_TEXT("Register Global Property return ")+OSString(result));
		return(false);
	}

	/**
	 * 映射一个函数(全局)
	 * @param func_name 函数描述(如：“float sin(float)”)
	 * @param func_address 函数地址
	 * @return 是否映射成功
	 */
	bool AngelVM::BindFunc(const char *func_name,void *func_address)
	{
		if(!engine||!func_name||!(*func_name)||!func_address)
			return(false);

		const int result=engine->RegisterGlobalFunction(func_name,asFUNCTION(func_address),asCALL_CDECL);

		if(result>=asSUCCESS)
			return(true);

		LOG_ERROR(UTF8String("Register Global Function \"")+func_name+UTF8String("\" return ")+UTF8String(result));
		return(false);
	}

	/**
	 * 创建一个对象映射
	 * @param name 对象名称
	 * @param size 对象所占内存字节数
	 * @param construct 构造函数指针
	 * @param destruct 析构函数指针
	 */
	AngelObject *AngelVM::CreateObject(const char *name,int size,void *construct,void *destruct)
	{
		if(!engine||!name||!(*name))return(nullptr);

		uint attrib=asOBJ_VALUE|asOBJ_APP_CLASS;

		if(construct)attrib|=asOBJ_APP_CLASS_CONSTRUCTOR;
		if(destruct	)attrib|=asOBJ_APP_CLASS_DESTRUCTOR;

		if(engine->RegisterObjectType(name,size,attrib)<0)
			return(nullptr);

		if(construct)
		{
			if(engine->RegisterObjectBehaviour(name,asBEHAVE_CONSTRUCT,"void f()",asFUNCTION(construct),asCALL_CDECL_OBJLAST)<0)
				return(nullptr);
		}

		if(destruct)
		{
			if(engine->RegisterObjectBehaviour(name,asBEHAVE_DESTRUCT,"void f()",asFUNCTION(destruct),asCALL_CDECL_OBJLAST)<0)
				return(nullptr);
		}

		AngelObject *ao=new AngelObject(engine,name);

		OList.Add(ao);
		return(ao);
	}

	/**
	 * 创建一个对象映射
	 * @param name 对象名称
	 * @param size 对象所占内存字节数
	 * @param ref_factory
	 * @param add_ref 增加引用函数指针
	 * @param del_ref 减少引用函数指针
	 */
	AngelObject *AngelVM::CreateObject(const char *name,int size,void *ref_factory,void *add_ref,void *del_ref)
	{
		if(!engine||!name||!(*name))return(nullptr);
		if(!ref_factory)return(nullptr);

		if(add_ref&&del_ref)
		{
			if(engine->RegisterObjectType(name,size,asOBJ_REF)<0)
				return(nullptr);
		}
		else
		{
			if(engine->RegisterObjectType(name,size,asOBJ_REF|asOBJ_NOCOUNT)<0)
				return(nullptr);
		}

		{
			UTF8String script_func_name=name+UTF8String("@ f()");

			if(engine->RegisterObjectBehaviour(name,AngelScript::asBEHAVE_FACTORY,script_func_name.c_str(),asFUNCTION(ref_factory),asCALL_CDECL)<0)
				return(nullptr);
		}

		if(add_ref)
		{
#if ANGELSCRIPT_VERSION>=22700
			asSFuncPtr p(asCALL_THISCALL);
#else
			asSFuncPtr p;

			asMemClear(&p,sizeof(asSFuncPtr));

			p.flag=asCALL_THISCALL;
#endif//ANGELSCRIPT_VERSION

			p.ptr.f.func = (asFUNCTION_t)(size_t)add_ref;

			if(engine->RegisterObjectBehaviour(name,asBEHAVE_ADDREF,"void f()",p,asCALL_THISCALL)<0)
				return(nullptr);
		}

		if(del_ref)
		{
#if ANGELSCRIPT_VERSION>=22700
			asSFuncPtr p(asCALL_THISCALL);
#else
			asSFuncPtr p;

			asMemClear(&p,sizeof(asSFuncPtr));

			p.flag=asCALL_THISCALL;
#endif//ANGELSCRIPT_VERSION

			p.ptr.f.func = (asFUNCTION_t)(size_t)del_ref;

			if(engine->RegisterObjectBehaviour(name,asBEHAVE_RELEASE,"void f()",p,asCALL_THISCALL)<0)
				return(nullptr);
		}

		AngelObject *ao=new AngelObject(engine,name);

		OList.Add(ao);
		return(ao);
	}

	/**
	 * 创建一个模块
	 * @param module_name 模块名称
	 * @return 模块指针
	 */
	AngelModule *AngelVM::CreateModule(const UTF8String &module_name)
	{
		if(!engine||module_name.IsEmpty())return(nullptr);

		if(MList.Find(module_name)!=-1)//重复添加
			return(nullptr);

		asIScriptModule *module=engine->GetModule(module_name.c_str(),asGM_ALWAYS_CREATE);

		if(!module)
			return(nullptr);

		AngelModule *am=new AngelModule(engine,module
		#ifdef USE_ANGEL_SCRIPT_JIT
		,jit
		#endif//USE_ANGEL_SCRIPT_JIT
		);

		MList.Add(module_name,am);

		return am;
	}

	AngelModule *AngelVM::GetModule(const UTF8String &module_name)
	{
		if(!engine)return(nullptr);

		if(module_name.IsEmpty())return global_module;

		AngelModule *am;

		if(!MList.Get(module_name,am))
			return(nullptr);

		return(am);
	}
};//namespace hgl
