#ifndef HGL_CONSOLE_FLOW_OBJECT_INCLUDE
#define HGL_CONSOLE_FLOW_OBJECT_INCLUDE

#include<hgl/platform/BaseApplication.h>
#include<hgl/object/EnumObject.h>
#include<hgl/object/ConsoleObjectControl.h>
namespace hgl
{
	class BaseApplication;

	/**
	* 流程对象是指可以放在流程控制器里的一种特殊对象
	*/
	class ConsoleFlowObject:public EnumObject                                         	            ///流程对象基类
	{
		friend class ConsoleFlowControl;

	protected:

		ConsoleFlowObject *NextObject;                                                              ///<下一个对象

		FlowObjectState fos;                                                                        ///<当前对象状态

		FlowObjectState GetState(){return fos;}

	public: //属性

		ConsoleObjectControl Control;                                                               ///<对象控制器

		Property<FlowObjectState> ObjectState;                                                      ///<当前对象状态虚拟变量

	public:	//事件

		DefEvent(void,OnDestroy,(ConsoleFlowObject *));												///<销毁事件

		DefEvent(void,OnFlowReturn,(ConsoleFlowObject *));											///<流程返回事件

	public: //方法

		ConsoleFlowObject();																		///<本类构造函数
		virtual ~ConsoleFlowObject();																///<本类析构函数

		virtual ConsoleFlowObject *GetNextObject();													///<取得下一个对象
		virtual void ObjectReturn(ConsoleFlowObject *);												///<对象返回函数

		virtual void Update(){}																		///<刷新函数
		virtual void UpdateObject()
		{
			Update();

			if(Control.Enabled)
				Control.Update();
		}
	};//class ConsoleFlowObject
}//namespace hgl
#endif//HGL_CONSOLE_FLOW_OBJECT_INCLUDE
