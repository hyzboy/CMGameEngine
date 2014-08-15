#include"hgl/object/ConsoleFlowControl.h"

namespace hgl
{
	void ConsoleFlowControl::InitPrivate(ConsoleFlowObject *obj)
	{
		cur=obj;

		object_attrib|=oaControl;

		OnChange=nullptr;
	}

	/**
	* 本类构造函数
	*/
	ConsoleFlowControl::ConsoleFlowControl()
	{
		InitPrivate(nullptr);
	}

	/**
	* 本类构造函数
	* @param obj 本流程控制器的起始流程
	*/
	ConsoleFlowControl::ConsoleFlowControl(ConsoleFlowObject *obj)
	{
		InitPrivate(obj);
	}

	/**
	* 本类构造函数
	* @param name 本流程名称
	* @param obj 本流程控制器的起始流程
	*/
	ConsoleFlowControl::ConsoleFlowControl(u16char *name,ConsoleFlowObject *obj)
	{
		Name=name;

		InitPrivate(obj);
	}

	/**
	* 本类析构函数
	*/
	ConsoleFlowControl::~ConsoleFlowControl()
	{
		Clear();
	}

	/**
	* 设置起始流程对象
	*/
	void ConsoleFlowControl::SetStart(ConsoleFlowObject *obj)
	{
		Clear();

		cur=obj;

		//SafeCallEvent(OnChange,(cur));

		if(OnChange.func_pointer)
			if(OnChange.this_pointer)
				(OnChange.this_pointer->*(OnChange.func_pointer))(cur);
	}

	/**
	* 清除当前控制器中的所有对象
	*/
	void ConsoleFlowControl::Clear()
	{
		if(cur)
		{
			delete cur;
			cur=nullptr;

			SafeCallEvent(OnChange,(nullptr));
		}

		for(;;)
		{
			ConsoleFlowObject *obj=nullptr;

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
	void ConsoleFlowControl::Exit(ConsoleFlowObject *next)
	{
		cur->NextObject=next;

		cur->fos=fosExit;
	}


	/**
	* 退出当前流程对象，放入后台，并进入下一个流程对象
	* @param next 下一个流程对象
	*/
	void ConsoleFlowControl::ExitToBack(ConsoleFlowObject *next)
	{
		cur->NextObject=next;

		cur->fos=fosExitToBack;
	}

	/**
	* 退出整个游戏
	*/
	void ConsoleFlowControl::ExitGame()
	{
		cur->fos=fosExitGame;
	}

	/**
	* 呼叫子流程对象，并暂停当前流程对象
	* @param sub 子流程对象
	*/
	void ConsoleFlowControl::Call(ConsoleFlowObject *sub)
	{
		#ifdef _DEBUG
		if(!sub)
		{
			LOG_ERROR(OS_TEXT("错误！呼叫进入的子流程对象不存在！"));
		}
		else
		#endif//_DEBUG
		{
			cur->NextObject=sub;

			cur->fos=fosExitNotClear;
		}
	}

	/**
	* 进入下一个流程对象，并清空当前整个流程
	* @param next 子流程对象
	*/
	void ConsoleFlowControl::ClearTo(ConsoleFlowObject *next)
	{
		#ifdef _DEBUG
		if(!next)
		{
			LOG_ERROR(OS_TEXT("错误！呼叫进入的子流程对象不存在！"));
		}
		else
		#endif//_DEBUG
		{
			cur->NextObject=next;

			cur->fos=fosExitFlow;
		}
	}

	/**
	* 退出子流程对象，返回上一级流程对象
	*/
	void ConsoleFlowControl::Return()
	{
		cur->fos=fosReturn;
	}

	void ConsoleFlowControl::ChangeActiveObject(ConsoleFlowObject *obj)
	{
		cur=obj;

		SafeCallEvent(OnChange,(cur));
	}

	void ConsoleFlowControl::ProcState(FlowObjectState state)
	{
		if(state==fosOK)return;

		if(state==fosExitNotClear)
		{
			cur->Enabled=false;

			objstack.Push(cur);

			ChangeActiveObject(cur->GetNextObject());

			return;
		}

		if(state==fosExitToBack)
		{
			objstack.Push(cur);

			ChangeActiveObject(cur->GetNextObject());

			return;
		}

		if(state==fosReturn)
		{
			ConsoleFlowObject *ulobj=nullptr;

			objstack.Pop(ulobj);

			if(!ulobj)
			{
				delete cur;
				cur=nullptr;

				ChangeActiveObject(nullptr);

				return;
			}

			ulobj->ObjectReturn(cur);

			ulobj->NextObject=nullptr;

			delete cur;

			ulobj->Enabled=true;

			ulobj->fos=fosOK;

			ChangeActiveObject(ulobj);

			return;
		}

		if(state==fosExit)
		{
			ConsoleFlowObject *next=cur->GetNextObject();

			if(next==nullptr)
			{
				ConsoleFlowObject *ulobj=nullptr;

				objstack.Pop(ulobj);

				if(ulobj)
				{
					ulobj->ObjectReturn(cur);

					ulobj->Enabled=true;

					ulobj->fos=fosOK;

//					delete cur;
//					cur=nullptr;

					ChangeActiveObject(ulobj);
				}
				else
				{
					delete cur;
					cur=nullptr;

					ChangeActiveObject(nullptr);
				}

				fos=fosExitGame;
			}
			else
			{
				delete cur;
				cur=nullptr;

				ChangeActiveObject(next);
			}

			return;
		}

		if(state==fosExitFlow)
		{
			ConsoleFlowObject *next=cur->GetNextObject();

			Clear();			//清空所有流程

			if(next==nullptr)
			{
				cur=nullptr;

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
	void ConsoleFlowControl::Update()
	{
		if(!cur)
		{
			fos=fosExitGame;
			return;
		}

		if(!cur->Enabled)return;

		{
			int n,max=objstack.GetCount();

			for(n=0;n<max;n++)
			{
				ConsoleFlowObject *obj=objstack[n];

				if(obj->Enabled)
					obj->UpdateObject();
			}
		}

		cur->UpdateObject();
	}

    void ConsoleFlowControl::ProcCurState()
    {
		if(!cur)
		{
			fos=fosExitGame;
			return;
		}

		if(!cur->Enabled)return;

		ProcState(cur->ObjectState);
    }
}//namespace hgl
