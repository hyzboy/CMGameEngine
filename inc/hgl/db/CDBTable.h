#ifndef HGL_CDB_TABLE_INCLUDE
#define HGL_CDB_TABLE_INCLUDE

#include<hgl/type/Map.h>
#include<hgl/type/StringList.h>
#include<hgl/db/CDBField.h>
namespace hgl
{
	namespace io
	{
		class DataInputStream;
	}//namespace io

	namespace db
	{
		/**
		* CDB表管理类
		*/
		class CDBTable																				///CDB表格类
		{
			int data_count;																			///<数据数量

			void InitPrivate();

		public:	//属性

			MapObject<UTF16String,CDBField> Fields;												///<字段列表

			const int GetDataCount()const{return data_count;}										///<取得数据数量

		public:

			CDBTable();																				///<本类构造函数
			~CDBTable()=default;																	///<本类析构函数

			bool Load(io::DataInputStream *);														///<从流中加载一个表
			bool Load(const OSString &);															///<从文件中加载一个表
		};//class Table
	}//namespace db
}//namespace hgl
#endif//HGL_CDB_TABLE_INCLUDE
