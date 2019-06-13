#ifndef HGL_XRAM_INCLUDE
#define HGL_XRAM_INCLUDE

#include<hgl/al/al.h>
namespace openal
{
    // XRAM Extension function pointer variables and enum values

    typedef ALboolean (AL_APIENTRY *LPEAXSETBUFFERMODE)(ALsizei n, ALuint *buffers, ALint value);
    typedef ALenum    (AL_APIENTRY *LPEAXGETBUFFERMODE)(ALuint buffer, ALint *value);

    extern LPEAXSETBUFFERMODE eaxSetBufferMode;
    extern LPEAXGETBUFFERMODE eaxGetBufferMode;

    // X-RAM Enum values
    extern ALenum eXRAMSize, eXRAMFree;
    extern ALenum eXRAMAuto, eXRAMHardware, eXRAMAccessible;
}//namespace openal
#endif//
