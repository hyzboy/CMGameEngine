#include<hgl/io/TextOutputStream.h>
#include<hgl/platform/Platform.h>
#include<hgl/Str.h>
#include<pwd.h>
#include<unistd.h>

namespace hgl
{
    namespace logger
    {
        namespace
        {
            char files_dir[HGL_MAX_PATH];
        }

        void SetLocalAppdataPath(const char *fn)
        {
            strcpy(files_dir,HGL_MAX_PATH,fn);
        }

        void GetLocalAppdataPath(char fn[HGL_MAX_PATH])
        {
            strcpy(fn,HGL_MAX_PATH,files_dir);
        }

        io::TextOutputStream *CreateTextOutputStream(io::OutputStream *os)
        {
            return(new io::UTF8TextOutputStream(os));
        }
    }//namespace logger
}//namespace hgl
