#include<hgl/type/BaseString.h>
#include<unistd.h>
#include<pwd.h>
#include<unistd.h>

namespace hgl
{
    namespace filesystem
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

        /**
        * 取得当前程序完整路径名称
        */
        bool GetCurrentProgram(OSString &result)
        {
            os_char *path=new os_char[HGL_MAX_PATH];

            const int len=readlink("/proc/self/exe",path,HGL_MAX_PATH);

            if(len<=0)
            {
                delete[] path;
                return(false);
            }

            path[len]=0;
            result.Set(path,len,true);

            return(true);
        }

        /**
         * 取得当前程序所在路径
         */
        bool GetCurrentProgramPath(OSString &result)
        {
            os_char *path=new os_char[HGL_MAX_PATH];

            const int len=readlink("/proc/self/exe",path,HGL_MAX_PATH);

            if(len<=0)
            {
                delete[] path;
                return(false);
            }

            os_char *right=hgl::strrchr(path,len,HGL_DIRECTORY_SEPARATOR);

            if(right)
                *right=0;
            else
                path[len]=0;

            result.Set(path,len,true);

            return(true);
        }
    }//namespace filesystem
}//namespace hgl


