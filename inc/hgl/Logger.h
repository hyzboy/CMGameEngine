#ifndef HGL_LOGGER_INCLUDE
#define HGL_LOGGER_INCLUDE

#include<hgl/type/BaseString.h>
namespace hgl
{
    namespace logger
    {
        enum LogLevel
        {
            llError=0,		//错误，肯定出对话框
            llProblem,		//问题，默认出对话框
            llHint,			//提示，不重要，debug状态默认出对话框
            llLog			//记录一下
        };//enum LogLevel

        /**
         * 日志输出基类
         */
        class Logger
        {
        protected:

            LogLevel min_level;																		///<最小输出级别
            UTF16String project_code;

        public:

            Logger(LogLevel l){min_level=l;}
            virtual ~Logger()=default;

            const LogLevel GetLevel()const{return min_level;}										///<取得最小输出级别

            virtual void Close()=0;																	///<关闭日志

            virtual void Write(const u16char *,int)=0;												///<输出一行u16char日志
            virtual void Write(const char *,int)=0;													///<输出一行char(utf8)日志
        };//class Logger
    }//namespace logger

    namespace logger
    {
        /**
         * 日志级别枚举
         */
        enum LOG_LEVEL
        {
            LL_START=0,     ///<日志级别起始定义

            LL_HOTPOT,      ///<性能记录热点
            LL_CODE,        ///<代码日志(引擎内部记录)
            LL_USER_LOG,    ///<用户日志(开发人员的日志)
            LL_RUN_LOG,     ///<运行日志(实际使用时的日志)

            LL_END          ///<日志级别结束定义
        };

        enum ERROR_LEVEL
        {
            EL_START=0,

            EL_USER,        ///<用户级(无关仅要，正常程序逻辑报错)
            EL_FUNC,        ///<函数级(当前函数报错)
            EL_MODULE,      ///<模块级
            EL_FULL,        ///<整体级(很严重，只能退出)

            EL_MUST_FIXED,  ///<必须修正的代码问题

            EL_END
        };

        /**
         * 高级日志(未来删除旧的Logger后，改名为Logger)
         */
        class AdvLogger
        {
        protected:

            LOG_LEVEL log_level;
            ERROR_LEVEL err_level;

        protected:

            uint source_file;                           ///<源代所在文件ID
            uint source_line;                           ///<源代码所在行
            uint function;                              ///<函数ID
            HGL_POINTER_UINT object_address;            ///<对象地址
            HGL_POINTER_UINT thread_id;                 ///<线程ID

        public:
        };//class AdvLogger

        /**
         * 线程日志
         */
        class ThreadLogger
        {

        public:


        };//class ThreadLogger

        /**
         * 对象日志
         */
        class ObjectLogger
        {
        };
    }//namespace logger

    using namespace logger;
}//namespace hgl
#endif//HGL_LOGGER_INCLUDE
