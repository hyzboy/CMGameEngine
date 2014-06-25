#include<hgl/affect/Timer.h>
#include<hgl/affect/Affector.h>

namespace hgl
{
	namespace affect
	{
		TrackTimer::TrackTimer(double tt)
		{
			total_time=tt;
			last_pos=0;

			hglSetProperty(		TotalTime,	this,TrackTimer::GetTotalTime,TrackTimer::SetTotalTime);
			hglSetPropertyRead(	Position,	this,TrackTimer::GetLastPos);
			
			OnEnd=nullptr;
			EndAutoDestroy=false;
		}

		bool TrackTimer::Start()
		{
			last_pos=0;

			return Timer::Start();
		}

		void TrackTimer::Update()
		{
			if(total_time<=0
			 ||last_pos>=1)return;

			//if(!Timer::Update())return;
			Timer::Update();

			if(run_time>=total_time)
			{
				last_pos=1;

				if(EndAutoDestroy)							//OnEnd中有可能调用stop之类的函数，所以先清理
					control->Destroy(this);

				SafeCallEvent(OnEnd,(control,this));		//呼叫结束事件
			}
			else
				last_pos=run_time/total_time;
		}
	}//namespace affect
}//namespace hgl
