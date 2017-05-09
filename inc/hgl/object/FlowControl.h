#ifndef HGL_FLOW_CONTROL_INCLUDE
#define HGL_FLOW_CONTROL_INCLUDE

#include<hgl/object/FlowObject.h>
#include<hgl/object/_FlowControl.h>
namespace hgl
{
	/**
	* FlowControl是指流程控制器，它的作用是对一串FlowObject对象的流程进行控制。
	*/
    class FlowControl:public _FlowControl<FlowObject>                                               ///流程对象控制类
	{
		void InitPrivate(FlowObject *);

    protected:

        virtual void ChangeActiveObject(FlowObject *obj)override;

    public: //事件
        
        virtual void OnClose        (               );                                              ///<窗口关闭事件
        virtual void OnResize       (int,int        );                                              ///<窗口尺寸调整事件
        virtual void OnRotate       (int            );                                              ///<屏幕旋转事件

        virtual bool OnMouseMove    (int,int        );                                              ///<鼠标移动事件
        virtual bool OnMouseWheel   (int,int        );                                              ///<鼠标滚轮事件

        virtual bool OnMouseDown    (MouseButton    );                                              ///<鼠标按键按下事件
        virtual bool OnMouseUp      (MouseButton    );                                              ///<鼠标按键弹起事件
        virtual bool OnMouseRepeat  (MouseButton    );                                              ///<鼠标按键自动击发事件(持续按住不放产生的事件)

        virtual bool OnKeyDown      (KeyboardButton );                                              ///<按键按下事件
        virtual bool OnKeyUp        (KeyboardButton );                                              ///<按键弹起事件
        virtual bool OnKeyRepeat    (KeyboardButton );                                              ///<按键自动击发事件(持续按住不放产生的事件)

        virtual bool OnChar         (os_char        );                                              ///<字符输入

	public: //方法

        using _FlowControl<FlowObject>::_FlowControl;
        virtual ~FlowControl(){Clear();}															///<本类析构函数

		virtual void Draw(const Matrix4f *);
    };//class FlowControl
}//namespace hgl
#endif//HGL_FLOW_CONTROL_INCLUDE
