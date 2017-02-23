#include<hgl/io/TextOutputStream.h>
#include<windows.h>
#include<shlobj.h>

namespace hgl
{
    namespace logger
    {
        void GetLocalAppdataPath(os_char fn[HGL_MAX_PATH])
        {
            SHGetFolderPathW(nullptr,CSIDL_LOCAL_APPDATA,nullptr,0,fn);
        }

        io::TextOutputStream *CreateTextOutputStream(io::OutputStream *os)
        {
            return(new io::UTF16LETextOutputStream(os));
        }
    }//namespace logger
}//namespace hgl
