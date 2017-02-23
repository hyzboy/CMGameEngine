#include<hgl/Logger.h>
#include<hgl/CodePage.h>
#include<windows.h>

namespace hgl
{
    namespace logger
    {
        class LogWinConsole:public Logger
        {
        private:

            DWORD result;

            void *console_handle;

            u16char buf[4096];

        public:

            LogWinConsole(LogLevel ll):Logger(ll)
            {
                console_handle=GetStdHandle(STD_OUTPUT_HANDLE);
            }

            bool Create(const UTF16String &)
            {
                return(true);
            }

            ~LogWinConsole()
            {
                Close();
            }

            void Close()
            {
                CloseHandle(console_handle);
            }

            void Write(const u16char *str,int size)
            {
                WriteConsoleW(console_handle,str,size,&result,nullptr);
                WriteConsoleW(console_handle,L"\n", 1, &result, nullptr);
            }

            void Write(const char *str,int size)
            {
                const int len=u8_to_u16(buf,4096,str,size);

                if(len<=0)return;

                buf[len]=L'\n';

                WriteConsoleW(console_handle,buf,len+1,&result,nullptr);
            }
        };//class LogWinConsole

        Logger *CreateLoggerConsole(const OSString &,LogLevel ll)
        {
            return(new LogWinConsole(ll));
        }
    }//namespace logger
}//namespace hgl
