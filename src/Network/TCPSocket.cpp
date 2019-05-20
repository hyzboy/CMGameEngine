#include<hgl/network/TCPSocket.h>
#include<hgl/LogInfo.h>

#if HGL_OS != HGL_OS_Windows
#include<netinet/tcp.h>
#endif//HGL_OS != HGL_OS_Windows

namespace hgl
{
    void SetTimeVal(timeval &tv,const double t_sec);

    namespace network
    {
        /**
        * 设置是否使用无延迟模式(注：无延迟模式也无缓冲，有利于小包高响应应用，大包或无高响应需要求应用不要设置)
        * @param no_delay 是否无延迟
        */
        bool TCPSocket::SetNodelay(bool no_delay)
        {
            int flag=(no_delay?1:0);

            return(setsockopt(ThisSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&flag,sizeof(flag))!=-1);
        }

        /**
         * 设置是否使唤用KEEPALIVE自动保持连接机制
         * @param keep_alive    是否自动保持连接
         * @param idle            没有数据交互，开始探测的是时间(默认为7200秒)
         * @param interval        探测之间的时间间隔(默认为75秒)
         * @param count            探测的最大次数(默认为9次)
         */
        void TCPSocket::SetKeepAlive(bool keep_alive,const int idle,const int interval,const int count)
        {
            int flag=(keep_alive?1:0);

#if HGL_OS != HGL_OS_Windows
            setsockopt(ThisSocket,SOL_SOCKET,SO_KEEPALIVE,&flag,sizeof(flag));
#else
            setsockopt(ThisSocket, SOL_SOCKET, SO_KEEPALIVE, (const char *)&flag, sizeof(flag));
#endif//windows

            if(!flag)
                return;

#if HGL_OS != HGL_OS_Windows        //windows不支持
    #if HGL_OS != HGL_OS_macOS
            setsockopt(ThisSocket,IPPROTO_TCP,TCP_KEEPIDLE,   &idle,        sizeof(int));
    #endif//HGL_OS_macOS
            setsockopt(ThisSocket,IPPROTO_TCP,TCP_KEEPINTVL,  &interval,    sizeof(int));
            setsockopt(ThisSocket,IPPROTO_TCP,TCP_KEEPCNT,    &count,       sizeof(int));
#endif//HGL_OS_Windows
        }

        void TCPSocket::ResetConnect()
        {
            FD_ZERO(&local_set);
            FD_SET(ThisSocket,&local_set);
        }

        /**
        * 使用指定socket
        * @param sock 指定socket编号
        * @param addr socket地址
        */
        bool TCPSocket::UseSocket(int sock,const IPAddress *addr)
        {
            if(sock<0)
                RETURN_FALSE;

            if(!addr->IsTCP())
                RETURN_FALSE;

            if(!Socket::UseSocket(sock,addr))
                RETURN_FALSE;

            ResetConnect();
            return(true);
        }

        /**
         * 取得当前socket是否还在联接状态
         */
        bool TCPSocket::IsConnect()
        {
            return(ThisSocket!=-1);
        }

        /**
         * 等待数据接收
         * @param wait_time 等待的时间
         * @return >0 有数据
         * @return 0 无数据，但不是错误（如超时）
         * @return <0 出错
         */
        int TCPSocket::WaitRecv(double wait_time)
        {
            if(ThisSocket==-1)
                RETURN_ERROR(-1);

            SetTimeVal(time_out,wait_time);

            memcpy(&recv_set,&local_set,sizeof(fd_set));
            memcpy(&err_set,&local_set,sizeof(fd_set));

            const int result=select(ThisSocket+1,&recv_set,nullptr,&err_set,&time_out);

            if(result<0)
            {
                int err=GetLastSocketError();

                RETURN_ERROR(-err);
            }

            if(FD_ISSET(ThisSocket,&recv_set))
                return(1);

            if(FD_ISSET(ThisSocket,&err_set))
                RETURN_ERROR(-1);

            return(0);
        }
    }//namespace network
}//namespace hgl
