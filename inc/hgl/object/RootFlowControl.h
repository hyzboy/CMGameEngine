#ifndef HGL_ROOT_FLOW_CONTROL_INCLUDE
#define HGL_ROOT_FLOW_CONTROL_INCLUDE

#include<hgl/platform/InputDevice.h>
#include<hgl/object/FlowControl.h>
namespace hgl
{
    /**
    * 图形应用根流程控制器
    */
    class RootFlowControl:public FlowControl
    {
    public: //事件

        using FlowControl::FlowControl;
        ~RootFlowControl();

        virtual void OnClose        ();                                                             ///<窗口关闭事件
        virtual void OnResize       (int,int);                                                      ///<窗口尺寸调整事件
        virtual void OnRotate       (int);                                                          ///<屏幕旋转事件

        virtual bool OnMouseMove    (int,int);                                                      ///<鼠标移动事件
        virtual bool OnMouseWheel   (int,int);                                                      ///<鼠标滚轮事件

        virtual bool OnMouseDown    (MouseButton);                                                  ///<鼠标按键按下事件
        virtual bool OnMouseUp      (MouseButton);                                                  ///<鼠标按键弹起事件
        virtual bool OnMouseRepeat  (MouseButton);                                                  ///<鼠标按键自动击发事件(持续按住不放产生的事件)

        virtual bool OnKeyDown      (KeyboardButton);                                               ///<按键按下事件
        virtual bool OnKeyUp        (KeyboardButton);                                               ///<按键弹起事件
        virtual bool OnKeyRepeat    (KeyboardButton);                                               ///<按键自动击发事件(持续按住不放产生的事件)

        virtual bool OnChar         (os_char);                                                      ///<字符输入
    };//class RootFlowControl
}//namespace hgl
#endif//HGL_ROOT_FLOW_CONTROL_INCLUDE
