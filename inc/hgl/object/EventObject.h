#ifndef HGL_EVENTOBJECT_INCLUDE
#define HGL_EVENTOBJECT_INCLUDE

#include<hgl/object/VisualObject.h>
#include<hgl/platform/InputDevice.h>
namespace hgl
{
	namespace graph
	{
		/**
		* 事件对象是指能够处理被动事件的对象,它们接收来自系统的各种输入事件。<br>
		* 他们都是可视控件，而且在一个对象控制器中有一个事件对像可以被选为活动对像，优先接收各种事件。
		*/
		class EventObject:public VisualObject                                                           ///事件对象
		{
			#include<hgl/object/EventObject.Private.h>

		public: //属性

			Property<bool> Active;                                                                      ///<对象是否处于活动状态

		public: //事件

			DefEvent(bool,OnActive,(Object *,bool));													///<活动事件

			DefEvent(bool,OnEvent,(int,void *));														///<自定义事件

		public:

			EventObject();
			virtual ~EventObject()HGL_DEFAULT_MEMFUNC;

			virtual void Destroy();                                                                     //<销毁当前对象
		};//class EventObject
	}//namespace graph
}//namespace hgl
#endif//HGL_EVENTOBJECT_INCLUDE
