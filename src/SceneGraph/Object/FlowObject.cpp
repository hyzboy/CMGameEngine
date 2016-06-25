#include"hgl/object/FlowObject.h"
//#include<hgl/platform/InputDevice.h>

namespace hgl
{
	/**
	* 本类构造函数
	*/
	FlowObject::FlowObject()
	{
		object_attrib|=oaFlow;

		fos=fosNotInit;

		NextObject		=nullptr;

		#include<hgl/object/Object.NULL.h>

		OnResize		=nullptr;
		OnDestroy		=nullptr;
		OnFlowReturn	=nullptr;

		KeyPreview      =false;

		hglSetPropertyRead(ObjectState,this,FlowObject::GetState);
	}

	/**
	* 流程对像基类析构函数<br>
	* NextObject如果没有被清除，在这里会被清除
	*/
	FlowObject::~FlowObject()
	{
    	SafeCallEvent(OnDestroy,(this));

		delete NextObject;
	}

	//鼠标移动事件必须每个控件都传,所以要特殊处理
	bool FlowObject::Proc_CursorPos(int x,int y)
	{
		bool result=Control.Proc_CursorPos(x,y);

		if(OnCursorPos!=nullptr)
			return OnCursorPos(x,y);

		return(result);
	}

	#define HGL_PROC_EVENT(name,type1,type2)	bool FlowObject::Proc_##name(type1 v1,type2 v2)	\
												{	\
                                                    if(KeyPreview)  \
                                                    {   \
                                                        if(On##name!=nullptr)	\
                                                            return On##name(v1,v2);	\
                                                        \
                                                        if(Control.Proc_##name(v1,v2))return(true);	\
                                                    }   \
                                                    else    \
                                                    {   \
                                                        if(Control.Proc_##name(v1,v2))return(true);	\
                                                    \
                                                        if(On##name!=nullptr)	\
                                                            return On##name(v1,v2);	\
                                                    }   \
													return(false);	\
												}

	HGL_PROC_EVENT(Scroll,int,int);
	HGL_PROC_EVENT(MouseButton,int,bool);
	HGL_PROC_EVENT(JoystickButton,int,bool);
	HGL_PROC_EVENT(Key,int,bool);
	HGL_PROC_EVENT(Event,int,void *);

	bool FlowObject::Proc_Char(u16char ch)
	{
		if(Control.Proc_Char(ch))return(true);

		if(OnChar!=nullptr)
			return OnChar(ch);

		return(false);
	}

	void FlowObject::Proc_Resize(int w,int h)
	{
		SafeCallEvent(OnResize,(w,h));

		if(!w||!h)return;			//如果是极小化，不处理

		if(Control.Width!=w			//如果不相等才处理
		 ||Control.Height!=h)
		{
			Control.SetSize(w,h);
			Control.SetRefresh();
		}
	}

	/**
	* 取得下一个对象。如果不重载此函数，它将返回当前对象中的NextObject变量
	*
	* 注意：下一个对象可以不是流程对象(FlowObject)，但非流程对象不能进行流程操作，即不能流程控制器清除自己进入下一流程
	*/
	FlowObject *FlowObject::GetNextObject()
	{
		if(NextObject)
		{
			FlowObject *result=NextObject;

			NextObject=nullptr;

			return(result);
		}
		else
        	return(nullptr);
	}

	void FlowObject::ObjectReturn(FlowObject *fo)
	{
    	SafeCallEvent(OnFlowReturn,(fo));
	}
}//namespace hgl
