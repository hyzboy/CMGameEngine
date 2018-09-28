#include<hgl/type/BaseString.h>
#import <Foundation/Foundation.h>

namespace hgl
{
    namespace filesystem
    {
        void GetLocalAppdataPath(os_char fn[HGL_MAX_PATH])
        {
            NSFileManager *dfm=[NSFileManager defaultManager];

            NSString *ns_string=[[dfm homeDirectoryForCurrentUser] path];

            hgl::strcpy(fn,HGL_MAX_PATH,[ns_string cStringUsingEncoding:NSUTF8StringEncoding],[ns_string length]);
        }

        /**
        * 取得当前程序完整路径名称
        */
        bool GetCurrentProgram(OSString &result)
        {
            NSURL *url = [[[NSBundle mainBundle] bundleURL] URLByDeletingPathExtension];

            NSString *ns_string=[url path];

            result.Set([ns_string cStringUsingEncoding:NSUTF8StringEncoding],[ns_string length]);

            return(true);
        }

        /**
         * 取得当前程序所在路径
         */
        bool GetCurrentProgramPath(OSString &result)
        {
            NSURL *url = [[[NSBundle mainBundle] bundleURL] URLByDeletingLastPathComponent];

            NSString *ns_string=[url path];

            result.Set([ns_string cStringUsingEncoding:NSUTF8StringEncoding],[ns_string length]);

            return(true);
        }
    }//namespace filesystem
}//namespace hgl

