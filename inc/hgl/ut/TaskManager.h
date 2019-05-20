#ifndef HGL_TASK_MANAGER_INCLUDE
#define HGL_TASK_MANAGER_INCLUDE

#include<hgl/type/List.h>
namespace hgl
{
    namespace util
    {
        class TaskManager;

        typedef DefEvent(bool,TASK_UPDATE_FUNC,(TaskManager *));

        #define HGL_TASK_ADD(tm,class_pointer,class_name,func)	{	\
                                                                    TASK_UPDATE_FUNC tuf;	\
                                                                    \
                                                                    SetEventCall(tuf,class_pointer,class_name,func);	\
                                                                    \
                                                                    tm->Add(tuf);	\
                                                                }

        /**
        * 任务管理器是为了解决在一个大的任务流程中多个小的任务流程所使用的对象<br>
        * 它就像是一个轻便小型的流程控制器，没有什么复杂的设计。
        */
        class TaskManager:public List<TASK_UPDATE_FUNC>													///任务管理器
        {
        protected:

            bool enabled;																				///<是否启用
            int cur;                                                                                    ///<当前任务

        protected:

            bool GetEnabled(){return enabled;}
            void SetEnabled(bool e){enabled=e;}

            int GetCurTask()const{return(cur);}               											///<取得当前任务用函数
            void SetCurTask(int index){Goto(index);}													///<设置当前任务用函数按下

            void ChangeTask(int);																		///<切换到指定任务

        public:	//属性

            Property<bool>	Enabled;																	///<当前任务管理器状态
            Property<int>	CurTask;																	///<当前任务虚拟函数

        public:	//事件

            DefEvent(void,OnChange,(TaskManager *,int,int));											///<更改任务事件函数
            DefEvent(void,OnEnd,(TaskManager *));														///<任务运行结束事件函数

        public:

            TaskManager();                                                                              ///<本类构造函数
            virtual ~TaskManager();                                                                     ///<本类析构函数

            virtual bool Start();																		///<开始运行
            virtual void Pause();																		///<暂停运行
            virtual void Resume();																		///<恢复运行
            virtual void Stop();																		///<停止运行

            virtual bool Prev();																		///<跳到前一任务
            virtual bool Next();																		///<跳到下一任务
            virtual bool Goto(int);																		///<跳到指定任务

            virtual bool Update();																		///<刷新并执行任务
        };//class TaskManager
    }//namespace util
}//namespace hgl
#endif//HGL_TASK_MANAGER_INCLUDE
