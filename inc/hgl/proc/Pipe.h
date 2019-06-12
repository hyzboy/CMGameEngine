#ifndef HGL_MULTI_PROC_PIPE_INCLUDE
#define HGL_MULTI_PROC_PIPE_INCLUDE

#include<hgl/platform/Platform.h>

#if HGL_OS==HGL_OS_Windows
#include<windows.h>
#endif//

namespace hgl
{
    #if HGL_OS==HGL_OS_Windows
        using pipe_ptr=HANDLE;
        constexpr pipe_ptr PIPE_NULL=nullptr;
    #else
        using pipe_ptr=int;
        constexpr pipe_ptr PIPE_NULL=-1;
    #endif//

    using pipe_pair=pipe_ptr[2];

    bool CreatePipe(pipe_pair &);       ///<创建一对通信管道
}//namespace hgl
#endif//HGL_MULTI_PROC_PIPE_INCLUDE
