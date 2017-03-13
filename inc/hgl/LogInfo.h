#ifndef HGL_LOGINFO_INCLUDE
#define HGL_LOGINFO_INCLUDE

#ifndef __MAKE_PLUGIN__
#include<hgl/Logger.h>
#include<hgl/CodePage.h>

#if (HGL_COMPILER == HGL_COMPILER_GNU)||(HGL_COMPILER == HGL_COMPILER_LLVM)
    #define __HGL_FUNC__    __PRETTY_FUNCTION__
#elif HGL_COMPILER == HGL_COMPILER_Microsoft
    #define __HGL_FUNC__    __FUNCSIG__
#else
    #define __HGL_FUNC__    __FUNCTION__
#endif//

namespace hgl
{
	class PlugIn;

	namespace logger
	{
		bool AddLogger(Logger *);																	///<增加日志输出者

#ifndef NO_LOGINFO
		void Log(LogLevel,const u16char *,int=-1);													///<输出日志
		void Log(LogLevel,const char *,int=-1);														///<输出日志(utf8)
		inline	void Log(LogLevel ll,const UTF16String &str){Log(ll,str.c_str(),str.Length());}		///<输出日志
		inline	void Log(LogLevel ll,const UTF8String &str){Log(ll,str.c_str(),str.Length());}		///<输出日志

		inline	void DebugLog(LogLevel ll,const UTF16String &str,const char *filename,int line,const char *funcname)
		{
			Log(ll,str+U16_TEXT(" in \"")+to_u16(filename)+U16_TEXT("\", ")+UTF16String(line)+U16_TEXT(" line,func:")+to_u16(funcname));
		}

		inline	void DebugLog(LogLevel ll,const UTF8String &str,const char *filename,int line,const char *funcname)
		{
			Log(ll,str+U8_TEXT(" in \"")+UTF8String(filename)+U8_TEXT("\", ")+UTF8String(line)+U8_TEXT(" line,func:")+UTF8String(funcname));
		}

		#ifdef LOG_INFO_SOURCE
			#define LOG_INFO(str)		{DebugLog(llLog,	str,__FILE__,__LINE__,__HGL_FUNC__);}
			#define LOG_HINT(str)		{DebugLog(llHint,	str,__FILE__,__LINE__,__HGL_FUNC__);}
			#define LOG_PROBLEM(str)	{DebugLog(llProblem,str,__FILE__,__LINE__,__HGL_FUNC__);}
			#define LOG_ERROR(str)		{DebugLog(llError,	str,__FILE__,__LINE__,__HGL_FUNC__);}
		#else
			#define LOG_INFO(str)		{Log(llLog,		str);}
			#define LOG_HINT(str)		{Log(llHint,	str);}
			#define LOG_PROBLEM(str)	{Log(llProblem,	str);}
			#define LOG_ERROR(str)		{Log(llError,	str);}
		#endif//LOG_INFO_SOURCE

		#define RETURN_FALSE		{DebugLog(llLog,OS_TEXT("return(false)"							),__FILE__,__LINE__,__HGL_FUNC__);return(false);}
		#define RETURN_ERROR(v)		{DebugLog(llLog,OS_TEXT("return error(")+OSString(v)+OS_TEXT(")"),__FILE__,__LINE__,__HGL_FUNC__);return(v);}
		#define RETURN_ERROR_NULL	{DebugLog(llLog,OS_TEXT("return error(nullptr)"					),__FILE__,__LINE__,__HGL_FUNC__);return(nullptr);}

		#define RETURN_BOOL(proc)	{if(proc)return(true);RETURN_FALSE}
#else
		#define LOG_INFO(str)		{}
		#define LOG_HINT(str)		{}
		#define LOG_PROBLEM(str)	{}
		#define LOG_ERROR(str)		{}

		#define RETURN_FALSE		return(false);
		#define RETURN_ERROR(v)		return(v);
		#define RETURN_ERROR_NULL	return(nullptr);

		#define RETURN_BOOL(proc)	return(proc);
#endif//NO_LOGINFO

        #define IF_FALSE_RETURN(str)    if(!str)RETURN_FALSE;
	}//namespace logger

	using namespace logger;
}//namespace hgl
#endif//__MAKE_PLUGIN__
#endif//HGL_LOGINFO_INCLUDE
