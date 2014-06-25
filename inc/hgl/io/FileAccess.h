#ifndef HGL_IO_FILE_ACCESS_INCLUDE
#define HGL_IO_FILE_ACCESS_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/io/SeekAccess.h>
#include<sys/stat.h>
namespace hgl
{
	namespace io
	{
		enum FileOpenMode                   ///文件访问模式枚举
		{
			fomNone=0,

			fomCreate,         				///<创建文件，如存在则失败
			fomCreateTrunc,					///<强制创建，如存在则抹掉
			fomOnlyRead,         			///<只读方式
			fomOnlyWrite,         			///<只写方式
			fomReadWrite,					///<可读可写
			fomAppend,						///<追加模式

			fomEnd
		};//enum FileOpenMode

		/**
		* 文件访问实例管理类
		*/
		class FileAccess																			///文件访问实例管理类
		{
		protected:

			OSString filename;

			int fp;

			struct_stat64 file_state;

			FileOpenMode mode;

		public:

			FileAccess();
			virtual ~FileAccess();

			virtual bool Open(const OSString &,FileOpenMode fom);									///<以指定模式打开一个文件
			virtual bool Create(const OSString &fn){return Open(fn,fomCreate);}						///<创建一个新文件，如文件已存在则创建失败
			virtual bool CreateTrunc(const OSString &fn){return Open(fn,fomCreateTrunc);}			///<创建一个新文件，如文件已存在则抹消它
			virtual bool OpenRead(const OSString &fn){return Open(fn,fomOnlyRead);}					///<以只读模式打开一个文件
			virtual bool OpenWrite(const OSString &fn){return Open(fn,fomOnlyWrite);}				///<以只写模式打开一个文件
			virtual bool CreateTemp();

			virtual void Close();																	///<关闭文件
			virtual void CloseRead();																///<仅关闭读取
			virtual void CloseWrite();																///<仅关闭写入

			virtual int GetFileHandle()const{return fp;}

			virtual bool CanRead()const;															///<文件是否可读
			virtual bool CanPeek()const{return(CanRead()&&CanSeek());}								///<文件是否可预读
			virtual bool CanWrite()const;															///<文件是否可写
			virtual bool CanSeek()const;															///<文件是否可定位访问指针
			virtual bool CanRestart()const{return CanSeek();}										///<文件是否可复位访问
			virtual bool CanSize()const{return(true);}												///<文件是否可取得长度

			virtual int64 Seek(int64,SeekOrigin=soBegin);											///<定位访问指针
			virtual int64 Tell()const;																///<取得访问指针位置
			virtual int64 GetSize();																///<取得文件长度
			virtual bool Restart();																	///<复位访问指针

			virtual int64 Read(void *,int64);														///<读取指定长度数据
			virtual int64 Peek(void *,int64);														///<预读指定长度数据
			virtual int64 Write(const void *,int64);												///<写入指定长度数据

			virtual int64 AvailableRead();															///<剩下的可读数据

			virtual int64 Read(int64,void *,int64);													///<在指定位置读取指定长度的数据
			virtual int64 Write(int64,const void *,int64);											///<在指定位置写入指定长度的数据
		};//class FileAccess
	}//namespace io
}//namespace hgl
#endif//HGL_IO_FILE_ACCESS_INCLUDE
