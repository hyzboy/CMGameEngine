#ifndef HGL_NETWORK_MULTI_THREAD_ACCEPT_INCLUDE
#define HGL_NETWORK_MULTI_THREAD_ACCEPT_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/type/Stack.h>
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

        public:

            IPAddressStack ip_stack;

        public:

            AcceptThread(AcceptServer *);
            virtual ~AcceptThread()=default;

            bool Execute() override;

        public:

            virtual bool OnAccept(int,IPAddress *)=0;                           ///<接入一个新Socket
        };//class AcceptThread:public Thread

        /**
         * 多Socket接入线程管理模板
         */
        template<typename ACCEPT_THREAD> class MultiThreadAccept
        {
            MultiThreadManage<ACCEPT_THREAD> accept_thread_manage;

        public:

            virtual ~MultiThreadAccept()=default;

            bool Init(AcceptServer *server,int thread_count)
            {
                if(!server)return(false);
                if(thread_count<=0)return(false);

                for(int i=0;i<thread_count;i++)
                    accept_thread_manage.Add(new ACCEPT_THREAD(server));

                return(true);
            }

            ACCEPT_THREAD *GetAcceptThread(const int index)
            {
                return accept_thread_manage.GetThread(index);
            }

            bool Start  (){return accept_thread_manage.Start()>0;}
             int IsLive (){return accept_thread_manage.IsLive();}
            bool Wait   (){return accept_thread_manage.Wait();}
            bool Close  (){return accept_thread_manage.Close();}
        };//template<typename ACCEPT_THREAD> class MultiThreadAccept
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_MULTI_THREAD_ACCEPT_INCLUDE
