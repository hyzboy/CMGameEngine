#include<hgl/type/DataType.h>

namespace hgl
{
    bool CheckSystemEndian()
    {
        constexpr uint16 db=0xABCD;

    #if HGL_ENDIAN == HGL_BIG_ENDIAN
        return(*(uint8 *)&db==0xAB);
    #else
        return(*(uint8 *)&db==0xCD);
    #endif//HGL_BIG_ENDIAN
    }
}//namespace hgl
