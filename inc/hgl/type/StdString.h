#pragma once

#include<string>
#include<hgl/type/BaseString.h>
#include<hgl/CodePage.h>

#if HGL_OS == HGL_OS_Windows
inline hgl::OSString ToOSString(const std::string &str)
{
    return hgl::to_u16(str.c_str(),(int)(str.length()));
}

inline std::string ToStdString(const hgl::OSString &str)
{
    UTF8String u8_str=hgl::to_u8(str);

    return std::string(u8_str.c_str());
}
#else
inline hgl::OSString ToOSString(const std::string &str)
{
    return hgl::OSString(str.c_str(),str.size());
}

inline std::string ToStdString(const hgl::OSString &str)
{
    return std::string(str.c_str());
}
#endif//

inline hgl::UTF8String ToUTF8String(const std::string &str)
{
    return hgl::UTF8String(str.c_str(),str.size());
}

