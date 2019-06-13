#ifndef HGL_NETWORK_TCP_PACKET_DISTRIBUTE_INCLUDE
#define HGL_NETWORK_TCP_PACKET_DISTRIBUTE_INCLUDE

#include<hgl/network/Socket.h>
namespace hgl
{
    namespace io
    {
        class InputStream;
        class OutputStream;
    }//namespace io

    namespace network
    {
        class TCPSocket;
        class RecvBufferStream;
        class TCPDistribute;

        typedef DefEvent(bool,TCPPacketProcessFunc,(void *))                ///<封包处理事件

        #define HGL_TCP_PACKET_NAME(name)                                   name##Packet

        #define HGL_TCP_PACKET_BEGIN(name)                                  struct HGL_TCP_PACKET_NAME(name):public TCPPacket{using TCPPacket::TCPPacket;

        #define HGL_TCP_PACKET_END(name)                                    public: \
                                                                            \
                                                                                bool Create(io::OutputStream *);    \
                                                                                bool Parse(io::InputStream *);  \
                                                                            };

        #define HGL_TCP_PACKET_PROC_CREATE(name,func)                       (new TCPPacketProc(new HGL_TCP_PACKET_NAME(name),func))

        #define HGL_TCP_PACKET_CREATE_FUNC(name)                            bool HGL_TCP_PACKET_NAME(name)::Create(io::OutputStream *ms)
        #define HGL_TCP_PACKET_PARSE_FUNC(name)                             bool HGL_TCP_PACKET_NAME(name)::Parse(io::InputStream *ms)

        #define HGL_TCP_PACKET_RECV_FUNC(type,name)                         bool OnRecv##name(HGL_TCP_PACKET_NAME(type##name) *);

        #define HGL_TCP_PACKET_RECV_REGISTRY(dis,obj,obj_name,type,name)    {   \
                                                                                TCPPacketProcessFunc ppf;   \
                                                                                \
                                                                                    SetEventCall(ppf,obj,obj_name,OnRecv##name);    \
                                                                                \
                                                                                    dis->Registry(new TCPPacketProc(new HGL_TCP_PACKET_NAME(type##name),ppf));  \
                                                                            }

        /**
        * TCP封包基类，用于封包分发处理
        */
        class TCPPacket                                                                             ///TCP封包基类
        {
            HGL_PACKET_TYPE packet_type;

        public:

            TCPPacket(HGL_PACKET_TYPE type)
            {
                packet_type=type;
            }

            virtual ~TCPPacket()
            {
            }

            const HGL_PACKET_TYPE GetPacketType()const
            {
                return packet_type;
            }

            bool Send(TCPSocket *);                                                                 ///<发送到指定的TCPSocket
            //bool Send(TCPSocket **,int);                                                          ///<发送到指定的一批TCPSocket

            virtual bool Create(io::OutputStream *)=0;                                              ///<将数据转换成流
            virtual bool Parse(io::InputStream *)=0;                                                ///<从流中解晰数据
        };//class TCPPacket

        /**
        * TCP封包处理基类，用于封包处理执行
        */
        class TCPPacketProc
        {
        protected:

            TCPPacket *pack;

            TCPPacketProcessFunc OnProcess;                                                         ///<封包处理事件

        public:

            TCPPacketProc(TCPPacket *tp,TCPPacketProcessFunc ppf)
            {
                pack=tp;
                OnProcess=ppf;
            }

            virtual ~TCPPacketProc()
            {
                delete pack;
            }

            const HGL_PACKET_TYPE GetPacketType()const                                              ///<取得封包类型
            {
                return pack->GetPacketType();
            }

            bool Parse(io::InputStream *rbs)                                                        ///<解晰封包
            {
                return pack->Parse(rbs);
            }

            /**
            * 封包处理，需开发者针对具体包重载
            */
            virtual bool Process()                                                                  ///<处理封包
            {
                if(OnProcess==nullptr)return(false);

                return CallEvent(OnProcess,(pack));
            }
        };//class TCPPacketProc

        /**
        * TCP封包分发处理类
        */
        class TCPDistribute                                                                         ///封包分发处理类
        {
        protected:

            TCPSocket *sock;                                                                        ///<TCP连接

            HGL_PACKET_TYPE start,end;                                                              ///<自动分发封包类型
            TCPPacketProc **packet_list;                                                            ///<自动分发封包对象
            RecvBufferStream *RecvStream;                                                           ///<封包读取流

        public:

            TCPDistribute(TCPSocket *,HGL_PACKET_TYPE,HGL_PACKET_TYPE);
            virtual ~TCPDistribute();

            void SetSocket(TCPSocket *tcp);                                                         ///<设置socket

            bool Registry(TCPPacketProc *);                                                         ///<注册一个封包解晰器
            bool Unregistry(HGL_PACKET_TYPE);                                                       ///<解除一个封包解晰器
            void Clear();                                                                           ///<解除所有封包解晰器

            virtual int Update(int=0);                                                              ///<解收并分发封包
        };//class TCPDistribute
    }//namespace network
}//namespace hgl
#endif//HGL_NETWORK_TCP_PACKET_DISTRIBUTE_INCLUDE
