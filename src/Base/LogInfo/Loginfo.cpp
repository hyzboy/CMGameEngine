#include<hgl/PlugIn.h>
#include<hgl/Logger.h>
#include<hgl/type/DateTime.h>

namespace hgl
{
	namespace logger
	{
		static List<Logger *> log_list;		//记录器列表

		bool AddLogger(Logger *log)
		{
			if(!log)
				return(false);

			if(log_list.Find(log)!=-1)		//重复添加
				return(false);

			log_list.Add(log);
			return(true);
		}

		template<typename T>
		void WriteLog(LogLevel level,const T *str,int size)
		{
			const int n=log_list.GetCount();

			if(n<=0)return;

			Logger *log;

			for(int i=0;i<n;i++)
			{
				log=log_list[i];

				if(log==nullptr)continue;

				if(log->GetLevel()>=level)
					log->Write(str,size);
			}
		}

		bool PutLogHeader()
		{
			if(log_list.GetCount()<=0)
				return(false);

			Date d;
			Time t;

			d.Sync();
			t.Sync();

			const OSString str=	OS_TEXT("Create Log Date/Time: ")+
								OSString(d.GetYear	())+OS_TEXT("-")+
								OSString(d.GetMonth	())+OS_TEXT("-")+
								OSString(d.GetDay	())+OS_TEXT(" ")+
								OSString(t.GetHour	())+OS_TEXT(":")+
								OSString(t.GetMinute())+OS_TEXT(":")+
								OSString(t.GetSecond())+OS_TEXT("\n");

			WriteLog(llLog,str.c_str(),str.Length());

			return(true);
		}

		void CloseAllLog()
		{
			const int n=log_list.GetCount();

			if(n<=0)return;

			Logger *log;

			for(int i=0;i<n;i++)
			{
				log=log_list[i];

				log->Close();
			}

			log_list.Clear();
		}
	}//namespace logger

	namespace logger
	{
		/**
		 * 日志插件接口
		 */
		struct LogInterface
		{
			bool (*Add)(Logger *);																		///<增加一个日志输出

			bool (*Init)();																				///<初始化日志输出
			void (*Close)();																			///<关闭所有日志输出

			void (*WriteUTF16)(LogLevel,const char16_t *,int);											///<输出一行日志
			void (*WriteUTF8)(LogLevel,const char *,int);												///<输出一行日志
		};//struct LogInterface

		static LogInterface LogInterface3=
		{
			AddLogger,

			PutLogHeader,
			CloseAllLog,

			WriteLog<char16_t>,
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
		class LogPlugIn:public PlugIn																	///日志插件
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

		void Log(LogLevel level,const char16_t *str,int size)
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
}//namespace hgl
