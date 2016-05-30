#include<hgl/script/AngelVM.h>
#include<angelscript.h>

namespace hgl
{
	/**
	 * 映射一个属性
	 * @param decl 属性描述（如：“int test”）
	 * @param off 属性相对偏移地址
	 */
	bool AngelObject::BindProperty(const char *decl,int off)
	{
		if(engine->RegisterObjectProperty(obj_name,decl,off)>=0)
			return(true);

		LOG_ERROR("AngelObject::BindProperty(ObjectName=\""+obj_name+"\" property decl: "+UTF8String(decl));
		return(false);
	}

	/**
	 * 映射一个阵列属性(参数说明假设阵列为：“int array[]”)
	 * @param decl_type 属性描述(如：“int”)
	 * @param decl_name 属性名称描述(如：“array”)
	 * @param get 阵列读取函数，如："int get(uint)"
	 * @param set 阵列写入函数，如："void set(uint,int)"
	 */
	bool AngelObject::BindArray(const char *decl_type,const char *decl_name,void *get,void *set)
	{
		{
			UTF8String get_func=UTF8String(decl_type)+UTF8String(" get_")+UTF8String(decl_name)+UTF8String("(uint)");

			if(engine->RegisterObjectMethod(obj_name,get_func.c_str(),asFUNCTION(get),asCALL_CDECL_OBJLAST)<0)
				return(false);
		}

		{
			UTF8String set_func=UTF8String("void")+UTF8String(" set_")+UTF8String(decl_name)+UTF8String("(uint,")+UTF8String(decl_type)+UTF8String(")");

			if(engine->RegisterObjectMethod(obj_name,set_func.c_str(),asFUNCTION(set),asCALL_CDECL_OBJLAST)>=0)
				return(false);
		}

		return(true);
	}

	/**
	 * 映射一个成员函数
	 * @param decl 函数描述(如："bool create(const string &,int,int)")
	 * @param addr 函数地址
	 */
	bool AngelObject::BindFunc(const char *decl,void *addr)
	{
#if ANGELSCRIPT_VERSION>=22700
		asSFuncPtr p(asCALL_THISCALL);
#else
		asSFuncPtr p;

		asMemClear(&p,sizeof(asSFuncPtr));

		p.flag=asCALL_THISCALL;
#endif//ANGELSCRIPT_VERSION

 		p.ptr.f.func = (asFUNCTION_t)(size_t)addr;

		if(engine->RegisterObjectMethod(obj_name,decl,p,asCALL_THISCALL)>=0)
			return(true);

		LOG_ERROR("AngelObject::BindFunc(ObjectName=\""+obj_name+"\" func decl: "+UTF8String(decl));
		return(false);
	}
}//namespace hgl
