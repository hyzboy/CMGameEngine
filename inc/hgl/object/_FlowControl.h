#ifndef HGL__FLOW_CONTROL_INCLUDE
#define HGL__FLOW_CONTROL_INCLUDE

#include<hgl/type/Stack.h>
#include<hgl/object/_FlowObject.h>
namespace hgl
{
    /**
    * _FlowControl是流程控制器的实现模板,它的作用是对一串_FlowObject模板派生对象的流程进行控制。
    */
    template<typename T> class _FlowControl                                                         ///流程对象控制类
    {
        FlowObjectState fos=fosNotInit;                                                             ///<当前对象状态

    protected:

        T *active_obj;

        Stack<T *> objstack;

    protected:

        virtual void DestroyCurrentObject()
        {
            active_obj->OnDestroy();
            delete active_obj;
            active_obj=nullptr;
        }

        virtual void OnExitNotClear()
        {
            active_obj->OnToBack();

            objstack.Push(active_obj);

            ChangeActiveObject(active_obj->GetNextObject());
        }

        virtual void OnExitToBack()
        {
            objstack.Push(active_obj);

            ChangeActiveObject(active_obj->GetNextObject());
        }

        virtual void OnReturn()
        {
            T *ulobj=nullptr;

            objstack.Pop(ulobj);

            if(!ulobj)
            {
                DestroyCurrentObject();
                ChangeActiveObject(nullptr);

                return;
            }

            ulobj->ObjectReturn(active_obj);
            DestroyCurrentObject();
            ChangeActiveObject(ulobj);
        }

        virtual void OnExit()
        {
            T *next=active_obj->GetNextObject();

            if(next==nullptr)
            {
                T *ulobj=nullptr;

                objstack.Pop(ulobj);

                if(ulobj)
                {
                    ulobj->ObjectReturn(active_obj);
                    ChangeActiveObject(ulobj);
                }
                else
                {
                    DestroyCurrentObject();
                    ChangeActiveObject(nullptr);
                }

                fos=fosExitApp;
            }
            else
            {
                DestroyCurrentObject();
                ChangeActiveObject(next);
            }
        }

        virtual void OnExitFlow()
        {
            T *next=active_obj->GetNextObject();

            Clear();			//清空所有流程

            if(next==nullptr)
            {
                active_obj=nullptr;

                fos=fosExitApp;
            }
            else
            {
                ChangeActiveObject(next);
            }
        }

        virtual void OnExitApp()
        {
            Clear();

            fos=fosExitApp;
        }

        void ProcState(FlowObjectState state)
        {
            if(state==fosOK)return;

            if(state==fosExitNotClear   ){OnExitNotClear();return;}
            if(state==fosExitToBack     ){OnExitToBack();return;}
            if(state==fosReturn         ){OnReturn();return;}
            if(state==fosExit           ){OnExit();return;}
            if(state==fosExitFlow       ){OnExitFlow();return;}
            if(state==fosExitApp        ){OnExitApp();return;}
        }

        virtual void ChangeActiveObject(T *obj)
        {
            active_obj=obj;

            OnChange(obj);
        }

    public: //属性

        const FlowObjectState GetState()const{return fos;}                                          ///<取得当前对象状态

        int GetStackCount()const{return objstack.GetCount();}                                       ///<取得当前堆栈数量

        T *GetActiveObject(){return active_obj;}                                                    ///<取得当前活动流程对象

    public: //事件

        virtual void OnChange(T *){}                                                                ///<当前流程活动对像改变事件

    public: //方法

        _FlowControl(){active_obj=nullptr;}                                                         ///<本类构造函数

        /**
        * 本类构造函数
        * @param obj 本流程控制器的起始流程
        */
        _FlowControl(T *obj){active_obj=obj;}                                                       ///<本类构造函数
        virtual ~_FlowControl(){Clear();}                                                           ///<本类析构函数

        /**
        * 设置起始流程对象
        */
        virtual void SetStart(T *obj)                                                               ///<设定起始流程对象
        {
            Clear();

            ChangeActiveObject(obj);
        }

        /**
        * 清除当前控制器中的所有对象
        */
        virtual void Clear()                                                                        ///<清除本控制器中的对象
        {
            if(active_obj)
            {
                T *fo=active_obj;

                ChangeActiveObject(nullptr);

                delete fo;
            }

            T *obj;

            while(true)
            {
                if(objstack.Pop(obj))
                    delete obj;
                else
                    break;
            }
        }

        /**
        * 退出当前流程对象，并进入下一个流程对象，如未指定下一个流程对象，则退出当前流程
        * @param next_obj 下一个流程对象
        */
        virtual void Exit(T *next_obj=nullptr)                                                      ///<退出当前流程对象并进入下一个流程对象
        {
            if(!active_obj)return;

            active_obj->Exit(next_obj,fosExit);
        }

        /**
        * 退出当前流程对象，放入后台(继续执行)，并进入下一个流程对象
        * @param next_obj 下一个流程对象
        */
        virtual void ExitToBack(T *next_obj)                                                        ///<退出当前流程对象到后台
        {
            if(!active_obj)return;

            active_obj->Exit(next_obj,fosExitToBack);
        }

        /**
        * 退出当前流程对象(但不清除)，并暂停当前流程对象
        * @param next_obj 下一个流程对象
        */
        virtual void ExitNotClear(T *next_obj)                                                      ///<退出并进入下一个流程对象
        {
            #ifdef _DEBUG
            if(!next_obj)
            {
                LOG_ERROR(OS_TEXT("错误！呼叫进入的子流程对象不存在！"));
                return;
            }
            #endif//_DEBUG

            if(!active_obj)return;

            active_obj->Exit(next_obj,fosExitNotClear);
        }

        /**
        * 退出整个应用程序
        */
        virtual void ExitApp()                                                                      ///<退出整个游戏
        {
            if(!active_obj)return;

            active_obj->Exit(nullptr,fosExitApp);
        }

        /**
        * 退出子流程对象，返回上一级流程对象
        */
        virtual void Return()                                                                       ///<退出当前流程对象，返回上一级流程对象
        {
            if(!active_obj)return;

            active_obj->Exit(nullptr,fosReturn);
        }

        /**
        * 进入下一个流程对象，并清空当前整个流程
        * @param next_obj 下一个流程对象
        */
        virtual void ClearTo(T *next_obj)                                                           ///<清空当前所有流程对象，进入指定流程对象
        {
            #ifdef _DEBUG
            if(!next_obj)
            {
                LOG_ERROR(OS_TEXT("错误！呼叫进入的子流程对象不存在！"));
                return;
            }
            #endif//_DEBUG

            if(!active_obj)return;

            active_obj->Exit(next_obj,fosExitFlow);
        }

        virtual void Update()                                                                       ///<刷新当前流程控制器
        {
            if(!active_obj)
            {
                fos=fosExitApp;
                return;
            }

            if(!active_obj->CanUpdate())return;

            {
                int n,max=objstack.GetCount();

                for(n=0;n<max;n++)
                {
                    T *obj=objstack[n];

                    if(obj->CanUpdate())
                        obj->Update();
                }
            }

            active_obj->Update();
        }

        virtual void ProcCurState()                                                                 ///<处理当前流程
        {
            if(!active_obj)
            {
                fos=fosExitApp;
                return;
            }

            if(!active_obj->CanUpdate())return;

            ProcState(active_obj->GetState());
        }
    };//class _FlowControl
}//namespace hgl
#endif//HGL__FLOW_CONTROL_INCLUDE
