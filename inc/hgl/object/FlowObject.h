#ifndef HGL_FLOW_OBJECT_INCLUDE
#define HGL_FLOW_OBJECT_INCLUDE

#include<hgl/object/_FlowObject.h>
namespace hgl
{
	/**
	* 作业流程对象，负责提供刷新功能和绘制功能。供流程控制器进行调用，以完成整个作业。<br>
	* 需要注意的是：画面绘制和刷新是分离的，刷新是在不停的运行的，而画面绘制只在时间到达时才会被调用。
	*/
    class FlowObject:public _FlowObject<FlowObject>                                                 ///作业流程对象基类
	{
	protected:

        virtual void OnResize(int,int){}                                                            ///<画布尺寸调整事件
        virtual void OnRotate(int){}                                                                ///<屏幕旋转事件

	public: //方法

        using _FlowObject<FlowObject>::_FlowObject;                                                 ///<本类构造函数
		virtual ~FlowObject(){}                                                                    	///<本类析构函数

		virtual void Update(){}																		///<刷新函数
        virtual void Draw(){}                                                                       ///<画面绘制
	};//class FlowObject
}//namespace hgl
#endif//HGL_FLOW_OBJECT_INCLUDE
