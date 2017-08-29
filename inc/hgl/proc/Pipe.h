#ifndef HGL_MULTI_PROC_PIPE_INCLUDE
#define HGL_MULTI_PROC_PIPE_INCLUDE

#include<hgl/platform/Platform.h>

#if HGL_OS==HGL_OS_Windows
#include<windows.h>
#endif//

namespace hgl
{
    #if HGL_OS==HGL_OS_Windows
        using PipePtr=HANDLE;
    #else
        using PipePtr=int;
    #endif//

    using PipePair=PipePtr[2];

    bool CreatePipe(PipePair &);            ///<创建一对通信管理
}//namespace hgl
#endif//HGL_MULTI_PROC_PIPE_INCLUDE
