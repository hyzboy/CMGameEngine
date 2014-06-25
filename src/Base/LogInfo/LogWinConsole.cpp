#include<hgl/Logger.h>
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

			char16_t buf[4096];

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

			void Write(const char16_t *str,int size)
			{
				WriteConsole(console_handle,str,size,&result,nullptr);
				WriteConsole(console_handle,u"\n",1,&result,nullptr);
			}

			void Write(const char *str,int size)
			{
				const int len=u8_to_u16(buf,4096,str,size);

				if(len<=0)return;

				buf[len]=u'\n';

				WriteConsole(console_handle,buf,len+1,&result,nullptr);
			}
		};//class LogWinConsole

		Logger *CreateLoggerConsole(const UTF16String &,LogLevel ll)
		{
			return(new LogWinConsole(ll));
		}
	}//namespace logger
}//namespace hgl
