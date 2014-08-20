#ifndef HGL_TIMER_INCLUDE
#define HGL_TIMER_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/object/EnumObject.h>
namespace hgl
{
	/**
	* 计时器是一个简单封装的组件。通过操作OnTimer事件通知应用程序时间到了！
	* 注意如果间隔时间为0那表示不使用定时器，但这样不如将Enable设为false更为高效。
	*/
	class Timer:public EnumObject                                                                   ///计时器类
	{
	protected:

		double inter;
		double next;

		double GetInterval(){return inter;}
		void SetInterval(double);

	public: //属性

		Property<double> Interval;                                                                  ///<间隔时间虚拟变量,单位秒

	public: //事件

		DefEvent(void,OnTimer,(Object *));															///<时间到了事件

	public:

		Timer(double=0);
		virtual ~Timer();

		virtual void Update();
	};
}
#endif//HGL_TIMER_INCLUDE
