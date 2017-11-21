#include<hgl/network/TCPServer.h>

#if HGL_OS != HGL_OS_Windows
#include<netinet/tcp.h>
#endif//

namespace hgl
{
    namespace network
    {
        int TCPServer::CreateServerSocket()
        {
            //独立一个函数，是因为Windows下创建IOCP Server时，这里需要特殊实现
            return socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        }

#if HGL_OS != HGL_OS_Windows
        /**
         * 设置推迟Accept（在接收到第一个数据后，才会Accept成功）
         * @param time_out 收到数据的时间限制(单位秒)
         */
        void TCPServer::SetDeferAccept(const int time_out)
        {
            setsockopt(this->ThisSocket,SOL_TCP,TCP_DEFER_ACCEPT,&time_out,sizeof(time_out));
        }
#endif//#if HGL_OS != HGL_OS_Windows
    }//namespace network
}//namespace hgl
