#ifndef HGL__FLOW_OBJECT_INCLUDE
#define HGL__FLOW_OBJECT_INCLUDE

#include<hgl/platform/Platform.h>
#include<hgl/object/FlowObjectState.h>
namespace hgl
{
    /**
	* 流程对象是指可以放在流程控制器里的一种特殊对象
	*/
    template<typename T> class _FlowObject                                                          ///流程对象基类
	{
	protected:

        T *NextObject;                                                                              ///<下一个对象

        FlowObjectState fos;                                                                        ///<当前对象状态

    public:  //事件

        virtual void OnToBack(){}                                                                   ///<将当前流程切入后台
        virtual void OnResume(){}                                                                   ///<将当前流恢复到前台
        virtual void OnDestroy(){/*重载此处理本对象销毁事件*/}                                          ///<当前对象销毁事件

	public: //方法

        _FlowObject()                                                                               ///<本类构造函数
        {
            fos=fosNotInit;

            NextObject=nullptr;
        }

        /**
        * 流程对像基类析构函数<br>
        * NextObject如果没有被清除，在这里会被清除
        */
        virtual ~_FlowObject()                                                                      ///<本类析构函数
        {
            SAFE_CLEAR(NextObject);
        }

        const FlowObjectState GetState()const{return fos;}                                          ///<取得当前对象状态

        /**
         * 结束当前流程并进入下一个流程
         * @param next_obj 下一个流程对象
         */
        virtual void Exit(T *next_obj,FlowObjectState os)
        {
            if(NextObject)                      //有一个没用，，，奇怪的错误
                NextObject->OnDestroy();

            NextObject=next_obj;
            fos=os;
        }

        /**
        * 取得下一个对象。如果不重载此函数，它将返回当前对象中的NextObject变量
        */
        virtual T *GetNextObject()
        {
            if(NextObject)
            {
                T *result=NextObject;

                NextObject=nullptr;

                return(result);
            }

            return(nullptr);
        }

        /**
         * 流程返回事件
         * @param prev_fo 之前执行的对象
         */
        virtual void ObjectReturn(T *prev_fo)                                                       ///<对象返回函数
        {
            OnResume();
            fos=fosOK;
            NextObject=nullptr;
        }
	};//template<typename T> class _FlowObject
}//namespace hgl
#endif//HGL__FLOW_OBJECT_INCLUDE
