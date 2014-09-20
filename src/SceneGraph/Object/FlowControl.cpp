#include"hgl/object/FlowControl.h"

namespace hgl
{
	void FlowControl::InitPrivate(FlowObject *obj)
	{
		active_object=obj;

		object_attrib|=oaControl;

		OnChange=nullptr;

		hglSetPropertyRead(ActiveObject,this,FlowControl::GetActiveObject);

		hglSetPropertyRead(StackCount,this,FlowControl::GetStackCount);
	}

	/**
	* 本类构造函数
	*/
	FlowControl::FlowControl()
	{
		InitPrivate(nullptr);
	}

	/**
	* 本类构造函数
	* @param obj 本流程控制器的起始流程
	*/
	FlowControl::FlowControl(FlowObject *obj)
	{
		InitPrivate(obj);
	}

	/**
	* 本类构造函数
	* @param name 本流程名称
	* @param obj 本流程控制器的起始流程
	*/
	FlowControl::FlowControl(u16char *name,FlowObject *obj)
	{
		Name=name;

		InitPrivate(obj);
	}

	/**
	* 本类析构函数
	*/
	FlowControl::~FlowControl()
	{
		Clear();
	}

	/**
	* 设置起始流程对象
	*/
	void FlowControl::SetStart(FlowObject *obj)
	{
		Clear();

		active_object=obj;

		SafeCallEvent(OnChange,(active_object));
	}

	/**
	* 清除当前控制器中的所有对象
	*/
	void FlowControl::Clear()
	{
		if(active_object)
		{
			delete active_object;
			active_object=nullptr;

			SafeCallEvent(OnChange,(nullptr));
		}

		for(;;)
		{
			FlowObject *obj=nullptr;

			objstack.Pop(obj);

			if(obj)
				delete obj;
			else break;
		}
	}

	/**
	* 退出当前流程对象，并进入下一个流程对象，如未指定下一个流程对象，则退出当前流程
	* @param next 下一个流程对象
	*/
	void FlowControl::Exit(FlowObject *next)
	{
		active_object->NextObject=next;

		active_object->fos=fosExit;
	}

	/**
	* 退出当前流程对象，放入后台，并进入下一个流程对象
	* @param next 下一个流程对象
	*/
	void FlowControl::ExitToBack(FlowObject *next)
	{
		active_object->NextObject=next;

		active_object->fos=fosExitToBack;
	}

	/**
	* 退出整个游戏
	*/
	void FlowControl::ExitGame()
	{
		active_object->fos=fosExitGame;
	}

	/**
	* 呼叫子流程对象，并暂停当前流程对象
	* @param sub 子流程对象
	*/
	void FlowControl::Call(FlowObject *sub)
	{
		#ifdef _DEBUG
		if(!sub)
		{
			LOG_ERROR(OS_TEXT("错误！呼叫进入的子流程对象不存在！"));
		}
		else
		#endif//_DEBUG
		{
			active_object->NextObject=sub;

			active_object->fos=fosExitNotClear;
		}
	}

	/**
	* 进入下一个流程对象，并清空当前整个流程
	* @param next 子流程对象
	*/
	void FlowControl::ClearTo(FlowObject *next)
	{
		#ifdef _DEBUG
		if(!next)
		{
			LOG_ERROR(OS_TEXT("错误！呼叫进入的子流程对象不存在！"));
		}
		else
		#endif//_DEBUG
		{
			active_object->NextObject=next;

			active_object->fos=fosExitFlow;
		}
	}

	/**
	* 退出子流程对象，返回上一级流程对象
	*/
	void FlowControl::Return()
	{
		active_object->fos=fosReturn;
	}

	void FlowControl::ChangeActiveObject(FlowObject *obj)
	{
		active_object=obj;

		SafeCallEvent(OnChange,(active_object));
	}

