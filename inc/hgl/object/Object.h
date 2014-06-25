#ifndef HGL_OBJECT_INCLUDE
#define HGL_OBJECT_INCLUDE

#include<hgl/object/_Object.h>
#include<hgl/type/BaseString.h>
namespace hgl
{
	namespace graph
	{
		class ObjectControl;
	}//namespace graph

	using namespace graph;

	/**
	* Object类是HGL类库中所有游戏对象类的基类,它提供对象该具备的所有基本要素。
	*/
	class Object:public _Object                                                                     ///对象基类
	{
		#include<hgl/object/Object.Attrib.h>

	public:	//属性

		UTF16String      				Name;                                                       ///<对象名称

		PropertyObject<ObjectControl>	SuperObject;                                                ///<上级对象

		Property<uint>					Attrib;                                                     ///<对象类型

		Property<bool>					Enabled;                                                    ///<对象是否启用

	public: //事件

		//关联/断开事件,格式为 OnLink(上级对象,本对象)

		DefEvent(void,OnLink,(Object *,Object *));													///<关联事件
		DefEvent(void,OnUnlink,(Object *,Object *));												///<断开连接事件

	public:

		Object();                                                                                   ///<本类构造函数
		Object(const UTF16String &);																///<本类构造函数
		virtual ~Object();                                                                          ///<本类析构函数

		virtual void Destroy();                                                                     ///<销毁当前对象
	};//class Object
}//namespace hgl
#endif//HGL_OBJECT_INCLUDE
