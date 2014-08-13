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
			virtual ~Logger()HGL_DEFAULT_MEMFUNC;

			const LogLevel GetLevel()const{return min_level;}										///<取得最小输出级别

			virtual void Close()=0;																	///<关闭日志

			virtual void Write(const u16char *,int)=0;												///<输出一行u16char日志
			virtual void Write(const char *,int)=0;													///<输出一行char(utf8)日志
		};//class Logger
	}//namespace logger

	using namespace logger;
}//namespace hgl
#endif//HGL_LOGGER_INCLUDE
