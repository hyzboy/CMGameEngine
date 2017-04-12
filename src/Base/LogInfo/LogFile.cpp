#include<hgl/Logger.h>
#include<hgl/FileSystem.h>
#include<hgl/thread/ThreadMutex.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/io/TextOutputStream.h>

namespace hgl
{
    namespace logger
    {
        void GetLocalAppdataPath(os_char fn[HGL_MAX_PATH]);
        io::TextOutputStream *CreateTextOutputStream(io::OutputStream *);

        class LogFile:public Logger
        {
            ThreadMutex lock;

            io::FileOutputStream fos;
            io::TextOutputStream *tos;

        public:

            LogFile(LogLevel ll):Logger(ll)
            {
                tos=nullptr;
            }

            ~LogFile()
            {
                SAFE_CLEAR(tos);
            }

            bool Create(const OSString &project_code)
            {
                os_char filename[HGL_MAX_PATH];
                os_char fn[HGL_MAX_PATH];
                os_char num[16]={'.',0};

                GetLocalAppdataPath(fn);

                strcat(fn,HGL_MAX_PATH,HGL_DIRECTORY_SEPARATOR);
                strcat(fn,HGL_MAX_PATH,OS_TEXT(".cmgdk"),6);
                filesystem::MakePath(fn);

                strcat(fn,HGL_MAX_PATH,HGL_DIRECTORY_SEPARATOR);

                strcat(fn,HGL_MAX_PATH,project_code);

                for(uint i=0;i<=0xFFFF;i++)
                {
                    strcpy(filename,HGL_MAX_PATH,fn);

                    if(i)
                    {
                        utos(num+1,14,i);
                        strcat(filename,HGL_MAX_PATH,num,sizeof(num));
                    }

                    strcat(filename,HGL_MAX_PATH,OS_TEXT(".Loginfo"),8);

                    if(fos.CreateTrunc(filename))//创建成功
                    {
                        tos=CreateTextOutputStream(&fos);
                        tos->WriteBOM();

                        return(true);
                    }
                }

                return(false);
            }

            void Close()
            {
                SAFE_CLEAR(tos);
                fos.Close();
            }

            void Write(const u16char *str,int size)
            {
                if(tos&&str&&*str&&size>0)
                {
//                    lock.Lock();
                    tos->WriteLine(str,size);
//                    lock.Unlock();
                }
            }

            void Write(const char *str,int size)
            {
                if(tos&&str&&*str&&size>0)
                {
//                    lock.Lock();
                    tos->WriteLine(str,size);
//                    lock.Unlock();
                }
            }
        };//class LogFile

        Logger *CreateLoggerFile(const OSString &pc,LogLevel ll)
        {
            if(ll<llError)
                return(nullptr);

            LogFile *lf=new LogFile(ll);

            if(lf->Create(pc))
                return lf;

            delete lf;
            return(nullptr);
        }
    }//namespace logger
}//namespace hgl
