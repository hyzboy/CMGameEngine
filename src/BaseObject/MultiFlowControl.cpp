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
		int max=flow.Count;

		if(n==-1)return;

		while(max--)
			flow[max]->Visible=false;      //将所有的页面设置成不显示

		flow[n]->Visible=true;             //将活动页面设置成显示
		active_flow=fc;
	}

	void MultiFlowControl::Update()
	{
		int n=flow.Count;

		while(n--)
			if(flow[n]->Enabled)
				flow[n]->Update();
	}

	void MultiFlowControl::Draw()
	{
		if(active_flow)
			active_flow->Draw();
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

	#define PROCFUNC(proc,on)	bool MultiFlowControl::proc(int x,int y,uint key)	\
									{	\
										if(active_flow&&active_flow->Enabled)	\
										{	\
											FlowObject *fo=active_flow->ActiveObject;	\
											\
											if(fo)	\
												return fo->proc(x,y,key);	\
										}	\
										\
										return(false);   \
									}

	PROCFUNC(Proc_CursorPos			    ,OnCursorPos		);

	PROCFUNC(Proc_MouseLeftDown		    ,OnMouseLeftDown	);
	PROCFUNC(Proc_MouseLeftUp		    ,OnMouseLeftUp		);
	PROCFUNC(Proc_MouseLeftDoubleClick	,OnMouseLeftDoubleClick	);

	PROCFUNC(Proc_MouseRightDown		,OnMouseRightDown	);
	PROCFUNC(Proc_MouseRightUp		    ,OnMouseRightUp		);
	PROCFUNC(Proc_MouseRightDoubleClick	,OnMouseRightDoubleClick);

	PROCFUNC(Proc_MouseMidDown		    ,OnMouseMidDown		);
	PROCFUNC(Proc_MouseMidUp		    ,OnMouseMidUp		);
	PROCFUNC(Proc_MouseMidDoubleClick	,OnMouseMidDoubleClick	);

	#undef PROCFUNC

	bool MultiFlowControl::Proc_Scroll(int x,int y,int val)
	{
		if(active_flow&&active_flow->Enabled)
		{
			FlowObject *fo=active_flow->ActiveObject;

			if(fo)
				return fo->Proc_Scroll(x,y,val);
		}

		return(false);
	}

	#define PROCFUNC(proc_name,type)	bool MultiFlowControl::proc_name(type key)	\
										{	\
											if(active_flow&&active_flow->Enabled)	\
											{	\
												FlowObject *fo=active_flow->ActiveObject;	\
												\
												if(fo)	\
													return fo->proc_name(key);	\
											}	\
										\
											return(false);	\
										}

	PROCFUNC(Proc_JoystickDown,	uint);
	PROCFUNC(Proc_JoystickPress,uint);
	PROCFUNC(Proc_JoystickUp,	uint);

	PROCFUNC(Proc_KeyDown,	uint);
	PROCFUNC(Proc_KeyPress,	uint);
	PROCFUNC(Proc_KeyUp,	uint);
	PROCFUNC(Proc_Char,		u16char);

	#undef PROCFUNC

	bool MultiFlowControl::Proc_Event           (int type,void *data)
	{
		if(active_flow&&active_flow->Enabled)
		{
			FlowObject *fo=active_flow->ActiveObject;

			if(fo)
				return fo->Proc_Event(type,data);
		}

		return(false);
	}
}//namespace hgl
