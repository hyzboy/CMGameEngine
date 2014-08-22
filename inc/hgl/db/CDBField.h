#ifndef HGL_CDB_FIELD_INCLUDE
#define HGL_CDB_FIELD_INCLUDE

#include<hgl/db/FieldType.h>
namespace hgl
{
	namespace io
	{
		class DataInputStream;
	}//namespace io

	namespace db
	{
		/**
		* CDB字段访问类
		*/
		class CDBField
		{
		protected:

			int data_count;

		public:

			UTF16String Name;																		///<字段名称
			FieldType Type;																			///<字段类型

		public:

			CDBField(const UTF16String &,const FieldType &);
			virtual ~CDBField()HGL_DEFAULT_MEMFUNC;

			virtual bool	ToString	(int,UTF16String &)=0;										///<转换为字符串

			virtual bool	GetBool		(int,bool &		){return false;}							///<从字段中取得一个布尔型数
			virtual bool	GetInteger	(int,int &		){return false;}							///<从字段中取得一个有符号整型数
			virtual bool	GetUInteger	(int,uint &		){return false;}							///<从字段中取得一个无符号整型数
			virtual bool	GetFloat	(int,float &	){return false;}							///<从字段中取得一个浮点数
			virtual bool	GetDouble	(int,double &	){return false;}							///<从字段中取得一个浮点数
			virtual bool	GetChar		(int,char &		){return false;}							///<从字段中取得一个utf8字符
			virtual bool	GetChar		(int,u16char &	){return false;}							///<从字段中取得一个utf16字符

			virtual bool	GetArray	(int,bool *,	int=-1,int=-1){return false;}				///<从字段中取得一组布尔型数
			virtual bool	GetArray	(int,int8 *,	int=-1,int=-1){return false;}				///<从字段中取得一组8位有符号整型数
			virtual bool	GetArray	(int,int16 *,	int=-1,int=-1){return false;}				///<从字段中取得一组16位有符号整型数
			virtual bool	GetArray	(int,int32 *,	int=-1,int=-1){return false;}				///<从字段中取得一组32位有符号整型数

			virtual bool	GetArray	(int,uint8 *,	int=-1,int=-1){return false;}				///<从字段中取得一组8位有符号整型数
			virtual bool	GetArray	(int,uint16 *,	int=-1,int=-1){return false;}				///<从字段中取得一组16位有符号整型数
			virtual bool	GetArray	(int,uint32 *,	int=-1,int=-1){return false;}				///<从字段中取得一组32位有符号整型数

			virtual bool	GetArray	(int,float *,	int=-1,int=-1){return false;}				///<从字段中取得一组浮点数
			virtual bool	GetArray	(int,double *,	int=-1,int=-1){return false;}				///<从字段中取得一组浮点数
			virtual bool	GetArray	(int,char *,	int=-1,int=-1){return false;}				///<从字段中取得一组utf8字符
			virtual bool	GetArray	(int,u16char *,int=-1,int=-1){return false;}				///<从字段中取得一组utf16字符

			virtual bool	GetString	(int,UTF8String &){return false;}							///<从字段中取得一个utf8字符串
			virtual bool	GetString	(int,UTF16String &){return false;}							///<从字段中取得一个utf16字符串

			virtual int		FindInteger	(const int 		){return -1;}
			virtual int		FindUInteger(const uint 	){return -1;}
			virtual int		FindFloat	(const float 	){return -1;}
			virtual int		FindDouble	(const double 	){return -1;}
			virtual int		FindChar	(const char		){return -1;}
			virtual int		FindChar	(const u16char	){return -1;}

			virtual	int		FindArray(const int8 *		){return -1;}
			virtual	int		FindArray(const int16 *		){return -1;}
			virtual	int		FindArray(const int32 *		){return -1;}

			virtual	int		FindArray(const uint8 *		){return -1;}
			virtual	int		FindArray(const uint16 *	){return -1;}
			virtual	int		FindArray(const uint32 *	){return -1;}

			virtual	int		FindArray(const float *		){return -1;}
			virtual	int		FindArray(const double *	){return -1;}
			virtual int		FindArray(const char *		){return -1;}								///<查找utf8字符陈列，不处理0结尾
			virtual int		FindArray(const u16char *	){return -1;}								///<查找utf16字符陈列，不处理0结尾

			virtual int		FindCharArray(const char *){return -1;}									///<查找utf8字符阵列，处理0结尾
			virtual int		FindCharArray(const u16char *){return -1;}								///<查找utf16字符阵列，处理0结尾
		};//class CDBField

		/**
		* 创建一个CDB字段
		* @param name 字段名称
		* @param ft 字段类型
		* @param str 字段数据输入流
		* @param count 字段数据输入个数
		* @return 字段指针
		*/
		CDBField *CreateCDBField(const UTF16String &name,const FieldType &ft,io::DataInputStream *dis=nullptr,int count=0);
	}//namespace db
}//namespace hgl
#endif//HGL_CDB_FIELD_INCLUDE
