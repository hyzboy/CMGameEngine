#include<hgl/Logger.h>
#include<hgl/Other.h>
#include<hgl/thread/ThreadMutex.h>
#include<unistd.h>
#include<pthread.h>

namespace hgl
{
	namespace logger
	{
    #define LOG_BUF_SIZE    4096

#ifndef LOGINFO_THREAD_MUTEX
		static HGL_THREAD char log_buf[HGL_MAX_PATH];		//HGL_THREAD关键字代表使用thread-local storage，即这个数据会每个线程有一份，所以无需加锁
#endif//LOGINFO_THREAD_MUTEX

		/**
		* unix控制台日志插件接口
		*/
		class LogUnixConsole:public Logger
		{
			char endline;

#ifdef LOGINFO_THREAD_MUTEX
			char log_buf[HGL_MAX_PATH];
			ThreadMutex mutex;
#endif//LOGINFO_THREAD_MUTEX

		public:

			LogUnixConsole(LogLevel ll):Logger(ll)
			{
				endline='\n';
			}

			bool Create(const OSString &)
			{
				return(true);
			}

			void Close(){}

#ifdef LOG_INFO_THREAD
			void WriteThreadID()
			{
				memcpy(log_buf,"[Thread:",8);
				int size;

				htos(log_buf+8,128-9,pthread_self());
				strcat(log_buf,HGL_MAX_PATH,']');

				write(STDOUT_FILENO,log_buf,strlen(log_buf));
			}
#endif//LOG_INFO_THREAD

#ifdef LOG_INFO_TIME
			void WriteTime()
			{
				memcpy(log_buf,"[Time:",6);

				ftos(log_buf+6,128-strlen(log_buf),GetDoubleTime());
				strcat(log_buf,HGL_MAX_PATH,']');

				write(STDOUT_FILENO,log_buf,strlen(log_buf));
			}
#endif//LOG_INFO_TIME

			void Write(const u16char *str,int size)
			{
			#ifdef LOGINFO_THREAD_MUTEX
				mutex.Lock();
			#endif//LOGINFO_THREAD_MUTEX

				#ifdef LOG_INFO_THREAD
					WriteThreadID();
				#endif//LOG_INFO_THREAD

				#ifdef LOG_INFO_TIME
					WriteTime();
				#endif//LOG_INFO_TIME

					int len;

					len=u16_to_u8(log_buf,4096,str,size);

					if(len>0)
					{
						log_buf[len++]='\n';

						write(STDOUT_FILENO,log_buf,len);
					}
			#ifdef LOGINFO_THREAD_MUTEX
				mutex.Unlock();
			#endif//LOGINFO_THREAD_MUTEX
			}

			void Write(const char *str,int size)
			{
			#ifdef LOGINFO_THREAD_MUTEX
				mutex.Lock();
			#endif//LOGINFO_THREAD_MUTEX

				#ifdef LOG_INFO_THREAD
					WriteThreadID();
				#endif//LOG_INFO_THREAD

				#ifdef LOG_INFO_TIME
					WriteTime();
				#endif//LOG_INFO_TIME

					write(STDOUT_FILENO,str,size);
					write(STDOUT_FILENO,&endline,1);
			#ifdef LOGINFO_THREAD_MUTEX
				mutex.Unlock();
			#endif//LOGINFO_THREAD_MUTEX
			}
		};//class LogInterface

		Logger *CreateLoggerConsole(const OSString &,LogLevel ll)
		{
			if(ll<llError)
				return(nullptr);

			return(new LogUnixConsole(ll));
		}
	}//logger
}//namespace hgl
