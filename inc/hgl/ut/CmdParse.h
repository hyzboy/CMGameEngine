#ifndef HGL_COMMAND_LINE_PARSE_INCLUDE
#define HGL_COMMAND_LINE_PARSE_INCLUDE

#include<hgl/type/StringList.h>
namespace hgl
{
    namespace util
    {
        /**
        * 命令行参数解晰辅助类
        */
        class CmdParse                                                          ///命令行参数解晰辅助类
        {
            OSStringList args;

        public:

            CmdParse(const OSStringList &);
            virtual ~CmdParse();

            int Find(const OSString &)const;                                    ///<查找一个指定字串开头的参数是否存在

            bool GetInteger(const OSString &,int *)const;                       ///<取得一个数值参数
            bool GetString(const OSString &,OSString &)const;                   ///<取得一个字符串参数
        };//class CmdParse
    }//namespace util
}//namespace hgl
#endif//HGL_COMMAND_LINE_PARSE_INCLUDE
