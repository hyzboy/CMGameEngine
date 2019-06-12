#include<hgl/network/WebSocketAccept.h>
#include<hgl/network/WebSocket.h>
#include<hgl/network/SocketInputStream.h>
#include<hgl/network/SocketOutputStream.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    namespace network
    {
        //参考工程: https://github.com/katzarsky/WebSocket

        WebSocketAccept::WebSocketAccept():TCPAccept(),recv_buffer(HGL_TCP_BUFFER_SIZE)
        {
        }

        WebSocketAccept::WebSocketAccept(int s,IPAddress *ip):TCPAccept(s,ip),recv_buffer(HGL_TCP_BUFFER_SIZE)
        {
        }

        void WebSocketAccept::WebSocketHandshake()
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
                {
                    LOG_ERROR(OS_TEXT("WebSocketAccept::WebSocketHandshake() read data error"));
                    break;
                }

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
                    if(OnHandshake(ws_accept_protocol,ws_protocol,ws_version))
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

            CloseSocket();
        }

        /**
         * 从socket接收数据回调函数<br>此写法有待改良
         */
        int WebSocketAccept::OnSocketRecv(int /*size*/)
        {
            int total=0;

            if(!sis)
            {
                sis=new SocketInputStream(ThisSocket);
                recv_total=0;
                recv_length=0;
                WebSocketHandshake();

                total+=recv_total;
            }

            while(true)
            {
                if(recv_length==0)
                {
                    msg_header_size=2;                  //最小头2字节
                    msg_full_length=0;
                    msg_length=0;
                    msg_opcode=0;
                    msg_fin=0;
                    msg_masked=0;
                }

                if(recv_length<msg_header_size)         //头都没收完
                {
                    int result=sis->Read(recv_buffer.data()+recv_length,msg_header_size-recv_length);

                    if(result<0)
                        RETURN_ERROR(result);

                    if(result==0)
                        return(total);

                    recv_length+=result;
                    recv_total+=result;
                    total+=result;
                }

                if(recv_length<msg_header_size)      //已经有头了
                    continue;

                uint8 *p=recv_buffer.data();

                msg_opcode  =p[0]&0x7;

                if(msg_opcode==8)   //close
                {
                    this->CloseSocket();
                    return(-1);
                }

                msg_fin     =(p[0]>>7)&0x1;
                msg_masked  =(p[1]>>7)&0x1;

                uint length_field=p[1]&0x7F;

                msg_header_size=2;

                if(length_field<=125)
                {
                    msg_length=length_field;
                }
                else
                if(length_field==126)       //16 bit msg_length
                {
                    msg_header_size+=2;

                    if(recv_length<msg_header_size)
                        return(total);

                    msg_length=ntohs(*(uint16 *)(p+2));
                }
                else
                if(length_field==127)       //64 bit msg_length
                {
                    msg_header_size+=8;

                    if(recv_length<msg_header_size)
                        return(total);

                    msg_length=ntohl(*(uint32 *)(p+2));
                }

                msg_full_length=msg_length;

                if(msg_masked)
                    msg_full_length+=4;

                recv_buffer.SetLength(msg_full_length);

                int result=sis->Read(recv_buffer.data()+recv_length,msg_full_length-(recv_length-msg_header_size));

                if(result<0)
                    RETURN_ERROR(result);

                if(result==0)
                    return total;

                recv_length+=result;
                recv_total+=result;
                total+=result;

                if(recv_length<msg_full_length+msg_header_size)                          //这个包还没收完整
                    return(total);                                                      //证明socket缓冲区里没有数据了，直接返回

                //收完了

                char *pack;

                p+=msg_header_size;

                if(msg_masked)
                {
                    uint32 mask=*(uint32 *)p;

                    p+=4;
                    pack=(char *)p;

                    for(uint i=0;i<msg_length;i++)
                    {
                        *p=(*p)^((uchar *)(&mask))[i%4];
                        ++p;
                    }
                }
                else
                {
                    pack=(char *)p;
                }

                if(msg_opcode==0xA)OnPong();else
                if(msg_opcode==0x9)OnPing();else
                if(msg_opcode>=3
                 &&msg_opcode<=7)
                {
                }
                else
                if(msg_opcode==2)
                {
                    if(msg_length>0)
                    {
                    #ifdef _DEBUG
                        data_out_str.SetLength(msg_length*3);

                        DataToLowerHexStr(data_out_str.data(),(uint8 *)pack,msg_length,',');

                        LOG_INFO(U8_TEXT("WebSocket[")+UTF8String(ThisSocket)+U8_TEXT("] Recv binary [")+UTF8String(msg_length)+U8_TEXT("]: ")+UTF8String(data_out_str.data()));
                    #endif//_DEBUG

                        OnBinary(pack,msg_length,msg_fin);
                    }

                    if(!msg_fin)
                        last_opcode=2;
                    else
                        last_opcode=0;
                }
                else
                if(msg_opcode==1){if(msg_length>0)OnText(pack,msg_length,msg_fin);if(!msg_fin)last_opcode=2;else last_opcode=0;}else
                if(msg_opcode==0)
                {
                    if(msg_length>0)
                    {
                        if(last_opcode==1)
                            OnText(pack,msg_length,msg_fin);
                        else
                        if(last_opcode==2)
                            OnBinary(pack,msg_length,msg_fin);
                    }
                }
                else
                {
                    LOG_PROBLEM(OS_TEXT("WebSocketAccept,opcode error,opcode:")+OSString(msg_opcode)+OS_TEXT(",length:")+OSString(msg_length));
                    OnError();
                }

                recv_buffer.ClearData();
                recv_length=0;
            }//while
        }

        int WebSocketAccept::SendFrame(uint8 opcode,void *msg,uint32 size,bool fin)
        {
            uint8 header[14];
            uint header_size;

            header[0]=opcode&0xF;
            if(fin)
                header[0]|=1<<7;

            if(size<=125)
            {
                header[1]=size;
                header_size=2;
            }
            else
            if(size<=65535)
            {
                header[1]=126;
                header[2]=(size&0xFF00)>>8;
                header[3]= size&0xFF;

                header_size=4;
            }
            else
            {
                header[1]=127;
                header[2]=0;
                header[3]=0;
                header[4]=0;
                header[5]=0;
                header[6]=(size&0xFF000000)>>24;
                header[7]=(size&0xFF0000)>>16;
                header[8]=(size&0xFF00)>>8;
                header[9]= size&0xFF;

                header_size=10;
            }

            if(!sos)
                sos=new SocketOutputStream(ThisSocket);

            if(sos->WriteFully(header,header_size)!=header_size)
                return(-1);

            if(size>0)
            if(sos->WriteFully(msg,size)!=size)
                return(-1);

            return header_size+size;
        }

        bool WebSocketAccept::SendPing()
        {
            return SendFrame(0x9,nullptr,0,true)>0;
        }

        bool WebSocketAccept::SendPong()
        {
            return SendFrame(0xA,nullptr,0,true)>0;
        }

        bool WebSocketAccept::SendBinary(void *data,uint32 size,bool fin)
        {
        #ifdef _DEBUG
            data_out_str.SetLength(size*3);

            DataToLowerHexStr(data_out_str.data(),(uint8 *)data,size,',');

            LOG_INFO(U8_TEXT("WebSocket[")+UTF8String(ThisSocket)+U8_TEXT("] Send binary [")+UTF8String(size)+U8_TEXT("]: ")+UTF8String(data_out_str.data()));
        #endif//_DEBUG

            return SendFrame(0x2,data,size,fin)>0;
        }

        bool WebSocketAccept::SendText(void *text,uint32 size,bool fin)
        {
            return SendFrame(0x1,text,size,fin)>0;
        }
    }//namespace network
}//namespace hgl