	void FlowControl::ProcState(FlowObjectState state)
	{
		if(state==fosOK)return;

		if(state==fosExitNotClear)
		{
			active_object->Enabled=false;
			active_object->Visible=false;

			objstack.Push(active_object);

			ChangeActiveObject(active_object->GetNextObject());

			return;
		}

		if(state==fosExitToBack)
		{
			objstack.Push(active_object);

			ChangeActiveObject(active_object->GetNextObject());

			return;
		}

		if(state==fosReturn)
		{
			FlowObject *ulobj=nullptr;

			objstack.Pop(ulobj);

			if(!ulobj)
			{
				delete active_object;
				active_object=nullptr;

				ChangeActiveObject(nullptr);

				return;
			}

			ulobj->ObjectReturn(active_object);

			ulobj->NextObject=nullptr;

			delete active_object;

			ulobj->Visible=true;
			ulobj->Enabled=true;

			ulobj->fos=fosOK;

			ChangeActiveObject(ulobj);

			return;
		}

		if(state==fosExit)
		{
			FlowObject *next=active_object->GetNextObject();

			if(next==nullptr)
			{
				FlowObject *ulobj=nullptr;

				objstack.Pop(ulobj);

				if(ulobj)
				{
					ulobj->ObjectReturn(active_object);

					ulobj->Visible=true;
					ulobj->Enabled=true;

					ulobj->fos=fosOK;

					delete active_object;
					active_object=nullptr;

					ChangeActiveObject(ulobj);
				}
				else
				{
					delete active_object;
					active_object=nullptr;

					ChangeActiveObject(nullptr);
				}

				fos=fosExitGame;
			}
			else
			{
				delete active_object;
				active_object=nullptr;

				ChangeActiveObject(next);
			}

			return;
		}

		if(state==fosExitFlow)
		{
			FlowObject *next=active_object->GetNextObject();

			Clear();			//清空所有流程

			if(next==nullptr)
			{
				active_object=nullptr;

				fos=fosExitGame;
			}
			else
			{
				ChangeActiveObject(next);
			}

			return;
		}

		if(state==fosExitGame)
		{
			Clear();

			fos=fosExitGame;

			return;
		}
	}

	/**
	* 刷新当前控制器中的对象
	*/
	void FlowControl::Update()
	{
		if(!active_object)
		{
			fos=fosExitGame;
			return;
		}

		if(!active_object->Enabled)return;

		{
			int n,max=objstack.GetCount();

			for(n=0;n<max;n++)
			{
				FlowObject *obj=objstack[n];

				if(obj->Enabled)
					obj->UpdateObject();
			}
		}

		active_object->UpdateObject();
	}

	/**
	* 调用当前控制器中的对象绘制画面
	*/
	void FlowControl::Draw(const Matrix4f *mv)
	{
		if(!active_object)return;

		if(!active_object->Visible)return;

		{
			int n,count=objstack.GetCount();

			for(n=0;n<count;n++)
			{
				FlowObject *obj=objstack[n];

				if(obj->Visible)
                	obj->DrawObject(mv);
			}
		}

		active_object->DrawObject(mv);
	}

    void FlowControl::ProcCurState()
    {
		if(!active_object)
		{
			fos=fosExitGame;
			return;
		}

		if(!active_object->Enabled)return;

		ProcState(active_object->ObjectState);
    }
}//namespace hgl

namespace hgl
{
	#define PROC(func_name,type1,type2)	bool FlowControl::func_name(type1 param1,type2 param2)	\
										{	\
											if(active_object)	\
												return active_object->func_name(param1,param2);	\
											else	\
												return(false);	\
										}

	PROC(Proc_CursorPos,		int,int);
	PROC(Proc_Scroll,			int,int);
	PROC(Proc_MouseButton,		int,bool);
	PROC(Proc_JoystickButton,	int,bool);
	PROC(Proc_Key,				int,bool);
	PROC(Proc_Event,			int,void *);

	#undef PROC

	bool FlowControl::Proc_Char(u16char ch)
	{
		if(active_object)
			return active_object->Proc_Char(ch);
		else
			return(false);
	}

	void FlowControl::Proc_Resize(int w,int h)
	{
		int n,count=objstack.GetCount();

		for(n=0;n<count;n++)
		{
			FlowObject *obj=objstack[n];

           	obj->Proc_Resize(w,h);
		}

		if(active_object)
			active_object->Proc_Resize(w,h);
	}
}//namespace hgl
