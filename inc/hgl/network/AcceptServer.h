#ifndef HGL_NETWORK_ACCEPT_SERVER_INCLUDE
#define HGL_NETWORK_ACCEPT_SERVER_INCLUDE

#include<hgl/network/ServerSocket.h>
namespace hgl
{
    namespace network
    {
        /**
         * 使用Accept处理接入的服务器基类
         */
        class AcceptServer:public ServerSocket                                                      ///使用Accept创建接入的服务器基类
        {
            char *ipstr;

            fd_set accept_set;
            struct timeval accept_timeout,ato;

        protected:

            virtual int CreateServerSocket()=0;                                                     ///<创建Server Socket

        protected:

            double overload_wait;

        public:

            AcceptServer()
            {
                overload_wait=HGL_SERVER_OVERLOAD_RESUME_TIME;
                ipstr = nullptr;

                FD_ZERO(&accept_set);
                hgl_zero(accept_timeout);

                SetTimeOut(HGL_NETWORK_TIME_OUT);
            }

            virtual ~AcceptServer(){SAFE_CLEAR(ipstr);}

                    void SetTimeOut(const double);

            virtual int Accept(IPAddress *);                                                        ///<接入一个socket连接
        };//class AcceptServer
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_ACCEPT_SERVER_INCLUDE
