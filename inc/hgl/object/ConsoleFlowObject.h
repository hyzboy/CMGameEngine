#ifndef HGL_CONSOLE_FLOW_OBJECT_INCLUDE
#define HGL_CONSOLE_FLOW_OBJECT_INCLUDE

#include<hgl/object/_FlowObject.h>
namespace hgl
{
	/**
	* 流程对象是指可以放在流程控制器里的一种特殊对象
	*/
    class ConsoleFlowObject:public _FlowObject<ConsoleFlowObject>                                   ///流程对象基类
	{
    public:

        using _FlowObject<ConsoleFlowObject>::_FlowObject;

		virtual void Update(){}																		///<刷新函数
	};//class ConsoleFlowObject
}//namespace hgl
#endif//HGL_CONSOLE_FLOW_OBJECT_INCLUDE
