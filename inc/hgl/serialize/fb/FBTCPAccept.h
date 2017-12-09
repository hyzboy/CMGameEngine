#ifndef HGL_SERIALIZE_FLAT_BUFFERS_TCP_ACCEPT_INCLUDE
#define HGL_SERIALIZE_FLAT_BUFFERS_TCP_ACCEPT_INCLUDE

#include<hgl/network/TCPAccept.h>
#include<hgl/serialize/fb/msg_proc.h>
namespace hgl
{
    namespace serialize
    {
        namespace fb
        {
            /**
            * 基于FlatBuffers的TCPAccept封包处理接入
            */
            class FBTCPAccept:public TCPAccept
            {
            protected:

                fb::MessageProc msg_proc;                                                   ///<消息分发处理器

                fb::MessageProc::MSG_PROC tmp_proc;

            public:

                FBTCPAccept(int s,IPAddress *ip,uint msg_start,uint msg_end):
                TCPAccept(s,ip),
                msg_proc(msg_start,msg_end)
                {
                }

                virtual ~FBTCPAccept()=default;

                bool OnRecvPacket(void *data,const PACKET_SIZE_TYPE &size) override
                {
                    uint32 *id=(uint32 *)data;

                    return msg_proc.Proc(*id,id+1);
                }
            };//class FBTCPAccept:public TCPAccept

            #define FBTCP_MSG_MAP(ns,cn,mn) FB_MSG_MAP(msg_proc,tmp_proc,ns,cn,mn)
        }//namespace fb

        //使用范例
        //
        //class UserAccept:public fb::FBTCPAccept
        //{
        //public:
        //
        //    UserAccept(int s,IPAddress *ip):FBTCPAccept(s,ip,c2zNone,c2zEnd)
        //    {           //                                      ^      ^
        //                //                                      |      |
        //                //                                      |      +----------结束消息
        //                //                                      +-----------------起始消息
        //                //                                   +--------------------名字空间
        //                //                                   |      +-------------消息处类名称
        //                //                                   |      |        +----消息名称
        //                //                                   |      |        |
        //                //                                   v      v        v
        //        #define USER_MSG_MAP(name)    FBTCP_MSG_MAP(c2z,UserAccept,name)
        //
        //        //定义消息映射
        //
        //        USER_MSG_MAP(Login)
        //        USER_MSG_MAP(Move)
        //    }
        //
        //    ~UserAccept()=default;
        //
        //    bool OnProcLogin(const c2z::Login *pack)
        //    {
        //
        //        return(true);
        //    }
        //
        //    bool OnProcMove(const c2z::Move *pack)
        //    {
        //
        //        return(true);
        //    }
        //};//class UserAccept:public fb::FBTCPAccept
    }//namespace serialize
}//namespace hgl
#endif//HGL_SERIALIZE_FLAT_BUFFERS_TCP_ACCEPT_INCLUDE
