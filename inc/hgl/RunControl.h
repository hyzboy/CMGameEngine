#ifndef HGL_RUN_CONTROL_INCLUDE
#define HGL_RUN_CONTROL_INCLUDE

#include<hgl/type/Stack.h>
namespace hgl
{
	/**
	* 流程执行控制模板
	*/
	template<typename T> class RunControl																///流程执行控制模板
	{
	protected:

		StackObject<T> stack_proc;																		///<流程堆栈
		T *cur_proc;																					///<当前流程
		T *next_proc;																					///<下一流程
		T *prev_proc;																					///<上一流程

	public:	//流程控制方法

		RunControl()
		{
			cur_proc=nullptr;
			next_proc=nullptr;
			prev_proc=nullptr;
		}

		virtual ~RunControl()
		{
			SAFE_CLEAR(prev_proc);
			SAFE_CLEAR(next_proc);
			SAFE_CLEAR(cur_proc);
			stack_proc.Clear();
		}

		virtual bool SetStart(T *obj)																	///<设置起始流程
		{
			if(cur_proc)return(false);

			cur_proc=obj;
			return(true);
		}

		virtual bool SetNextProc(T *obj)																///<设置下一流程
		{
			if(!obj)return(false);
			if(next_proc)return(false);

			next_proc=obj;
			return(true);
		}

		virtual void SetExit()																			///<设置完全退出
		{
			SAFE_CLEAR(next_proc);
			SetExitCur();
		}

		virtual bool SetExitCur(T *obj=nullptr)															///<设置退出当前流程状态
		{
			if(obj)
				if(!SetNextProc(obj))
					return(false);

			SAFE_CLEAR(prev_proc);

			prev_proc=cur_proc;
			cur_proc=next_proc;
			next_proc=nullptr;
			return(true);
		}

		virtual bool SetExitCurToBack(T *obj=nullptr)														///<设置退出当前流程到后台状态(不清除，压入堆栈，供以后Return)
		{
			if(obj)
				if(!SetNextProc(obj))
					return(false);

			if(cur_proc)
				stack_proc.Push(cur_proc);

			cur_proc=next_proc;
			next_proc=nullptr;
		}

		virtual bool SetReturnPrev()																	///<设置返回上一流程状态
		{
			prev_proc=cur_proc;

			if(stack_proc.Pop(cur_proc))
				return(true);

			cur_proc=nullptr;
			return(false);
		}

		virtual bool RunFrame()																			///<运行一帧
		{
			if(!cur_proc)return(false);

			cur_proc->Update();
		}

		virtual bool Run()																				///<运行
		{
			while(RunFrame());
		}
	};//template<typename T> class RunControl
}//namespace hgl
#endif//HGL_RUN_CONTROL_INCLUDE
