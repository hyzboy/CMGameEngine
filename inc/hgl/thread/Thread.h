#ifndef HGL_THREAD_INCLUDE
#define HGL_THREAD_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/type/Set.h>
#include<hgl/type/BaseString.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    void WaitThreadExit(thread_ptr tp,const double &time_out);

	/**
	* 线程类.本类用于建立一个线程并管理它。<br>
	* Sleep和Exit函数是给线程内的程序使用的；而Start,Close,Pause,Rewind是给线程外的程序使用的。<br>
	* 使用本类只需要重载本类，并将代码写在Execute函数内即可。<br>
	* 执行大概为：<br>
	* if(ProcStartThread())		<br>
	* 	while(Execute());	<br>
	* else					<br>
	* 	return(0);
	* 						<br>
	* ProcEndThread();
	*/
	class Thread																					///线程类
	{
		friend void WaitThread(Thread **,int,double);

	private:

		thread_ptr tp=0;

        ThreadMutex live_lock;
        ThreadMutex exit_lock;

        friend THREAD_FUNC ThreadFunc(Thread *tc);

#ifdef _DEBUG
        UTF8String thread_addr_string;                                                              ///<线程地址用字符串，调试使用
#endif//_DEBUG

	public:

		virtual ~Thread()=default;

#ifdef _DEBUG
		const UTF8String &GetThreadAddressString()const{return thread_addr_string;}               ///<取得线程地址字符串
#endif//_DEBUG

		/**
		* 线程执行函数，会被反复调用
		* @return true 继续执行
		* @return false 退出执行
		*/
		virtual bool Execute()=0;                                                                   ///<线程的运行函数

		virtual bool ProcStartThread()                                                                ///<线程启动运行函数,在Execute前被调用
                {
#ifdef _DEBUG
                    char thread_addr[(sizeof(thread_ptr)<<1)+1];

                    DataToUpperHexStr(thread_addr,(uint8 *)&tp,sizeof(thread_ptr));

                    thread_addr_string=thread_addr;
#endif//_DEBUG

                    return(true);
                }

		virtual void ProcEndThread(){}														          ///<结程结束运行函数,在Execute后被调用

		virtual bool IsExitDelete()const{return true;}												///<返回在退出线程时，是否删除本对象(注:此函数不可动态变动值)

                bool IsLive()                                                                       ///<当前线程是否还活着
                {
                    if(live_lock.TryLock())
                    {
                        //成功了，证明这个线程已经关闭了
                        live_lock.Unlock();
                        return(false);
                    }

                    return(true);
                }

	public:	//线程运行控制

		virtual bool Start();																		///<开始运行当前线程

		virtual bool IsCurThread();																	///<是否是当前线程

        virtual void Exit()                                                                         ///<退出当前线程(其它线程调用)
        {
            exit_lock.Unlock();
        }

        /**
         * (线程外部调用)退出当前线程，并等待其完成退出<br>
         * @param time_out 等待的时间，如果为0表示等到线程运行结束为止。默认为0
         */
		virtual void WaitExit(const double &time_out=0)
        {
            if(!tp)
                return;

            thread_ptr self_tp=tp;

            Exit();

            WaitThreadExit(self_tp,time_out);
        }
	};//class Thread

	void WaitThread(Thread **,int,double time=0);													///<等待多个线程中的一个完成

	bool CreateThread(Thread *);																	///<创建一个线程

    /**
     * 简单的多线程管理
     */
    template<typename THREAD> class MultiThreadManage
    {
    protected:

        Set<THREAD *> thread_set;

    public:

        virtual ~MultiThreadManage()
        {
            Close();
        }

        THREAD *GetThread(int index)
        {
            THREAD *result;

            if(thread_set.Get(index,result))
                return result;

            return nullptr;
        }

        /**
         * 增加一个线程到合集中
         * @return 是否增加成功
         */
        bool Add(THREAD *p)
        {
            if(!p)return(false);

            return(thread_set.Add(p)!=-1);
        }

        /**
         * 删除一个线程
         */
        void Delete(THREAD *p)
        {
            if(!p)return;

            thread_set.Delete(p);
        }

        /**
         * 关闭所有线程
         */
        void Close()
        {
            const int count=thread_set.GetCount();

            THREAD **p=thread_set.GetData();

            for(int i=0;i<count;i++)
            {
                (*p)->WaitExit();
                ++p;
            }

            thread_set.ClearData();
        }

        /**
         * 启动所有线程
         * @return 成功启动的线程数量
         */
        int Start()
        {
            const int count=thread_set.GetCount();

            THREAD **p=thread_set.GetData();

            int total=0;

            for(int i=0;i<count;i++)
            {
                if((*p)->Start())
                    ++total;

                ++p;
            }

            return total;
        }

        /**
         * 检查还有几个线程活着
         */
        int IsLive()
        {
            const int count=thread_set.GetCount();

            THREAD **p=thread_set.GetData();

            int total=0;

            for(int i=0;i<count;i++)
            {
                if((*p)->IsLive())
                    ++total;

                ++p;
            }

            return total;
        }
    };//class MultiThreadManage
}//namespace hgl
#endif//HGL_THREAD_INCLUDE
