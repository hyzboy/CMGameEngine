#ifndef HGL_NETWORK_MULTI_THREAD_TCP_SERVER_INCLUDE
#define HGL_NETWORK_MULTI_THREAD_TCP_SERVER_INCLUDE

#include<hgl/network/TCPAccept.h>
#include<hgl/network/TCPServer.h>
#include<hgl/network/MultiThreadAccept.h>
#include<hgl/network/SocketManageThread.h>
#include<hgl/Time.h>

namespace hgl
{
    namespace network
    {
        /**
         * 多线程TCP服务器
         */
        template<typename USER_ACCEPT,typename SOCKET_MANAGE_THREAD> class MTTCPServer
        {
        protected:

            class Accept2SocketManageThread:public AcceptThread
            {
                SOCKET_MANAGE_THREAD *sm_thread;

            public:

                using AcceptThread::AcceptThread;

                void SetSocketManage(SOCKET_MANAGE_THREAD *smt)
                {
                    sm_thread=smt;
                }

                bool OnAccept(int client_sock,IPAddress *ip_address) override
                {
                    if(!sm_thread)return(false);

                    USER_ACCEPT *us;

                    us=new USER_ACCEPT(client_sock,ip_address);      //这个new非常占时间，未来放到各自的线程去做

                    auto &sl=sm_thread->JoinBegin();

                    sl.Add(us);

                    sm_thread->JoinEnd();

                    return(true);
                }
            };//class Accept2SocketManageThread:public AcceptThread

        protected:

            IPAddress *                                     server_ip=nullptr;
            TCPServer                                       server;

            MultiThreadAccept<Accept2SocketManageThread>    accept_manage;
            MultiThreadManage<SOCKET_MANAGE_THREAD>         sock_manage;

        protected:

            virtual SOCKET_MANAGE_THREAD *CreateSocketManageThread(int max_user)
            {
                SocketManage *sm=new SocketManage(max_user);
                SOCKET_MANAGE_THREAD *smt=new SOCKET_MANAGE_THREAD(sm);

                return smt;
            }

        public:

            /**
            * 服务器初始化信息结构
            */
            struct InitInfomation
            {
                IPAddress * server_ip           =nullptr;               ///<服务器IP地址
                bool        port_reuse          =false;                 ///<端口复用
                bool        ipv6_only           =false;                 ///<是否使用IPv6 Only
                bool        block               =true;                  ///<是否使用阻塞模式
                uint        defer_accept_time   =1;                     ///<延迟Accept成功超时时间,单位:秒(第一次收到数据才会成功Accept，这里指从connect到第一个包的超时时间)
                double      accept_time_out     =1;                     ///<Accept超时时间,单位:秒

                uint        max_user            =1024;                  ///<最大用户数量
                uint        thread_count        =4;                     ///<线程数量
            };//struct MTTCPServerInitInfomation

            bool Init(InitInfomation &info)
            {
                if(!info.server_ip)return(false);
                if(info.max_user<=0)return(false);
                if(info.thread_count<=0)return(false);

                if(!server.CreateServer(info.server_ip,info.max_user,info.port_reuse))
                    return(false);

                if(info.server_ip->GetFamily()==AF_INET6)               //如果是IPv6地址
                    server.SetIPv6Only(info.ipv6_only);                 //设置是否仅使用IPv6,这个有可能失败，但是不管它

                server.SetBlock(true);                                  //设置使用阻塞模式

#if HGL_OS != HGL_OS_Windows
                server.SetDeferAccept(info.defer_accept_time);          //指定时间内收到数据才会产生accept
#endif

                server.SetTimeOut(info.accept_time_out);                //设置accept超时时间

                if(!accept_manage.Init(&server,info.thread_count))
                    return(false);

                for(int i=0;i<info.thread_count;i++)
                {
                    Accept2SocketManageThread *at=accept_manage.GetAcceptThread(i);

                    SOCKET_MANAGE_THREAD *smt=CreateSocketManageThread(info.max_user);

                    at->SetSocketManage(smt);

                    sock_manage.Add(smt);
                }

                if(!sock_manage.Start())
                    return(false);

                if(!accept_manage.Start())
                {
                    sock_manage.Close();
                    return(false);
                }

                server_ip=info.server_ip;

                return(true);
            }

            int IsLive()
            {
                return(sock_manage.IsLive()+accept_manage.IsLive());
            }

            bool Wait(const double &time_out=HGL_NETWORK_TIME_OUT)
            {
                WaitTime(time_out);

                const int live_s=sock_manage.IsLive();
                const int live_a=accept_manage.IsLive();

                if(live_s+live_s<=0)
                    return(false);

#ifdef _DEBUG
                std::cout<<"live accept thread "<<live_a<<", sock thread: "<<live_s<<std::endl;
#endif//_DEBUG

                return(true);
            }
        };//template<typename USER_ACCEPT,typename SOCKET_MANAGE_THREAD> class MTTCPServer

        template<typename USER_ACCEPT>
        using MTTCPServerStd=MTTCPServer<USER_ACCEPT,SocketManageThread<USER_ACCEPT>>;
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_MULTI_THREAD_TCP_SERVER_INCLUDE
