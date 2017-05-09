#ifndef HGL_ROOT_FLOW_CONTROL_INCLUDE
#define HGL_ROOT_FLOW_CONTROL_INCLUDE

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
        ~RootFlowControl()=default;

        virtual void OnClose        (               );                                              ///<窗口关闭事件
        virtual void OnResize       (int,int        );                                              ///<窗口尺寸调整事件
    };//class RootFlowControl
}//namespace hgl
#endif//HGL_ROOT_FLOW_CONTROL_INCLUDE
