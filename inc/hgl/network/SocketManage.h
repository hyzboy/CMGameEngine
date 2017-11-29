#ifndef HGL_NETWORK_SOCKET_MANAGE_INCLUDE
#define HGL_NETWORK_SOCKET_MANAGE_INCLUDE

#include<hgl/type/Map.h>
#include<hgl/network/SocketEvent.h>
#include<hgl/network/TCPAccept.h>
namespace hgl
{
    namespace network
    {
        class SocketManageBase;

        /**
         * 最简单的服Socket管理类，直接在一个Update内处理socket的轮循和处理事件(不关心是recv还是send)<br>
         * 该类所有函数均为非线程安全，所以不可以直接在多线程中使用
         */
        class SocketManage
        {
        protected:

            using SocketList=Map<int,TCPAccept *>;

        protected:

            SocketList socket_list;

            SocketManageBase *manage;                                           ///<实际的Socket管理器

            SocketEventList sock_recv_list,
                            sock_send_list,
                            sock_error_list;

            Set<TCPAccept *> error_list;

        protected:

            void ProcSocketRecvList();
            void ProcSocketSendList();
            void ProcSocketErrorList();

            void ProcErrorList();

        public:

            SocketManage(int max_user);
            virtual ~SocketManage();

                    bool Join(TCPAccept *s);
                     int Join(TCPAccept **s_list,int count);

                    bool Unjoin(TCPAccept *s);
                     int Unjoin(TCPAccept **s_list,int count);

            virtual  int Update(const double &time_out=HGL_NETWORK_TIME_OUT);

            virtual void Clear();
        };//class SocketManage
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_MANAGE_INCLUDE
