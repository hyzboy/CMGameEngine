#ifndef HGL_ENUM_OBJECT_INCLUDE
#define HGL_ENUM_OBJECT_INCLUDE

#include<hgl/object/Object.h>
namespace hgl
{
	/**
	* 枚举对象是指每个回合都会被调用刷新(Update函数)的对象
	*/
	class EnumObject:public Object
	{
	public:

		EnumObject(bool=true);
		virtual ~EnumObject()HGL_DEFAULT_MEMFUNC;

		virtual void Update();
	};//class EnumObject
}//namespace hgl
#endif//HGL_ENUM_OBJECT_INCLUDE
