#include<hgl/network/WebSocketAccept.h>
#include<hgl/network/WebSocket.h>
#include<hgl/network/SocketInputStream.h>
#include<hgl/network/SocketOutputStream.h>

namespace hgl
{
    namespace network
    {
        namespace 
        {
            enum WebSocketFrame
            {
                wsfTextIncomplete   =0x1,
                wsfBinaryIncomplete =0x2,

                wsfText             =0x81,
                wsfBinary           =0x82,

                wsfPing             =0x19,
                wsfPoing            =0x1A,

                wsfOpeing           =0x3300,
                wsfClosing          =0x3400,

                wsfIncomplete       =0xFE00,
                wsfError            =0xFF00,
            };
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
            if(!sis)
            {
                sis=new SocketInputStream(ThisSocket);
                recv_length=0;
            }

            int total=0;

            if(recv_total==0)       //第一次，进行WebSocket握手
                WebSocketHandshake();

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
                        return(result);

                    if(result==0)
                        return(total);

                    recv_length+=result;
                    recv_total+=result;
                    total+=result;
                }
                
                if(recv_length<msg_header_size)      //已经有头了
                    continue;
                
                uint8 *p=recv_buffer.data();

                msg_opcode  =p[0]&0xF;
                msg_fin     =(p[0]>>7)&0x1;
                msg_masked  =(p[1]>>7)&0x1;
                    
                uint length_field=p[1]&(~0x80);

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

                    msg_length=p[2]+(p[3]<<8);
                }
                else
                if(length_field==127)       //64 bit msg_length
                {
                    msg_header_size+=8;

                    if(recv_length<msg_header_size)
                        return(total);

                    msg_length=p[2]+(p[3]<<8);
                }
                    
                msg_full_length=msg_length;

                if(msg_masked)
                    msg_full_length+=4;

                recv_buffer.SetLength(msg_full_length);

                int result=sis->Read(recv_buffer.data()+recv_length,msg_full_length-(recv_length-msg_header_size));

                if(result<0)
                    return(result);

                if(result==0)
                    return total;

                recv_length+=result;
                recv_total+=result;
                total+=result;

                if(recv_length<msg_full_length+msg_header_size)                          //这个包还没收完整
                    return(total);                                                      //证明socket缓冲区里没有数据了，直接返回

                //收完了

                char *pack;

                if(msg_masked)
                {
                    p+=msg_header_size;
                    pack=(char *)p;
                        
                    uint32 mask=*(uint32 *)p;

                    p+=4;

                    for(uint i=0;i<msg_length;i++)
                    {
                        *p=(*p)^((uchar *)(&mask))[i%4];
                        ++p;
                    }
                }
                else
                {
                    pack=(char *)(p+msg_header_size);
                }

                if(msg_opcode==0xA)OnPong();else
                if(msg_opcode==0x9)OnPing();else
                if(msg_opcode==2)OnBinary(pack,msg_length,msg_fin);else
                if(msg_opcode==1
                 ||msg_opcode==0)OnText(pack,msg_length,msg_fin);
                else
                    OnError();

                recv_buffer.ClearData();
                recv_length=0;
            }//while
        }
    }//namespace network
}//namespace hgl
