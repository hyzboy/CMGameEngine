#ifndef DevilEnumH
#define DevilEnumH

#include<hgl/type/BaseString.h>
#include<hgl/type/Map.h>
namespace hgl
{
	/**
	* 枚举定义基类<br>
	* 用于在DevilEngine中定义
	*/
	class DevilEnum																					///枚举定义基类,用于在DevilEngine中定义
	{
	public:

		virtual bool Add(const UTF16String &);

        virtual uint32 GetValue32(const UTF16String &);												///<取得相应枚举值在内存中的32位数据
	};

	template<typename T>class DevilEnumTypedef:public DevilEnum										///枚举数据类型定义基类
	{
		IndexData<UTF16String,T> Items;																///<枚举项名字

	public:

		virtual bool Add(const UTF16String &);
	};

	template<typename T>class DevilEnumSigned:public DevilEnumTypedef<T>						    ///有符号整数型枚举定义基类
	{
	public:

		virtual bool Add(const UTF16String &);
	};

	template<typename T>class DevilEnumUnsigned:public DevilEnumTypedef<T>                          ///无符号整数型枚举定义基类
	{
	public:

		virtual bool Add(const UTF16String &);
	};

	template<typename T>class DevilEnumFloat:public DevilEnumTypedef<T>                             ///浮点数枚举定义基类
	{
	public:

		virtual bool Add(const UTF16String &);
	};

	template<typename T>class DevilEnumString:public DevilEnumTypedef<T>                            ///字符串枚举定义基类
	{
	public:

		virtual bool Add(const UTF16String &);
	};
}//namespace hgl
#endif//DevilEnumH
