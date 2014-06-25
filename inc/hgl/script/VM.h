#ifndef HGL_VM_INCLUDE
#define HGL_VM_INCLUDE

#include<string.h>

/**
* 虚拟机共用要素
*/
namespace hgl
{
	/*
	 * 虚拟机的选择
	 *
	 * 《天使》：较为强大，支持复杂语法与面向对象，支持二进制字节码保存，不支持状态保存，用于通常的执行使用
	 * 《魔鬼》：较为微型，仅支持简单的语法与过程调用，不支持二进制字节码。但执行较快，且支持状态保存与恢复
	 */

	#define vm_method(c,m) vm_method_ptr<sizeof(void (c::*)())>::Convert((void (c::*)())(&c::m))

	template <int>
	struct vm_method_ptr
	{
		template<class M>
		static void *Convert(M obj)
		{
			union u
			{
				M m;
				void *v;
			};

			return ((u *)(&obj))->v;
		}
	};

	#define vm_offset(s,m) ((size_t)(&reinterpret_cast<s*>(100000)->m)-100000)

	//全局用
//	#define MapMemberFunc(name,this_pointer,class,func)		MapFunc(name,this_pointer,vm_method(class,func))
//	#define MapCppFunc(name,this_pointer,class,func)		MapMemberFunc(name,this_pointer,class,func)

	//对象用
	#define MapObject(name,class,create,clear)  			CreateObjectMap(name,sizeof(class),create,clear)
	#define MapObjectProperty(name,class,value) 			MapProperty(name,vm_offset(class,class::value))
	#define MapObjectArray(name_type,name,class,get,set)	MapArray(name_type,name,vm_method(class,get),vm_method(class,set))
	#define MapObjectFunc(name,class,func) 					MapFunc(name,vm_method(class,func))
}//namespace hgl
#endif//HGL_VM_INCLUDE
