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
        TCPAccept::~TCPAccept()
        {
            SAFE_CLEAR(sos);
            SAFE_CLEAR(sis);
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
    }//namespace network
}//namespace hgl
