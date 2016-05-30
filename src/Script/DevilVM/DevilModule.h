#ifndef HGL_DEVIL_SCRIPT_MODULE_INCLUDE
#define HGL_DEVIL_SCRIPT_MODULE_INCLUDE

#include<hgl/script/DevilVM.h>
#include<hgl/type/Map.h>
#include<hgl/type/StringList.h>
#include"DevilCommand.h"
#include"DevilFunc.h"
#include"DevilEnum.h"
namespace hgl
{
	class DevilScriptModule:public DevilModule
	{
		MapObject<UTF16String,DevilPropertyMap>  	prop_map;		//属性映射表
		MapObject<UTF16String,DevilFuncMap> 		func_map;		//函数映射表
		MapObject<UTF16String,DevilFunc>			script_func;	//脚本函数表
		MapObject<UTF16String,DevilEnum>			enum_map;		//枚举映射表

	private:

		bool _MapFunc(const u16char *,void *,void *);

	public:	//内部属性

		UTF16StringList string_list;									//字符串列表

	public:	//内部方法

		DevilFunc *GetScriptFunc(const UTF16String &);
		DevilFuncMap *GetFuncMap(const UTF16String &);
		DevilPropertyMap *GetPropertyMap(const UTF16String &);

	public:

		bool MapProperty(const u16char *,void *);
		bool MapFunc(const u16char *,void *);
//		bool MapFunc(void *,const u16char *,void *);
		bool MapFunc(const u16char *,void *,void *);

		bool AddEnum(const u16char *,DevilEnum *);

		bool AddScript(const u16char *,int=-1);

		void Clear();

	public:	//Debug接口

	#ifdef _DEBUG
		void LogPropertyList();
		void LogMapFuncList();
		void LogScriptFuncList();
	#endif//_DEBUG
	};//HGL_DEVIL_SCRIPT_MODULE_INCLUDE
}//namespace hgl
#endif//HGL_DEVIL_SCRIPT_MODULE_INCLUDE
