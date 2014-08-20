#ifndef HGL_ANGELVM_INCLUDE
#define HGL_ANGELVM_INCLUDE

#include<hgl/script/VM.h>
#include<hgl/type/Map.h>

/**
 * AngelVM基于AngelScript实现,根据zlib授权协议取得免费授权.授权协议参见:doc/license/AngelCode Scripting Library.txt
 * AngelScript官方网站：http://www.angelcode.com/angelscript/
 */
namespace AngelScript
{
	class asIScriptEngine;
	class asIScriptModule;
	class asIScriptContext;
	class asIScriptGeneric;
	class asIScriptObject;
	class asIObjectType;
	class asIScriptFunction;
	class asIBinaryStream;
	class asIJITCompiler;

	struct asSMessageInfo;
}//namespace AngelScript

class asCJITCompiler;

namespace hgl
{
	namespace io
	{
		class InputStream;
		class OutputStream;
	}//namespace io

	using namespace AngelScript;

	typedef asIScriptFunction AngelFunc;

	class AngelContext
	{
		asIScriptModule *module;
		asIScriptContext *ctx;
		asIScriptFunction *func;

		bool _Run(va_list &);

	public:

		AngelContext(asIScriptModule *m,asIScriptContext *c)
		{
			module=m;
			ctx=c;
			func=nullptr;
		}

		~AngelContext();

		AngelFunc *GetFunc(const char *);													///<取得指定名称的函数

		bool Start(AngelFunc *);															///<准备开始从某个函数执行
		bool Start(const char *func_name){return Start(GetFunc(func_name));}				///<准备开始从某个函数执行

		bool Run();																			///<执行
		bool Pause();
		bool Stop();

		bool Run(AngelFunc *,...);															///<从某个函数开始执行
		bool Run(const char *func_name,...);												///<从某个函数开始执行
	};//class AngelContext

	class AngelModule
	{
		asIScriptEngine *engine;
		asIScriptModule *module;

		#ifdef USE_ANGEL_SCRIPT_JIT
		asCJITCompiler *jit;
		#endif//USE_ANGEL_SCRIPT_JIT

	public:

		AngelModule(asIScriptEngine *e,asIScriptModule *m
#ifdef USE_ANGEL_SCRIPT_JIT
		,asCJITCompiler *j
#endif//USE_ANGEL_SCRIPT_JIT
		)
		{
			engine=e;
			module=m;
			#ifdef USE_ANGEL_SCRIPT_JIT
			jit=j;
			#endif//USE_ANGEL_SCRIPT_JIT
		}

		virtual ~AngelModule();

		const char *GetName();																		///<取得名字

		bool AddScript(const char *,const char *,int=-1);											///<添加脚本
		bool Build();																				///<编译已添加的脚本

		AngelContext *CreateContext();																///<创建一个运行控制上下文

		bool RunScript(const char *);																///<执行一段脚本

		int SaveByteCode(io::OutputStream *);
		int LoadByteCode(io::InputStream *);
	};//class AngelModule

	class AngelObject
	{
		asIScriptEngine *engine;
		UTF8String obj_name;

	public:

		AngelObject(asIScriptEngine *e,const UTF8String &n)
		{
			engine=e;
			obj_name=n;
		}

		bool BindProperty(const char *,int);
		bool BindArray(const char *,const char *,void *get=nullptr,void *set=nullptr);
		bool BindFunc(const char *,void *);
	};//class AngelObject

#define DEF_ANGEL_OBJECT(script_name,name)	static void Construct##script_name(name *this_pointer)	\
											{	\
												new(this_pointer) name();	\
											}	\
											\
											static void Destruct##script_name(name *this_pointer)	\
											{	\
												this_pointer->~name();	\
											}

#define CreateAngelObject(angel,script_name,name)	angel->CreateObject(#script_name,sizeof(name),(void *)&Construct##script_name,(void *)&Destruct##script_name)

#define CreateAngelRefObject(angel,script_name,name,factory)	angel->CreateObject(#script_name,sizeof(name),(void *)&factory,vm_method(name,inc_ref),vm_method(name,unref))

	class AngelVM
	{
		asIScriptEngine *engine;
		asCJITCompiler *jit;

		AngelModule *global_module;

	private:

		void MessageCallback(const asSMessageInfo *);

	private:

		MapObject<UTF8String,AngelModule> MList;

		ObjectList<AngelObject> OList;

	public:

		AngelVM();
		~AngelVM();

		static bool IsSupportMultiThread();															///<是否支持多线程

		bool BindProperty(const char *,void *);														///<映射一个属性
		bool BindFunc(const char *,void *);															///<映射一个函数

		AngelObject *	CreateObject(const char *,int,void *construct,void *destruct);				///<映射一个对象

		AngelObject *	CreateObject(const char *,int,
									 void *ref_factory,void *add_ref,void *del_ref);				///<映射一个对象

		AngelModule *	CreateModule(const UTF8String &);											///<创建一个模块
		AngelModule *	GetModule(const UTF8String &);												///<取得一个模块
	};//class AngelVM
}//namespace hgl
#endif//HGL_ANGELVM_INCLUDE
