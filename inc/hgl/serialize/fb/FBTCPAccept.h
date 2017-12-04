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
    }//namespace serialize
}//namespace hgl
#endif//HGL_SERIALIZE_FLAT_BUFFERS_TCP_ACCEPT_INCLUDE
