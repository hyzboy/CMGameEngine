#ifndef HGL_POSIX_THREAD_INCLUDE
#define HGL_POSIX_THREAD_INCLUDE

#include<pthread.h>

namespace hgl
{
    using hgl_thread_mutex  =pthread_mutex_t;

    using thread_ptr        =pthread_t;
    using THREAD_FUNC       =void *;

    #define HGL_THREAD_DETACH_SELF  pthread_detach(pthread_self());
}//namespace hgl
#endif//HGL_POSIX_THREAD_INCLUDE
