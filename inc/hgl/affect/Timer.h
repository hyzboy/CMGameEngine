#ifndef HGL_AFFECTOR_TIMER_INCLUDE
#define HGL_AFFECTOR_TIMER_INCLUDE

#include<hgl/affect/AffectorObject.h>
namespace hgl
{
	/**
	* 影响器名字空间
	*/
	namespace affect																				///影响器名字空间
	{
		/**
		* 计时影响器<br>
		* 影响器的一种，作用是计时。但要注意它并不精确。
		*/
		class Timer:public AffectorObject															///计时影响器
		{
		protected:

			double run_time;																		///<总计运行时间
			double last_time;																		///<最终执行时间
			double last_gap;																		///<最终执行时间间隔

			double GetRunTime(){return run_time;}
			double GetLastTime(){return last_time;}
			double GetLastGap(){return last_gap;}

		public:	//属性

			Property<double> RunTime;																///<运行时间
			Property<double> LastTime;																///<最后执行时间
			Property<double> LastGapTime;															///<最后一次执行间隔时间

		public:	//方法

			Timer();
			virtual ~Timer()HGL_DEFAULT_MEMFUNC;

			virtual const u16char *GetClassName()const{ return U16_TEXT("Timer"); }					///<取得类名

			virtual bool Start();
			virtual bool Pause();
			virtual bool Resume();
			virtual bool Stop();

			virtual void Update();
		};//class Timer

		/**
		* 时间进度追踪影响器<br>
		* 影响器的一种，用于固定时间段的进度记时。但要注意它并不精确。
		*/
		class TrackTimer:public Timer																///时间进度影响器
		{
		protected:

			double total_time;																		///<总计所指定的运行时间
			double last_pos;																		///<最后进度

			void SetTotalTime(double time){total_time=time;}
			double GetTotalTime(){return total_time;}

			double GetLastPos(){return last_pos;}

		public:	//属性

			Property<double> TotalTime;																///<总计时间(可读可写)
			Property<double> Position;																///<进度场景

			bool EndAutoDestroy;																	///<是否结束后自动销毁

		public:	//事件

			DefEvent(void,OnEnd,(AffectorControl *,AffectorObject *));								///<结束事件

		public:	//方法

			TrackTimer(double=1.0);
			virtual ~TrackTimer()HGL_DEFAULT_MEMFUNC;

			virtual const u16char *GetClassName()const{ return U16_TEXT("TrackTimer"); }			///<取得类名

			virtual bool Start();

			virtual void Update();
		};//class TrackTimer
	}//namespace affect
}//namespace hgl
#endif//HGL_AFFECTOR_TIMER_INCLUDE
