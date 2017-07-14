#ifndef HGL_DB_SQL_DRIVER_INCLUDE
#define HGL_DB_SQL_DRIVER_INCLUDE

#include<hgl/db/sql/SQLField.h>

namespace hgl
{
	/**
	* SQL 支持模块名字空间
	*/
	namespace sql
	{
		class SQLRow
		{
		public:

			virtual ~SQLRow(){}

		public:

			virtual char *GetString( int index ) = 0;
		
		};//class SQLRow

		/**
		* SQL 查询结果支持
		*/
		class SQLStoreResult
		{	
		public:

			virtual ~SQLStoreResult(){}

		public:

			virtual int GetRowNumber()=0;																///<取得记录行数
			virtual int GetFieldCount()=0;															///<取得记录字段数
			virtual const char *GetFieldName(int)=0;												///<取得字段名称

			virtual SQLRow *GetRow(int) = 0;
			virtual SQLField *GetField(int) = 0;
			//virtual MYSQL_FIELD *GetField(int) = 0;

		};//class SQLStoreResult

		/**
		* SQL 连接支持
		*/
		class SQLConnection
		{
		public:

			virtual ~SQLConnection(){};

			virtual SQLStoreResult *Query(const char *)=0;													///<执行一个SQL查询
			virtual void GetMysqlRoleLiteInfo( ) = 0;
		};//SQLConnection

		/**
		* SQL 驱动类
		*/
		class SQLDriver
		{
		public:

			virtual ~SQLDriver(){};

			virtual SQLConnection *CreateConnection(const char *,const char *,const char *,const char *,const char *charset=nullptr)=0;				///<创建一个访问连接
		};//class SQLDriver

		/**
		* SQL类型
		*/
		enum SQLType
		{
			sqlNone=0,

			sqlMySQL,
			//sqlMySQLEmbedded,
			//sqlFireBird,
			//sqlFireBirdEmbedded,
			//sqlInterBase,
			//sqlInterBaseEmbedded,
			//sqlSQLite,

			sqlEnd

		};//enum SQLType

		SQLDriver *CreateSQLDriver(SQLType);														///<创建一个SQL驱动
	}//namespace sql
}//namespace hgl
#endif//HGL_DB_SQL_DRIVER_INCLUDE
