#ifndef HGL_CONSOLE_FLOW_CONTROL_INCLUDE
#define HGL_CONSOLE_FLOW_CONTROL_INCLUDE

#include<hgl/object/ConsoleFlowObject.h>
#include<hgl/type/Stack.h>
namespace hgl
{
	/**
	* ConsoleFlowControl是指流程控制器，它的作用是对一串ConsoleFlowObject对象的流程进行控制。
	*
	* ConsoleFlowControl从ConsoleFlowObject类派生，也就是说ConsoleFlowControl也是一个的流程对象。
	*/
	class ConsoleFlowControl:public ConsoleFlowObject												///流程对象控制类
	{
    	friend class BaseApplication;

		ConsoleFlowObject *cur;

		Stack<ConsoleFlowObject *> objstack;

	protected:

		void ProcState(FlowObjectState);
		void ChangeActiveObject(ConsoleFlowObject *);

		void InitPrivate(ConsoleFlowObject *);

	public: //属性

		int GetStackCount()const{return objstack.GetCount();}										///<取得当前堆栈数量

		ConsoleFlowObject *GetActiveObject(){return cur;}											///<取得当前活动流程对象

	public: //事件

		DefEvent(void,OnChange,(ConsoleFlowObject *));												///<当前流程活动对像改变事件

	public: //方法

		ConsoleFlowControl();                                                                       ///<本类构造函数
		ConsoleFlowControl(ConsoleFlowObject *);                                                    ///<本类构造函数
		ConsoleFlowControl(u16char *,ConsoleFlowObject *);                                          ///<本类构造函数
		virtual ~ConsoleFlowControl();                                                              ///<本类析构函数

		virtual void SetStart(ConsoleFlowObject *);                                                 ///<设定起始流程对象
		virtual void Clear();                                                                       ///<清除本控制器中的对象

		virtual void Exit(ConsoleFlowObject *next=nullptr);											///<退出当前流程对象
		virtual void ExitToBack(ConsoleFlowObject *);                                               ///<退出当前流程对象到后台
		virtual void ExitApp();																	    ///<退出整个游戏
		virtual void Call(ConsoleFlowObject *);                                                     ///<呼叫进入子流程对象
		virtual void Return();                                                                      ///<退出子流程对象，返回上一级流程对象
		virtual void ClearTo(ConsoleFlowObject *);													///<清空当前所有流程对象，进入指定流程对象

		virtual void Update();																		///<刷新当前流程控制器

        virtual void ProcCurState();																///<处理当前流程
	};//class ConsoleFlowControl

	/**
	* 控制器自带刷新、绘制处理的流程控制器，一般用于在各流程运行时，仍有一些共用的数据要处理的情况
	*/
	class IAOFlowControl:public ConsoleFlowControl
	{
	public:

		using ConsoleFlowControl::ConsoleFlowControl;
		virtual ~IAOFlowControl()=default;

		virtual void InitiallyUpdate(){}															///<前初的刷新
		virtual void LastlyUpdate(){}																///<最后的刷新
		virtual void Update()
		{
			InitiallyUpdate();
			ConsoleFlowControl::Update();
			LastlyUpdate();
		}
	};//class IAOFlowControl
}//namespace hgl
#endif//HGL_CONSOLE_FLOW_CONTROL_INCLUDE
