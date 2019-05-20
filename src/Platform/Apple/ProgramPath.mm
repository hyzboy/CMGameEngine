#include<hgl/type/BaseString.h>
#import <Foundation/Foundation.h>

namespace hgl
{
    namespace filesystem
    {
        void GetLocalAppdataPath(char fn[HGL_MAX_PATH])
        {
            NSFileManager *dfm=[NSFileManager defaultManager];

            const char *str=[[[dfm homeDirectoryForCurrentUser] path] cStringUsingEncoding:NSUTF8StringEncoding];

            hgl::strcpy(fn,HGL_MAX_PATH,str,strlen(str));
        }

        /**
        * 取得当前程序完整路径名称
        */
        bool GetCurrentProgram(UTF8String &result)
        {
            NSString *ns_string = [[[[NSBundle mainBundle] bundleURL] URLByDeletingPathExtension] path];

            result.Set([ns_string cStringUsingEncoding:NSUTF8StringEncoding]);

            return(true);
        }

        /**
         * 取得当前程序所在路径
         */
        bool GetCurrentProgramPath(UTF8String &result)
        {
            NSString *ns_string = [[[[NSBundle mainBundle] bundleURL] URLByDeletingLastPathComponent] path];

            result.Set([ns_string cStringUsingEncoding:NSUTF8StringEncoding]);

            return(true);
        }
    }//namespace filesystem
}//namespace hgl

