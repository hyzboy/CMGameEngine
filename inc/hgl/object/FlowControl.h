#ifndef HGL_FLOW_CONTROL_INCLUDE
#define HGL_FLOW_CONTROL_INCLUDE

#include<hgl/object/FlowObject.h>
#include<hgl/type/Stack.h>
namespace hgl
{
	/**
	* FlowControl是指流程控制器，它的作用是对一串FlowObject对象的流程进行控制。
	*
	* FlowControl从FlowObject类派生，也就是说FlowControl也是一个的流程对象。
	*/
	class FlowControl:public FlowObject                                                             ///流程对象控制类
	{
    	friend class BaseApplication;
    	friend class GraphicsApplication;

		#include<hgl/object/Object.ProcEvent.h>

		void Proc_Resize(int,int);

	protected:

		FlowObject *active_object;

		Stack<FlowObject *> objstack;

	protected:

		int GetStackCount(){return objstack.GetCount();}

		FlowObject *GetActiveObject(){return active_object;}

		void ProcState(FlowObjectState);
		void ChangeActiveObject(FlowObject *);

		void InitPrivate(FlowObject *);

	public: //属性

    	Property<int> StackCount;																	///<堆栈数量

		Property<FlowObject *> ActiveObject;                                                        ///<当前活动流程对象

	public: //事件

		DefEvent(void,OnChange,(FlowObject *));														///<当前流程活动对像改变事件

	public: //方法

		FlowControl();																				///<本类构造函数
		FlowControl(FlowObject *);																	///<本类构造函数
		FlowControl(char16_t *,FlowObject *);														///<本类构造函数
		virtual ~FlowControl();																		///<本类析构函数

		virtual void SetStart(FlowObject *);                                                        ///<设定起始流程对象
		virtual void Clear();                                                                       ///<清除本控制器中的对象

		virtual void Exit(FlowObject *next=nullptr);												///<退出当前流程对象
		virtual void ExitToBack(FlowObject *);                                                   	///<退出当前流程对象到后台
		virtual void ExitGame();																	///<退出整个游戏
		virtual void Call(FlowObject *);                                                         	///<呼叫进入子流程对象
		virtual void Return();                                                                      ///<退出子流程对象，返回上一级流程对象
		virtual void ClearTo(FlowObject *);															///<清空当前所有流程对象，进入指定流程对象

		virtual void Update();

		virtual void Draw(const Matrix4f *);

        virtual void ProcCurState();
	};

	/**
	* 控制器自带刷新、绘制处理的流程控制器，一般用于在各流程运行时，仍有一些共用的数据要处理的情况
	*/
	class IAOFlowControl:public FlowControl
	{
	public:

#ifdef HGL_CONSTRUCTION_REUSE
		using FlowControl::FlowControl;
#else
		IAOFlowControl():FlowControl(){}															///<本类构造函数
		IAOFlowControl(FlowObject *fo):FlowControl(fo){}											///<本类构造函数
		IAOFlowControl(char16_t *n,FlowObject *fo):FlowControl(n,fo){}								///<本类构造函数
#endif//HGL_CONSTRUCTION_REUSE
		virtual ~IAOFlowControl()HGL_DEFAULT_MEMFUNC;

		virtual void InitiallyUpdate(){}															///<前初的刷新
		virtual void LastlyUpdate(){}																///<最后的刷新
		virtual void Update()
		{
			InitiallyUpdate();
			FlowControl::Update();
			LastlyUpdate();
		}

		virtual void InitiallyDraw(const Matrix4f *){};												///<最初的绘制
		virtual void LastlyDraw(const Matrix4f *){};												///<最后的绘制
		virtual void Draw(const Matrix4f *mv)
		{
			InitiallyDraw(mv);
			FlowControl::Draw(mv);
			LastlyDraw(mv);
		}
	};//class IAOFlowControl
}//namespace hgl
#endif//HGL_FLOW_CONTROL_INCLUDE
