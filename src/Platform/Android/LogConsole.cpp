#include<hgl/Logger.h>
#include<hgl/CodePage.h>
#include<hgl/thread/ThreadMutex.h>
#include<unistd.h>
#include<pthread.h>

#ifdef LOG_INFO_TIME
#include<hgl/Time.h>
#endif//LOG_INFO_TIME

#include<android/log.h>

namespace hgl
{
    namespace logger
    {
        constexpr uint LOG_BUF_SIZE=4096;

        constexpr android_LogPriority android_priority[]={  ANDROID_LOG_ERROR,          //llError
                                                            ANDROID_LOG_WARN,           //llProblem
                                                            ANDROID_LOG_INFO,           //llHint
                                                            ANDROID_LOG_VERBOSE};       //llLog

        /**
        * Android控制台日志插件接口
        */
        class LogAndroidConsole:public Logger
        {
            char endline;
            char thread_string[256];
            char time_string[256];
            char log_buf[LOG_BUF_SIZE];

#ifdef LOGINFO_THREAD_MUTEX
            ThreadMutex mutex;
#endif//LOGINFO_THREAD_MUTEX

            android_LogPriority prio;

        public:

            LogAndroidConsole(LogLevel ll):Logger(ll)
            {
                prio=android_priority[ll];

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
                memcpy(thread_string,"[Thread:",8);

                htos(thread_string+8,128-9,pthread_self());
                strcat(thread_string,LOG_BUF_SIZE,']');
            }
#endif//LOG_INFO_THREAD

#ifdef LOG_INFO_TIME
            void WriteTime()
            {
                memcpy(time_string,"[Time:",6);

                ftos(time_string+6,128-strlen(time_string),GetDoubleTime());
                strcat(time_string,LOG_BUF_SIZE,']');
            }
#endif//LOG_INFO_TIME

            void Write(const u16char *str,int size)
            {
            #ifdef LOGINFO_THREAD_MUTEX
                mutex.Lock();
            #endif//LOGINFO_THREAD_MUTEX

                log_buf[0]=0;

                #ifdef LOG_INFO_THREAD
                    WriteThreadID();
                    strcpy(log_buf,LOG_BUF_SIZE,thread_string);
                #endif//LOG_INFO_THREAD

                #ifdef LOG_INFO_TIME
                    WriteTime();
                    strcat(log_buf,LOG_BUF_SIZE,time_string);
                #endif//LOG_INFO_TIME

                    int len;

                    len=u16_to_u8(log_buf+hgl::strlen(log_buf),LOG_BUF_SIZE,str,size);

                    if(len>0)
                    {
                        log_buf[len++]=0;

                        __android_log_write(prio,"",log_buf);
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

                log_buf[0]=0;

                #ifdef LOG_INFO_THREAD
                    WriteThreadID();
                    strcpy(log_buf,LOG_BUF_SIZE,thread_string);
                #endif//LOG_INFO_THREAD

                #ifdef LOG_INFO_TIME
                    WriteTime();
                    strcat(log_buf,LOG_BUF_SIZE,time_string);
                #endif//LOG_INFO_TIME

                strcpy(log_buf,LOG_BUF_SIZE,str,size);

                __android_log_write(prio,"",log_buf);

            #ifdef LOGINFO_THREAD_MUTEX
                mutex.Unlock();
            #endif//LOGINFO_THREAD_MUTEX
            }
        };//class LogAndroidConsole:public Logger

        Logger *CreateLoggerConsole(const OSString &,LogLevel ll)
        {
            if(ll<llError)
                return(nullptr);

            return(new LogAndroidConsole(ll));
        }
    }//logger
}//namespace hgl

