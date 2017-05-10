#include<hgl/algorithm/affect/Timer.h>
#include<hgl/algorithm/Rand.h>

namespace hgl
{
    namespace algorithm
    {
	    namespace affect
	    {
		    /**
		    * 矢量跟踪
		    * @param c 起始坐标
		    * @param s 速度/秒
		    */
		    template<typename T>
		    VectorTrace<T>::VectorTrace(Timer *t,const T &c,const T &s)
		    {
			    time=t;

			    Coord		=c;
			    Speed		=s;

			    TraceRate	=FastRand(2,9);

			    MaxSpeed		=1000;
			    MinDistance		=10;
			    MaxSpeedDistance=2000;
			    DecSpeed		=0.75f;

			    OnToEnd		=nullptr;
		    }

            namespace 
            {
                template<typename T> void SpeedAddRand(T &speed);

                template<Vector1f> inline void SpeedAddRand(Vector1f &speed)
                {
                    speed.x=rand(-1.0,1.0,0.01);
                }

                template<Vector2f> inline void SpeedAddRand(Vector2f &speed)
                {
                    speed.x=rand(-1.0,1.0,0.01);
                    speed.y=rand(-1.0,1.0,0.01);
                }

                template<Vector3f> inline void SpeedAddRand(Vector3f &speed)
                {
                    speed.x=rand(-1.0,1.0,0.01);
                    speed.y=rand(-1.0,1.0,0.01);
                    speed.z=rand(-1.0,1.0,0.01);
                }
            }//namespace

		    template<typename T>
		    void VectorTrace<T>::Update()
		    {
			    if(Coord==Target)
			    {
				    SafeCallEvent(OnToEnd,(this));

        		    return;
			    }

			    T v;     		//临时变量
			    float distance; //临时变量
			    float gap_time=time->LastGapTime;

			    //更新当前位置
			    Coord+=Speed*gap_time;

			    //计算距离
			    distance=Target.Length(Coord);

			    if(distance<MinDistance)
			    {
				    //太近，到达目得地了
				    Coord=Target;

        		    SafeCallEvent(OnToEnd,(this));

				    return;
			    }

			    if(distance < MaxSpeedDistance)
			    {
				    v=Target-Coord;

				    //执行跟踪算法
				    v*=TraceRate/(distance+1);

				    //每次增加一个单位向量（指向目标）
				    Speed+=v/gap_time;

				    //增加一点干扰，更具真实性
				    if((FastRand()&0x77777777)==0)
                        SpeedAddRand<T>(Speed);

				    distance = Speed.Length(0,0);

				    //跟踪速度达到最大跟踪速度
				    if(distance > MaxSpeed)
					    Speed*=DecSpeed;		//减速25%
			    }
			    else
			    {
				    //超过最大距离，强制复位和跟踪因子
				    Coord=Target;

				    TraceRate = FastRand(2,9);
			    }
		    }
	    }//namespace affect
    }//namespace algorithm
}//namespace hgl
