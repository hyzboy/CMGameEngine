#ifndef HGL_MULTIFLOWCONTROL_INCLUDE
#define HGL_MULTIFLOWCONTROL_INCLUDE

#include<hgl/object/FlowControl.h>
namespace hgl
{
    /**
    * 多流程控制器用于同时运行多个流程。不管在何时,每个流程都会被刷新,<br>
    * 但只有活动的活动才会被调用绘制函数,也只有活动的流程中才会被传递进事件。<br>
    * 要注意的是: 由于要和主线程同步，所以每个流程并不是独立线程运行的，而是轮流在主线程内运行。
    */
    class MultiFlowControl:public FlowObject                                                        ///多页控制控件
    {
        #include<hgl/object/Object.ProcEvent.h>

    private:

        ObjectList<FlowControl> flow;

        FlowControl *active_flow;

    protected:

        FlowControl *GetFlow(int n){return (n<0||n>=flow.GetCount())?NULL:flow[n];}

        FlowControl *GetActiveFlow(){return active_flow;}
        void SetActiveFlow(FlowControl *);

    public:

        Property<FlowControl *> ActiveFlow;                                                         ///<活动流程访问虚拟变量

    public:

        MultiFlowControl();
        virtual ~MultiFlowControl();

        virtual void Update();

        virtual void Draw(const Matrix4f *);

        virtual void SetMenuCoord(float,float);

        virtual void operator += (FlowControl *);                                                   ///<添加一个流程到当前控制器

        FlowControl *operator[](int n){return GetFlow(n);}

        virtual FlowControl *CreateFlow(FlowObject *);                                              ///<使用指定起始流程对象创建一个流程
    };
}//namespace hgl
#endif//HGL_MULTIFLOWCONTROL_INCLUDE
