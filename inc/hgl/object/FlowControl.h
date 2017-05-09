#ifndef HGL_FLOW_CONTROL_INCLUDE
#define HGL_FLOW_CONTROL_INCLUDE

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
		void InitPrivate(FlowObject *);

    protected:

        virtual void ChangeActiveObject(FlowObject *obj)override;

    public: //事件
        
        virtual void OnClose        (){}                                                            ///<窗口关闭事件
        virtual void OnResize       (){}                                                            ///<窗口尺寸调整事件
        virtual void OnRotate       (int){}                                                         ///<屏幕旋转事件

	public: //方法

        using _FlowControl<FlowObject>::_FlowControl;
        virtual ~FlowControl(){Clear();}															///<本类析构函数

		virtual void Draw(const Matrix4f *);
    };//class FlowControl
}//namespace hgl
#endif//HGL_FLOW_CONTROL_INCLUDE
