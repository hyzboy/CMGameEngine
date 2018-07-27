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

            Set<TCPAccept *> error_set;

        protected:

            void ProcSocketRecvList();
            void ProcSocketSendList();
            void ProcSocketErrorList();

            void ProcErrorList();

        public:

            const Set<TCPAccept *> &GetErrorSocketSet(){return error_set;}     ///<获取错误SOCKET合集

        public:

            SocketManage(int max_user);
            virtual ~SocketManage();

                    bool Join(TCPAccept *s);
                     int Join(TCPAccept **s_list,int count);

                    bool Unjoin(TCPAccept *s);
                     int Unjoin(TCPAccept **s_list,int count);

            /**
             * 刷新所有操作(删除错误Socket,轮循可用Socket，发送，接收<br>
             * 需要注意的是，Update中轮循到的错误/关闭Socket列表，将在下一次Update时清除。所以在每次调用Update后，请调用GetErrorSocketSet获取错误Socket合集并处理出错Socket
             */
            virtual  int Update(const double &time_out=HGL_NETWORK_TIME_OUT);

            virtual void Clear();
        };//class SocketManage
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SOCKET_MANAGE_INCLUDE
