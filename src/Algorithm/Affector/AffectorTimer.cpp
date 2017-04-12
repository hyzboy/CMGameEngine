#include<hgl/algorithm/affect/Timer.h>
#include<hgl/Time.h>

namespace hgl
{
    namespace algorithm
    {
        namespace affect
        {
            Timer::Timer()
            {
                run_time = 0;
                last_time = 0;
                last_gap = 0;

                hglSetPropertyRead(RunTime, this, Timer::GetRunTime);
                hglSetPropertyRead(LastTime, this, Timer::GetLastTime);
                hglSetPropertyRead(LastGapTime, this, Timer::GetLastGap);
            }

            bool Timer::Start()
            {
                run_time = 0;
                last_gap = 0;
                last_time = GetDoubleTime();

                return(true);
            }

            bool Timer::Pause()
            {
                Update();

                return(true);
            }

            bool Timer::Resume()
            {
                last_time = GetDoubleTime();

                return(true);
            }

            bool Timer::Stop()
            {
                last_time = 0;

                return(true);
            }

            void Timer::Update()
            {
                if (last_time == 0)return;

                double cur_time = GetDoubleTime();

                last_gap = cur_time - last_time;

                run_time += last_gap;

                last_time = cur_time;
            }
        }//namespace affect
    }//namespace algorithm
}//namespace hgl
