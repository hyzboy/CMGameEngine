#ifndef HGL_SHADE_NUMBER_INCLUDE
#define HGL_SHADE_NUMBER_INCLUDE

namespace hgl
{
	/**
	* 渐变数分当前值和最终值，当前值会不断向最终值靠拢
	*/
	template<typename T> class ShadeNumber                                                          ///渐变数模板
	{
	public:

		T Current;
		T Target;                                                                                   ///<目标值
		T Step;                                                                                     ///<步进值,指每次渐变的幅度

	public:

		ShadeNumber()
		{
		};

		ShadeNumber(T v,T s)
		{
			Set(v,s);
		}

		void Set(T v,T s)
		{
			Current=v;
			Target=v;
			Step=s;
		}

		void SetCur(T v)
		{
			Current=v;
		}

		void Update()
		{
			if(Current>Target)
			{
				Current-=Step;
				if(Current<Target)
					Current=Target;
			}
			else
			if(Current<Target)
			{
				Current+=Step;
				if(Current>Target)
					Current=Target;
			}
		}

		T GetCur()
		{
			T cur=Current;
			Update();
			return cur;
		}

		operator        T() {return GetCur();}
	};//template<typename T> class ShadeNumber
}//namespace hgl
#endif//HGL_SHADE_NUMBER_INCLUDE
