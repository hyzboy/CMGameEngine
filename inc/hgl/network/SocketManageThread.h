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
        class SocketManageThread:public Thread
        {
        public:

            using AcceptSocketList=List<TCPAccept *>;                           ///<工作对象列表定义

        protected:

            SocketManage *sock_manage;

        protected:

            SemSwapData<AcceptSocketList> join_list;                            ///<待添加的Socket对象列表
            SemSwapData<AcceptSocketList> unjoin_list;                          ///<待移出的Socket对象列表

            virtual void ClearUserSocket(TCPAccept *us)
            {
                delete us;
            }

            template<typename ST>
            void ClearAcceptSocketList(ST &sl)
            {
                const int count=sl.GetCount();
                TCPAccept **us=sl.GetData();

                for(int i=0;i<count;i++)
                {
                    ClearUserSocket(*us);
                    ++us;
                }

                sl.ClearData();
            }

            bool Join(TCPAccept *us){return sock_manage->Join(us);}             ///<单个工作对象接入处理函数
            bool Unjoin(TCPAccept *us){return sock_manage->Unjoin(us);}         ///<单个工作对象退出处理函数

            void ProcJoinList();                                                ///<处理要接入的工作对象列表
            void ProcUnjoinList();                                              ///<处理要退出的工作对象列表

        public:

            SocketManageThread(SocketManage *sm)
            {
                sock_manage=sm;
            }

            virtual ~SocketManageThread()
            {
                SAFE_CLEAR(sock_manage);
            }

            virtual void ProcEndThread() override;
            virtual bool Execute() override;

        public:

            virtual AcceptSocketList &JoinBegin(){return join_list.GetPost();}    ///<开始添加要接入的Socket对象
            virtual void            JoinEnd()                                   ///<结束添加要接入的Socket对象
            {
                join_list.ReleasePost();
                join_list.PostSem();
            }

            virtual AcceptSocketList &UnjoinBegin(){return unjoin_list.GetPost();}///<开始添加要退出的Socket对象
            virtual void            UnjoinEnd()                                 ///<结束添加要退出的Socket对象
            {
                unjoin_list.ReleasePost();
                unjoin_list.PostSem();
            }
        };//template<typename TCPAccept> class SocketManageThread:public Thread
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_MANAGE_THREAD_INCLUDE
