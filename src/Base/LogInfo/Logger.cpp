#include<hgl/Logger.h>
#include<hgl/type/Set.h>
#include<hgl/type/BaseString.h>

namespace hgl
{
    namespace logger
    {
        namespace
        {
            Set<UTF8String> source_file_list;                   ///<所有源代码文件列表
            Set<UTF8String> function_list;                      ///<所有函数列表
        }
    }//namespace logger
}//namespace hgl
