#include<hgl/type/BaseString.h>
#include<windows.h>
#include<shlobj.h>

namespace hgl
{
    namespace filesystem
    {
        void GetLocalAppdataPath(os_char fn[HGL_MAX_PATH])
        {
            SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, fn);
        }

        /**
         * 取得当前程序完整路径名称
         */
        bool GetCurrentProgram(OSString &result)
        {
            os_char *path=new os_char[HGL_MAX_PATH];

            GetModuleFileNameW(nullptr,path,HGL_MAX_PATH);

            result = path;
            delete[] path;

            return(true);
        }

        /**
         * 取得当前程序所在路径
         */
        bool GetCurrentProgramPath(OSString &result)
        {
            os_char *path=new os_char[HGL_MAX_PATH];

            int len=GetModuleFileNameW(nullptr,path,HGL_MAX_PATH);

            os_char *right=hgl::strrchr(path,len,HGL_DIRECTORY_SEPARATOR);

            if(right)
                *right=0;

            result = path;
            delete[] path;

            return(true);
        }
    }//namespace filesystem
}//namespace hgl
