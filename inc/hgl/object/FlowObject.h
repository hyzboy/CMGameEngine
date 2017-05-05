#ifndef HGL_FLOW_OBJECT_INCLUDE
#define HGL_FLOW_OBJECT_INCLUDE

#include<hgl/object/_FlowObject.h>
namespace hgl
{
    class FlowControl;

	/**
	* 作业流程对象，负责提供刷新功能和绘制功能。供流程控制器进行调用，以完成整个作业。<br>
	* 需要注意的是：画面绘制和刷新是分离的，刷新是在不停的运行的，而画面绘制只在时间到达时才会被调用。
	*/
    class FlowObject:public _FlowObject<FlowObject>                                                 ///作业流程对象基类
	{
        bool is_back=false;
        bool is_draw=true;
                
        FlowControl *control=nullptr;

    protected:

        friend class FlowControl;

        virtual void JoinControl(FlowControl *fc){control=fc;}
        virtual void UnjoinControl(FlowControl *){control=nullptr;}

    public: //输入事件

        virtual bool OnMouseMove        (int,int){}
        virtual bool OnMouseLeftDown    (int,int){}
        virtual bool OnMouseLeftUp      (int,int){}
        virtual bool OnMouseMidDown     (int,int){}
        virtual bool OnMouseMidUp       (int,int){}
        virtual bool OnMouseRightDown   (int,int){}
        virtual bool OnMouseRightUp     (int,int){}
        virtual bool OnMouseWheel       (int){}
        virtual bool OnKeyDown          (int){}
        virtual bool OnKeyUp            (int){}
        virtual bool OnKeyRepeat        (int){}          //自动重发
        virtual bool OnChar             (os_char){}

	public: //方法

        using _FlowObject<FlowObject>::_FlowObject;                                                 ///<本类构造函数
		virtual ~FlowObject(){}                                                                    	///<本类析构函数

        virtual void OnResize(int,int){}                                                            ///<画布尺寸调整事件
        virtual void OnRotate(int){}                                                                ///<屏幕旋转事件

        virtual void OnToBack()override{is_back=true;}
        virtual void OnResume()override{is_back=false;is_draw=true;}

        virtual bool CanUpdate(){return !is_back;}                                                  ///<是否可以刷新
        virtual bool CanDraw(){return is_draw;}                                                     ///<是否可以绘制函数

		virtual void Update(){}																		///<刷新函数
        virtual void Draw(const Matrix4f *)=0;                                                      ///<画面绘制
	};//class FlowObject
}//namespace hgl
#endif//HGL_FLOW_OBJECT_INCLUDE
