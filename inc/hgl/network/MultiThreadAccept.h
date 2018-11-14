#ifndef HGL_NETWORK_MULTI_THREAD_ACCEPT_INCLUDE
#define HGL_NETWORK_MULTI_THREAD_ACCEPT_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/type/Stack.h>
#include<hgl/thread/Semaphore.h>
namespace hgl
{
    namespace network
    {
        class IPAddress;
        class AcceptServer;

        using IPAddressStack=Stack<IPAddress *>;

        /**
         * Socket接入线程
         */
        class AcceptThread:public Thread
        {
            AcceptServer *server;
            Semaphore *active_semaphore;

        protected:

            IPAddressStack ip_stack;                                            ///<IP地址空间堆栈，会提前分配好空间供未来使用

        public:

            AcceptThread(AcceptServer *,Semaphore *);
            virtual ~AcceptThread()=default;

            bool Execute() override;

        public:

            /**
             * 接受一个接入
             * @param sock Socket值
             * @param addr ip地址(注:需自行delete)
             */
            virtual bool OnAccept(int sock,IPAddress *addr)=0;                  ///<接入一个新Socket
        };//class AcceptThread:public Thread

        /**
         * 多Socket接入线程管理模板
         */
        template<typename ACCEPT_THREAD> class MultiThreadAccept
        {
            MultiThreadManage<ACCEPT_THREAD> accept_thread_manage;

            Semaphore active_semaphore;

        public:

            virtual ~MultiThreadAccept()=default;

            bool Init(AcceptServer *server,const uint thread_count)
            {
                if(!server)return(false);
                if(thread_count<=0)return(false);

                for(int i=0;i<thread_count;i++)
                    accept_thread_manage.Add(new ACCEPT_THREAD(server,&active_semaphore));

                return(true);
            }

            ACCEPT_THREAD *GetAcceptThread(const int index)
            {
                return accept_thread_manage.GetThread(index);
            }

            bool Start  (){return accept_thread_manage.Start()>0;}
             int IsLive (){return accept_thread_manage.IsLive();}
            bool Close  (){return accept_thread_manage.Close();}

            /**
             * 等待接收信号
             */
            bool Wait(const double &time_over=5)
            {
                return active_semaphore.Acquire(time_over);
            }
        };//template<typename ACCEPT_THREAD> class MultiThreadAccept
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_MULTI_THREAD_ACCEPT_INCLUDE
