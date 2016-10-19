#include<hgl/io/TextOutputStream.h>
#include<pwd.h>
#include<unistd.h>

namespace hgl
{
	namespace logger
	{
        void GetLocalAppdataPath(os_char fn[HGL_MAX_PATH])
        {
            struct passwd pwd;
            struct passwd *result;
            char *buf;
            size_t bufsize=sysconf(_SC_GETPW_R_SIZE_MAX);

            if(bufsize==-1)
                bufsize=16384;

            buf=(char *)malloc(bufsize);

            getpwuid_r(getuid(),&pwd,buf,bufsize,&result);

            strcpy(fn,HGL_MAX_PATH,pwd.pw_dir);

            free(buf);
        }

        io::TextOutputStream *CreateTextOutputStream(io::OutputStream *os)
        {
            return(new io::UTF8TextOutputStream(os));
        }
	}//namespace logger
}//namespace hgl
