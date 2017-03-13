#include<hgl/network/TCPSocket.h>
#include<hgl/thread/RingBuffer.h>
#include<hgl/MemBlock.h>

namespace hgl
{
    namespace network
    {
        void TCPSocketRB::InitPrivate()
        {
            RecvBuffer=new RingBuffer(HGL_TCP_BUFFER_SIZE);
            SendBuffer=new RingBuffer(HGL_TCP_BUFFER_SIZE);

            recv_temp_buffer=new MemBlock<char>(HGL_TCP_BUFFER_SIZE);
            send_temp_buffer=new char[HGL_TCP_BUFFER_SIZE];

            ris=new io::RingInputStream(RecvBuffer);
            ros=new io::RingOutputStream(SendBuffer);
        }

        TCPSocketRB::TCPSocketRB()
        {
            InitPrivate();
        }

        TCPSocketRB::TCPSocketRB(int s,IPAddress *addr):TCPSocket()
        {
            InitPrivate();
            UseSocket(s,addr);
        }

        TCPSocketRB::~TCPSocketRB()
        {
            delete[] send_temp_buffer;
            delete recv_temp_buffer;

            delete SendBuffer;
            delete RecvBuffer;

            delete ros;
            delete ris;
        }

        void TCPSocketRB::UseSocket(int sock,IPAddress *addr) 
        {
            TCPSocket::UseSocket(sock,addr);

            ris->GetRingBuffer()->Clear();
            ros->GetRingBuffer()->Clear();
        }
    }//namespace network
}//namespace hgl
