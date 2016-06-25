#ifndef HGL_FLOW_OBJECT_INCLUDE
#define HGL_FLOW_OBJECT_INCLUDE

#include<hgl/platform/BaseApplication.h>
#include<hgl/object/EventObject.h>
#include<hgl/gui/ObjectControl.h>
namespace hgl
{
	/**
	* 作业流程对象，负责提供刷新功能和绘制功能。供流程控制器进行调用，以完成整个作业。<br>
	* 需要注意的是：画面绘制和刷新是分离的，刷新是在不停的运行的，而画面绘制只在时间到达时才会被调用。
	*/
	class FlowObject:public EventObject                                         	                ///作业流程对象基类
	{
		friend class FlowControl;

		#include<hgl/object/Object.ProcEvent.h>
		#include<hgl/object/Object.OnEvent.h>

	protected:

		FlowObject *NextObject;                                                                     ///<下一个对象

		FlowObjectState fos;                                                                        ///<当前对象状态

		FlowObjectState GetState(){return fos;}

		void Proc_Resize(int,int);																	///<处事重设尺寸事件

	public: //属性

		bool KeyPreview;                                                                            ///<键盘事件预览(为ture时键盘事件优先自己处理，为false时优先传给Control中的控件。默认为false)

		ObjectControl Control;                                                                      ///<对象控制器

		Property<FlowObjectState> ObjectState;                                                      ///<当前对象状态虚拟变量

	public:	//事件

		DefEvent(void,OnResize,(int,int));															///<屏幕尺寸调整
		DefEvent(void,OnDestroy,(FlowObject *));													///<销毁事件
		DefEvent(void,OnFlowReturn,(FlowObject *));													///<流程返回事件

	public: //方法

		FlowObject();                                                                               ///<本类构造函数
		virtual ~FlowObject();                                                                    	///<本类析构函数

		virtual FlowObject *GetNextObject();                                                        ///<取得下一个要执行的流程对象
		virtual void ObjectReturn(FlowObject *);                                                  	///<对象返回函数

		virtual void Update(){}													                    ///<刷新函数
		virtual void UpdateObject()
		{
			Update();

			if(Control.Enabled)
				Control.Update();
		}

		virtual void Draw(){}																		///<画面绘制
		virtual void LastlyDraw(){}																	///<最终画面绘制(当对象都绘制完成后会被调用)
		virtual void DrawObject(const Matrix4f *mv)
		{
			Draw();

			if(Control.Visible)
				Control.DrawObject(mv);

			LastlyDraw();
		}
	};//class FlowObject
}//namespace hgl
#endif//HGL_FLOW_OBJECT_INCLUDE
