#include<hgl/network/AcceptServer.h>
#include<hgl/LogInfo.h>
#include<hgl/Time.h>

namespace hgl
{
    void SetTimeVal(timeval &tv,const double t_sec);

    namespace network
    {
        /**
        * 刷新服务器，并等待一个处理消息
        * @return >0 有用户接入
        * @return =0 正常，但无用户接入
        * @return <0 出错
        */
        int AcceptServer::Accept(IPAddress *addr)
        {
            if(!addr)
                return(-1);

            socklen_t sockaddr_size=server_address->GetSockAddrInSize();

            if(accept_timeout.tv_sec
             ||accept_timeout.tv_usec)
            {
                int result;

                hgl_cpy(ato,accept_timeout);            //下面的select会将数据清0,所以必须是复制一份出来用

                FD_ZERO(&accept_set);
                FD_SET(ThisSocket,&accept_set);
                result=select(ThisSocket+1,&accept_set,nullptr,nullptr,&ato);

                if(result<=0)
                    return(0);
            }

            int new_sock=accept(ThisSocket,addr->GetSockAddr(),&sockaddr_size);

            if(new_sock<0)
            {
                const int err=GetLastSocketError();

                if(err==nseTimeOut        //超时
                 ||err==nseNoError        // 0 没有错误
                 ||err==4                 //Interrupted system call(比如ctrl+c,一般DEBUG下才有)
                 ||err==11                //资源临时不可用
                 )
                    return(0);

                LOG_HINT(OS_TEXT("AcceptServer Accept error,errno=")+OSString(err));

                if(err==nseTooManyLink)    //太多的人accept
                {
                    WaitTime(overload_wait);        //强制等待指定时间
                    return(0);
                }

                return(-1);
            }

            const int IP_STR_MAX_SIZE=server_address->GetIPStringMaxSize();

            if(!ipstr)
                ipstr=new char[IP_STR_MAX_SIZE+1];

            addr->ToString(ipstr,IP_STR_MAX_SIZE);

            LOG_INFO(U8_TEXT("AcceptServer Accept IP:")+UTF8String(ipstr)+U8_TEXT(" ,sock:")+UTF8String(new_sock));

            return(new_sock);
        }

        void AcceptServer::SetTimeOut(const double time_out)
        {
            SetTimeVal(accept_timeout,time_out);
        }
    }//namespace network
}//namespace hgl
