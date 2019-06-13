#include<hgl/object/MultiFlowControl.h>

namespace hgl
{
    MultiFlowControl::MultiFlowControl()
    {
        object_attrib|=oaVisual|oaUpdate|oaProcEvent|oaFlow|oaControl;

        active_flow=nullptr;

        hglSetProperty(ActiveFlow,this,MultiFlowControl::GetActiveFlow,MultiFlowControl::SetActiveFlow);
    }

    MultiFlowControl::~MultiFlowControl()
    {
    }

    void MultiFlowControl::SetActiveFlow(FlowControl *fc)
    {
        int n=flow.Find(fc);
        int max=flow.GetCount();

        if(n==-1)return;

        while(max--)
            flow[max]->Visible=false;      //将所有的页面设置成不显示

        flow[n]->Visible=true;             //将活动页面设置成显示
        active_flow=fc;
    }

    void MultiFlowControl::Update()
    {
        int n=flow.GetCount();

        while(n--)
            if(flow[n]->Enabled)
                flow[n]->Update();
    }

    void MultiFlowControl::Draw(const Matrix4f *mv)
    {
        if(active_flow)
            active_flow->Draw(mv);
    }

    void MultiFlowControl::SetMenuCoord(float x,float y)
    {
        if(active_flow&&active_flow->Enabled)
        {
            FlowObject *fo=active_flow->ActiveObject;

            fo->Control.SetMenuCoord(x,y);
            fo->Control.SetRefresh();
        }
    }

    void MultiFlowControl::operator += (FlowControl *fc)
    {
        flow+=fc;
    }

    /**
    * 创建一个新的流程
    * @param fo 新流程的起始流程对象
    * @return 新流程的流程控制器
    */
    FlowControl *MultiFlowControl::CreateFlow(FlowObject *fo)
    {
        FlowControl *fc=new FlowControl(fo);

        flow+=fc;

        return(fc);
    }

    #define PROC(func_name,type1,type2) bool MultiFlowControl::func_name(type1 param1,type2 param2) \
                                        {   \
                                            if(active_flow&&active_flow->Enabled)   \
                                            {   \
                                                FlowObject *fo=active_flow->ActiveObject;   \
                                                \
                                                if(fo)  \
                                                    return fo->func_name(param1,param2);    \
                                            }   \
                                            \
                                            return(false);   \
                                        }

    PROC(Proc_CursorPos,        int,int);
    PROC(Proc_Scroll,           int,int);
    PROC(Proc_MouseButton,      int,bool);
    PROC(Proc_JoystickButton,   int,bool);
    PROC(Proc_Key,              int,bool);
    PROC(Proc_Event,            int,void *);

    #undef PROC

    bool MultiFlowControl::Proc_Char(u16char ch)
    {
        if (active_flow&&active_flow->Enabled)
        {
            FlowObject *fo = active_flow->ActiveObject;

            if (fo)
                return fo->Proc_Char(ch);
        }

        return(false);
    }
}//namespace hgl
