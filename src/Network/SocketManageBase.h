#ifndef HGL_NETWORK_SOCKET_MANAGE_BASE_INCLUDE
#define HGL_NETWORK_SOCKET_MANAGE_BASE_INCLUDE

#include<hgl/network/SocketEvent.h>
namespace hgl
{
    namespace network
    {
        /**
         * Socket基础管理
         */
        class SocketManageBase
        {
        public:

            virtual ~SocketManageBase()=default;

            virtual bool Join(int)=0;                                                               ///<加入一个Socket
//            virtual bool Join(const int *,int)=0;                                                 ///<加入一批Socket
            virtual bool Unjoin(int)=0;                                                             ///<分离一个Socket
//            virtual bool Unjoin(const int *,int)=0;                                               ///<分离一批Socket

            virtual int GetCount()const=0;                                                          ///<取得Socket数量
            virtual void Clear()=0;                                                                 ///<清除所有Socket

            virtual int Update(const double &,
                               SocketEventList &,
                               SocketEventList &,
                               SocketEventList &)=0;                                                ///<轮循刷新所有socket
        };//class SocketManageBase

        SocketManageBase *CreateSocketManageBase(int max_user);                                     ///<创建一个Socket基础管理器
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_SERVER_ACCEPT_MANAGE_BASE_INCLUDE
