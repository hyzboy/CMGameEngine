#ifndef HGL_DB_MYSQL_DRIVER_INCLUDE
#define HGL_DB_MYSQL_DRIVER_INCLUDE

#include<hgl/db/sql/SQLDriver.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<mysql/mysql.h>

namespace hgl
{
	namespace sql
	{
		/**
		* MySQL支持模块名字空间
		*/
		namespace mysql
		{
			/**
			* MySQL驱动支持类
			*/
			class MySQLDriver:public SQLDriver
			{
			public:

				MySQLDriver();
				virtual ~MySQLDriver();

				virtual SQLConnection *CreateConnection(const char *,const char *,const char *,const char *,const char *);			///<创建一个访问连接
			};//class MySQLDriver
		}//namespace mysql
	}//namespace sql
}//namespace hgl
#endif//HGL_DB_MYSQL_DRIVER_INCLUDE
