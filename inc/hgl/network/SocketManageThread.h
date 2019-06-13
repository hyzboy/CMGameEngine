#ifndef HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE
#define HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE

#include<hgl/network/SocketManage.h>
#include<hgl/thread/Thread.h>
#include<hgl/thread/SwapData.h>
namespace hgl
{
    namespace network
    {
        /**
         * 简单的Socket管理器线程
         */
        template<typename USER_ACCEPT> class SocketManageThread:public Thread
        {
        public:

            using AcceptSocketList=List<USER_ACCEPT *>;                           ///<工作对象列表定义

        protected:

            SocketManage *sock_manage;

        protected:

            SemSwapData<AcceptSocketList> join_list;                            ///<待添加的Socket对象列表
            SemSwapData<AcceptSocketList> unjoin_list;                          ///<待移出的Socket对象列表

            virtual void OnSocketClear(USER_ACCEPT *us){delete us;}             ///<Socket清理事件
            virtual void OnSocketError(USER_ACCEPT *us){OnSocketClear(us);}     ///<Socket出错处理事件

            template<typename ST>
            void ClearAcceptSocketList(ST &sl)
            {
                const int count=sl.GetCount();
                USER_ACCEPT **us=sl.GetData();

                for(int i=0;i<count;i++)
                {
                    OnSocketClear(*us);
                    ++us;
                }

                sl.ClearData();
            }

            virtual bool Join(USER_ACCEPT *us){return sock_manage->Join(us);}     ///<单个工作对象接入处理函数
            virtual bool Unjoin(USER_ACCEPT *us){return sock_manage->Unjoin(us);} ///<单个工作对象退出处理函数

            /**
             *处理要接入的工作对象列表
             */
            void ProcJoinList()
            {
                AcceptSocketList &usl=join_list.GetReceive();

                const int count=usl.GetCount();
                USER_ACCEPT **us=usl.GetData();

                for(int i=0;i<count;i++)
                {
                    Join(*us);
                    ++us;
                }

                usl.ClearData();
            }

            /**
             * 处理要退出的工作对象列表
             */
            void ProcUnjoinList()
            {
                AcceptSocketList &usl=unjoin_list.GetReceive();

                const int count=usl.GetCount();
                USER_ACCEPT **us=usl.GetData();

                for(int i=0;i<count;i++)
                {
                    Unjoin(*us);
                    ++us;
                }

                usl.ClearData();
            }

        public:

            SocketManageThread(SocketManage *sm)
            {
                sock_manage=sm;
            }

            virtual ~SocketManageThread()
            {
                SAFE_CLEAR(sock_manage);
            }

            virtual void ProcEndThread() override
            {
                ClearAcceptSocketList(join_list.GetReceive());
                join_list.Swap();
                ClearAcceptSocketList(join_list.GetReceive());

                sock_manage->Clear();

                //unjoin_list中的理论上都已经在wo_list/join_list里了，所以不需要走Clear，直接清空列表
                unjoin_list.GetReceive().ClearData();
                unjoin_list.Swap();
                unjoin_list.GetReceive().ClearData();
            }

            virtual bool Execute() override
            {
                if(join_list.TrySemSwap())
                    ProcJoinList();

                if(unjoin_list.TrySemSwap())
                    ProcUnjoinList();

                sock_manage->Update(0.1);         //这里写0.1秒，只是为了不卡住主轮循。这是个错误的设计，未来要将epoll(recv)完全独立一个线程跑

                const auto &error_set=sock_manage->GetErrorSocketSet();
                USER_ACCEPT **us=(USER_ACCEPT **)error_set.GetData();

                for(int i=0;i<error_set.GetCount();i++)
                {
                    OnSocketError(*us);
                    ++us;
                }
                return(true);
            }

        public:

            virtual AcceptSocketList &  JoinBegin(){return join_list.GetPost();}    ///<开始添加要接入的Socket对象
            virtual void                JoinEnd()                                   ///<结束添加要接入的Socket对象
            {
                join_list.ReleasePost();
                join_list.PostSem();
            }

            virtual AcceptSocketList &  UnjoinBegin(){return unjoin_list.GetPost();}///<开始添加要退出的Socket对象
            virtual void                UnjoinEnd()                                 ///<结束添加要退出的Socket对象
            {
                unjoin_list.ReleasePost();
                unjoin_list.PostSem();
            }
        };//template<typename USER_ACCEPT> class SocketManageThread:public Thread
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE
