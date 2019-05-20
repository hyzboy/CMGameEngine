#include<hgl/PlugIn.h>
#include<hgl/Logger.h>
#include<hgl/type/DateTime.h>
#include<hgl/thread/RWLock.h>

namespace hgl
{
    namespace logger
    {
        RWLockObject<List<Logger *>> log_list;        //记录器列表

        bool AddLogger(Logger *log)
        {
            if(!log)
                return(false);

            OnlyWriteLock owl(log_list);

            if(log_list->IsExist(log))        //重复添加
                return(false);

            log_list->Add(log);
            return(true);
        }

        template<typename T>
        void WriteLog(LogLevel level,const T *str,int size)
        {
            OnlyReadLock orl(log_list);

            const int n=log_list->GetCount();

            if(n<=0)return;

            Logger **log=log_list->GetData();

            for(int i=0;i<n;i++)
            {
                if((*log)->GetLevel()>=level)
                    (*log)->Write(str,size);

                ++log;
            }
        }

        bool PutLogHeader()
        {
            OnlyReadLock orl(log_list);

            if(log_list->GetCount()<=0)
                return(false);

            Date d;
            Time t;

            d.Sync();
            t.Sync();

            const OSString str= OS_TEXT("Create Log Date/Time: ")+
                                OSString(d.GetYear  ())+OS_TEXT("-")+
                                OSString(d.GetMonth ())+OS_TEXT("-")+
                                OSString(d.GetDay   ())+OS_TEXT(" ")+
                                OSString(t.GetHour  ())+OS_TEXT(":")+
                                OSString(t.GetMinute())+OS_TEXT(":")+
                                OSString(t.GetSecond())+OS_TEXT("\n");

            WriteLog(llLog,str.c_str(),str.Length());

            return(true);
        }

        void CloseAllLog()
        {
            OnlyWriteLock owl(log_list);

            const int n=log_list->GetCount();

            if(n<=0)return;

            Logger **log=log_list->GetData();

            for(int i=0;i<n;i++)
            {
                (*log)->Close();
                delete *log;

                ++log;
            }

            log_list->Clear();
        }
    }//namespace logger

    namespace logger
    {
        /**
         * 日志插件接口
         */
        struct LogInterface
        {
            bool (*Add)(Logger *);                                                                      ///<增加一个日志输出

            bool (*Init)();                                                                             ///<初始化日志输出
            void (*Close)();                                                                            ///<关闭所有日志输出

            void (*WriteUTF16)(LogLevel,const u16char *,int);                                           ///<输出一行日志
            void (*WriteUTF8)(LogLevel,const char *,int);                                               ///<输出一行日志
        };//struct LogInterface

        static LogInterface LogInterface3=
        {
            AddLogger,

            PutLogHeader,
            CloseAllLog,

            WriteLog<u16char>,
            WriteLog<char>
        };

        bool GetLogInterface(uint32 ver,void *data)
        {
            if(ver!=3)
                return(false);

            memcpy(data,&LogInterface3,sizeof(LogInterface));
            return(true);
        }

        /**
         * 日志插件,一个模拟的接口
         */
        class LogPlugIn:public PlugIn                                                                    ///日志插件
        {
        public:

            LogPlugIn()
            {
                type=pitLog;

                name=OS_TEXT("unicode text log module.");
                filename=OS_TEXT("LogInfo.cpp");

                GetInterface=GetLogInterface;
            }
        };//class LogPlugIn
    }//namespace logger

    namespace logger
    {
        static LogInterface *li=nullptr;

        PlugIn *InitLog()
        {
            PlugIn *pi=new LogPlugIn;

            li=new LogInterface;

            if(pi->GetInterface(3,li))
                if(li->Init())
                    return(pi);

            delete li;
            li=nullptr;

            UnloadPlugIn(pi);

            return(nullptr);
        }

        void CloseLog()
        {
            if(li)
            {
                li->Close();
                delete li;
                li=nullptr;
            }
        }

        void Log(LogLevel level,const u16char *str,int size)
        {
            if(li)
                li->WriteUTF16(level,str,size==-1?hgl::strlen(str):size);
        }

        void Log(LogLevel level,const char *str,int size)
        {
            if(li)
                li->WriteUTF8(level,str,size==-1?hgl::strlen(str):size);
        }
    }//namespace logger

    namespace logger
    {
        Logger *CreateLoggerConsole (const OSString &,LogLevel);
        Logger *CreateLoggerFile    (const OSString &,LogLevel);

        /**
         * 独立的日志系统初始化<br>
         * 供不整体使用SDK的应用程序使用
         */
        bool InitLogger(const OSString &app_name)
        {
            AddLogger(CreateLoggerConsole(app_name,llLog));
            AddLogger(CreateLoggerFile(app_name,llLog));

            return InitLog();
        }
    }//namespace logger
}//namespace hgl
