#ifndef HGL_ALGORITHM_AFFECTOR_THRUST_INCLUDE
#define HGL_ALGORITHM_AFFECTOR_THRUST_INCLUDE

#include<hgl/algorithm/affect/AffectorObject.h>
#include<hgl/algorithm/VectorMath.h>
namespace hgl
{
    namespace algorithm
    {
        namespace affect
        {
            class Timer;

            /**
            * 推力影响器<br>
            * 一种提供给游戏用的推力模拟影响器，注意它并不符合物理学。
            */
            template<typename T>
            class Thrust:public AffectorObject                                                          ///推力影响器
            {
                Timer *time;                                                                            ///<时间记录器

                float power;                                                                            ///<力量
                float dec_power;                                                                        ///<力量衰减幅度

                T dir;                                                                                  ///<力量方向

                T dir_power;                                                                            ///<方向推力(结果)

            private:

                float GetPower(){return power;}
                float GetDecPower(){return dec_power;}
                T GetDir(){return dir;}
                T GetDirPower(){return dir_power;}

                void SetPower(float p){power=p;}
                void SetDecPower(float p){dec_power=p;}
                void SetDir(const T &v){dir=v;}

            public:

                Property<float> Power;                                                                  ///<力量
                Property<float> DecPower;                                                               ///<力量衰减幅度(每秒)
                Property<T> Direction;                                                                  ///<推力方向

                Property<T> DirectionPower;                                                             ///<方向推力(结果,只读)

            public:

                /**
                * 推力影响器构造函数
                * @param t 时间记录器
                * @param v 推力方向
                * @param ss 起始力量
                * @param ds 每秒力量衰减幅度
                */
                Thrust::Thrust(Timer *t,const T &v,double ss=1.0f,double ds=0.1f)
                {
                    time=t;

                    power=ss;
                    dec_power=ds;

                    dir=v;

                    hglSetProperty(Power,       this,Thrust::GetPower,      Thrust::SetPower    );
                    hglSetProperty(DecPower,    this,Thrust::GetDecPower,   Thrust::SetDecPower );
                    hglSetProperty(Direction,   this,Thrust::GetDir,        Thrust::SetDir      );

                    hglSetPropertyRead(DirectionPower,this,Thrust2D::GetDirPower);
                }

                virtual ~Thrust()=default;

                virtual const char *GetClassName(){return U8_TEXT("Thrust");}                           ///<取得类名

                virtual void Update()
                {
                    if(!time)return;
                    if(power<=0)return;

                    double gap=time->LastGapTime;

                    dir_power=dir*(power*gap);          //计算当前时间所取得的推力

                    power-=power*(dec_power*gap);       //力量衰减
                }
            };//class Thrust

            using Thrust1f=Thrust<Vector1f>;
            using Thrust2f=Thrust<Vector2f>;
            using Thrust3f=Thrust<Vector3f>;
        }//namespace affect
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_AFFECTOR_THRUST_INCLUDE
