#include<hgl/proc/Pipe.h>

namespace hgl
{
    bool CreatePipe(pipe_pair &pp)
    {
        SECURITY_ATTRIBUTES sa;

        sa.nLength=sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle=TRUE;
        sa.lpSecurityDescriptor=nullptr;

        return CreatePipe(&(pp[0]),&(pp[1]),&sa,0);
    }
}//namespace hgl

