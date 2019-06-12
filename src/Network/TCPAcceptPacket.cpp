#include<hgl/network/TCPAccept.h>
#include<hgl/network/SocketInputStream.h>
#include<hgl/network/SocketOutputStream.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/Str.h>

namespace hgl
{
    namespace network
    {
        TCPAcceptPacket::TCPAcceptPacket():TCPAccept(),recv_buffer(HGL_TCP_BUFFER_SIZE)
        {
        }

        TCPAcceptPacket::TCPAcceptPacket(int s,IPAddress *ip):TCPAccept(s,ip),recv_buffer(HGL_TCP_BUFFER_SIZE)
        {
        }

        /**
         * 从socket接收数据回调函数<br>此写法有待改良
         */
        int TCPAcceptPacket::OnSocketRecv(int /*size*/)
        {
            if(!sis)
            {
                sis=new SocketInputStream(ThisSocket);
                recv_total=0;
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

                if(recv_length<PACKET_SIZE_TYPE_BYTES)      //已经有头了
                    continue;

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

        bool TCPAcceptPacket::SendPacket(void *data,const PACKET_SIZE_TYPE &size)
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
