#include<hgl/Logger.h>
#include<windows.h>

namespace hgl
{
	namespace logger
	{
		class LogWinDialog:public Logger
		{
			UTF16String name;

			char16_t buf[4096];

		public:

			LogWinDialog(LogLevel ll):Logger(ll)
			{
				name=project_code;
			}

			bool Create(const UTF16String &)
			{
				return(true);
			}

			void Close()
			{
			}

			void Write(const char16_t *str,int)
			{
				MessageBoxW(nullptr,str,name,MB_OK);
			}

			void Write(const char *str,int size)
			{
				const int len=u8_to_u16(buf,4096,str,size);

				if(len<=0)return;

				MessageBoxW(nullptr,buf,name,MB_OK);
			}
		};//class LogWinDialog

		Logger *CreateLoggerDialog(const UTF16String &,LogLevel ll)
		{
			return(new LogWinDialog(ll));
		}
	}//namespace logger
}//namespace hgl
