#ifndef HGL_BASE_APPLICATION_INCLUDE
#define HGL_BASE_APPLICATION_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/platform/SystemInfo.h>
namespace hgl
{
	class ConsoleSystemInitInfo;

	/** \releates ConsoleFlowObject
	* 流程对象状态枚举
	* @see ConsoleFlowObject
	* @see ControlFlowObject
	*/
	enum FlowObjectState
	{
		fosNotInit=0,                   ///<未初始化
		fosOK,                          ///<正常

		fosExitNotClear,                ///<退出，但不清除
		fosExitToBack,                  ///<退出到后台，继续绘制和刷新，但不接收事件
		fosReturn,                      ///<返回上一个状态为ExitNotClear/ExitToBack的对象

		fosExit,                        ///<退出，进入下一个对象
		fosExitFlow,                    ///<退出当前流程控制中的所有流程，并进入指定的下一个流程
		fosExitGame,                    ///<退出整个游戏
	};

	/**
	* 应用程序基类
	*/
	class BaseApplication																			///应用程序基类
	{
	protected:

		ConsoleSystemInitInfo *sii;

		double cur_time;

		bool active;

	protected:

		ConsoleSystemInitInfo *	GetSII		()		{return sii;}

		double					GetTime		()		{return cur_time;}

		void					UpdateTime	();

		bool					GetActive	()		{return active;}
		void 					SetActive	(bool);

	public: //需要用户在启动游戏前设置的数据

		OSString							ProjectName;											///<游戏名称
		OSString							ProjectCode;											///<游戏代码

	public: //属性

		SystemInfo							si;                                                     ///<系统信息

		Property<ConsoleSystemInitInfo *>	SII;													///<系统初始化信息
		Property<double>					Time;													///<当前时间(秒)

		Property<bool>						Active;                                         		///<当前程序是否处于活动状态
		bool								WaitActive;                                        		///<是否等待活动状态,默认为false

	public:	//事件

		DefEvent(void,OnActive,(bool));																///<程序活动状态变更事件

	public:

		BaseApplication();
		virtual ~BaseApplication();

		virtual bool Init(ConsoleSystemInitInfo *);													///<初始化当前应用程序

		virtual int Run()=0;																	    ///<运行当前应用程序

		virtual void ExitGame()=0;																	///<退出游戏
	};//class BaseApplication
}//namespace hgl
#endif//HGL_BASE_APPLICATION_INCLUDE;
