#ifndef HGL_ALGORITHM_AFFECTOR_FADE_OBJECT_INCLUDE
#define HGL_ALGORITHM_AFFECTOR_FADE_OBJECT_INCLUDE

#include<hgl/algorithm/affect/AffectorObject.h>
#include<hgl/algorithm/affect/Timer.h>
namespace hgl
{
    namespace algorithm
    {
	    namespace affect
	    {
		    class TrackTimer;

		    /*
		    * 淡入影响器(从一个值过渡到另一个值)
		    */
		    template<typename T> class Fade:public AffectorObject										///淡入淡出影响器
		    {
			    UTF8String name;

			    TrackTimer *track;

			    T start;
			    T end;
			    T gap;

			    T cur;

		    private:

			    T GetStart(){return start;}
			    T GetEnd(){return end;}
			    T GetCur(){return cur;}

			    void SetStart(T a)
			    {
				    cur=start=a;

				    gap=end-start;
			    }

			    void SetEnd(T a)
			    {
				    end=a;

				    gap=end-start;
			    }

		    public:	//属性

			    Property<T> StartValue;																	///<起始值
			    Property<T> EndValue;																	///<结束值
			    Property<T> CurValue;																	///<当前值

		    public:

			    Fade(const UTF8String &n,TrackTimer *tt)
			    {
				    name=n;
				    track=tt;/*

				    start=0;
				    end=0;
				    gap=0;
				    cur=0;*/

				    hglSetProperty(		StartValue,	this,Fade<T>::GetStart,	Fade<T>::SetStart	);
				    hglSetProperty(		EndValue,	this,Fade<T>::GetEnd,	Fade<T>::SetEnd	);
				    hglSetPropertyRead(	CurValue,	this,Fade<T>::GetCur);
			    }

			    virtual ~Fade()=default;

			    const char *GetClassName(){return name.c_str();}									    ///<返回类名

			    void Update()
			    {
				    if(!track)return;

				    cur=start+gap*(track->Position);
			    }
		    };//class Fade
	    }//namespace affect
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_AFFECTOR_FADE_OBJECT_INCLUDE
