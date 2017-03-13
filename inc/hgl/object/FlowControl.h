#ifndef HGL_FLOW_CONTROL_INCLUDE
#define HGL_FLOW_CONTROL_INCLUDE

#include<hgl/platform/EventBase.h>
#include<hgl/object/FlowObject.h>
#include<hgl/object/_FlowControl.h>
namespace hgl
{
	namespace graph
	{
		class GraphicsApplication;
	}//namespace graph

	/**
	* FlowControl是指流程控制器，它的作用是对一串FlowObject对象的流程进行控制。
	*/
    class FlowControl:public _FlowControl<FlowObject>                                               ///流程对象控制类
	{
		RootEventBase *root_event_base=nullptr;

		void InitPrivate(FlowObject *);

    public: //事件

        virtual void OnResize(int,int){}                                                            ///<画布尺寸调整事件
        virtual void OnRotate(int){}                                                                ///<屏幕旋转事件

	public: //方法

        using _FlowControl<FlowObject>::_FlowControl;
        virtual ~FlowControl(){Clear();}															///<本类析构函数

        RootEventBase *GetEventBase(){return root_event_base;}                                      ///<取得事件收发器

		virtual void Draw(const Matrix4f *);
    };//class FlowControl
}//namespace hgl
#endif//HGL_FLOW_CONTROL_INCLUDE
