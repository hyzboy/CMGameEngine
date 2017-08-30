#include<hgl/proc/Pipe.h>
#include<unistd.h>

namespace hgl
{
    bool CreatePipe(pipe_pair &pp)
    {
        if(pipe(pp))            // return 0 表示成功
            return(false);

        return true;
    }
}//namespace hgl
