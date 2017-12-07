#include<hgl/network/TCPAccept.h>
#include<hgl/network/SocketInputStream.h>
#include<hgl/network/SocketOutputStream.h>
#include<hgl/network/WebSocket.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/Str.h>

namespace hgl
{
    namespace network
    {
        TCPAccept::TCPAccept():TCPSocket(),recv_buffer(HGL_TCP_BUFFER_SIZE)
        {
        }

        TCPAccept::TCPAccept(int s,IPAddress *ip):TCPSocket(s,ip),recv_buffer(HGL_TCP_BUFFER_SIZE)
        {
        }

        TCPAccept::~TCPAccept()
        {
            SAFE_CLEAR(sos);
            SAFE_CLEAR(sis);
        }

        void TCPAccept::RecvWebSocketHeader()
        {
            constexpr char HTTP_HEADER_END_STR[4]={'\r','\n','\r','\n'};        //别用"\r\n\r\n"，不然sizeof会得出来5
            constexpr int HTTP_HEADER_END_SIZE=sizeof(HTTP_HEADER_END_STR);

            MemBlock<char>  ws_header(1024);

            int pos=0;
            int total=0;
            int size=0;

            const char *end;

            while(true)
            {
                size=sis->Read(ws_header.data()+pos,1024);

                if(size==0)continue;
                if(size<0)
                    break;
                
                recv_total+=size;
                pos+=size;

                end=hgl::strrstr<const char>(ws_header.data(),pos,HTTP_HEADER_END_STR,HTTP_HEADER_END_SIZE);

                if(!end)
                    continue;

                end+=HTTP_HEADER_END_SIZE;
                total=end-ws_header.data();

                UTF8String key;
                UTF8String ws_protocol;                                     
                uint       ws_version;
                    
                UTF8String ws_accept_protocol;

                if(GetWebSocketInfo(key,ws_protocol,ws_version,ws_header.data(),total))
                    if(OnWebSocket(ws_accept_protocol,ws_protocol,ws_version))
                    {
                        UTF8String ws_return;

                        MakeWebSocketAccept(ws_return,key,ws_accept_protocol);

                        if(Send(ws_return.c_str(),ws_return.Length()))
                        {
                            size=pos-total;

                            if(size>0)
                                memcpy(recv_buffer.data(),end,size);
                            
                            recv_length=size;                            
                            return;
                        }
                    }

                break;
            }

            this->CloseSocket();
        }

        /**
         * 从socket接收数据回调函数<br>此写法有待改良
         */
        int TCPAccept::OnSocketRecv(int /*size*/)
        {
            if(!sis)
            {
                sis=new SocketInputStream(ThisSocket);
                recv_length=0;
            }

            int total=0;

            while(true)
            {
                if(recv_length<PACKET_SIZE_TYPE_BYTES)       //头都没收完
                {
                    int result=sis->Read(recv_buffer.data()+recv_length,PACKET_SIZE_TYPE_BYTES-recv_length);

                    if(result<0)
                        return(result);

                    if(result==0)
                        return total;

                    recv_length+=result;
                    recv_total+=result;
                    total+=result;
                }
                
                if(recv_total==PACKET_SIZE_TYPE_BYTES)              //最开始，判断一下是不是WebSocket
                {
                    if(memcmp(recv_buffer.data(),"GET ",4)==0)      //HTTP头，可能是WebSocket
                    {
                        RecvWebSocketHeader();
                        continue;
                    }
                }

                if(recv_length>=PACKET_SIZE_TYPE_BYTES)      //已经有头了
                {
                    PACKET_SIZE_TYPE pack_size=*(PACKET_SIZE_TYPE *)(recv_buffer.data());

                    recv_buffer.SetLength(PACKET_SIZE_TYPE_BYTES+pack_size);

                    int result=sis->Read(recv_buffer.data()+recv_length,pack_size-(recv_length-PACKET_SIZE_TYPE_BYTES));

                    if(result<0)
                        return(result);

                    if(result==0)
                        return total;

                    recv_length+=result;
                    recv_total+=result;
                    total+=result;

                    if(recv_length<pack_size+PACKET_SIZE_TYPE_BYTES)                //这个包还没收完整
                        return(total);                                                      //证明socket缓冲区里没有数据了，直接返回

                    OnRecvPacket(recv_buffer.data()+PACKET_SIZE_TYPE_BYTES,pack_size);  //调用回调

                    recv_buffer.ClearData();
                    recv_length=0;
                }
            }
        }

        int TCPAccept::OnSocketSend(int /*size*/)
        {
            return 0;
        }

        void TCPAccept::OnSocketError(int error_no)
        {
        }

        bool TCPAccept::Send(void *data,const uint size)
        {
            if(!data)return(false);
            if(size<=0)return(false);

            if(!sos)
                sos=new SocketOutputStream(ThisSocket);

            int result=sos->WriteFully(data,size);

            if(result!=size)
                return(false);

            return(true);
        }

        bool TCPAccept::SendPacket(void *data,const PACKET_SIZE_TYPE &size)
        {
            if(!data)return(false);
            if(size<=0)return(false);

            //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$4
            //直接发，，，，正式版 本当然不能这样写，，，但现在先这样了

            if(!sos)
                sos=new SocketOutputStream(ThisSocket);

            int result=sos->WriteFully(&size,sizeof(PACKET_SIZE_TYPE));

            if(result!=sizeof(PACKET_SIZE_TYPE))
                return(false);

            result=sos->WriteFully(data,size);

            if(result!=size)
                return(false);

            return(true);
        }
    }//namespace network
}//namespace hgl
