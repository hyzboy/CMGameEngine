#ifndef HGL_CONSOLE_FLOW_OBJECT_INCLUDE
#define HGL_CONSOLE_FLOW_OBJECT_INCLUDE

#include<hgl/platform/Platform.h>
#include<hgl/object/FlowObjectState.h>
namespace hgl
{
	/**
	* 流程对象是指可以放在流程控制器里的一种特殊对象
	*/
    class ConsoleFlowObject                                                                         ///流程对象基类
	{
	protected:

		ConsoleFlowObject *NextObject;                                                              ///<下一个对象

		FlowObjectState fos;                                                                        ///<当前对象状态

    protected:  //事件

        virtual void OnDestroy(){/*重载此处理本对象销毁事件*/}                                          ///<当前对象销毁事件

	public: //方法

		ConsoleFlowObject();																		///<本类构造函数
		virtual ~ConsoleFlowObject();																///<本类析构函数

        const FlowObjectState GetState()const{return fos;}                                          ///<取得当前对象状态

		virtual ConsoleFlowObject *GetNextObject();													///<取得下一个对象

        /**
         * 流程返回事件
         * @param prev_fo 之前执行的对象
         */
		virtual void ObjectReturn(ConsoleFlowObject *prev_fo){/*重载此函数以处理返回事件*/}				///<对象返回函数

		virtual void Update(){}																		///<刷新函数
	};//class ConsoleFlowObject
}//namespace hgl
#endif//HGL_CONSOLE_FLOW_OBJECT_INCLUDE
