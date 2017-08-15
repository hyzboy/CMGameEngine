#ifndef HGL_DB_MYSQL_FIELD_INCLUDE
#define HGL_DB_MYSQL_FIELD_INCLUDE

#include<hgl/db/sql/MySQLDriver.h>
#include<hgl/db/sql/SQLField.h>
namespace hgl
{
	namespace sql
	{
		namespace mysql
		{
			class MySQLField:public SQLField
			{
			protected:

				MYSQL_FIELD *m_field;

			public:

				MySQLField(MYSQL_FIELD *field);
				const char *GetFieldName( );
				const char *GetFieldDefValue( );
				void GetFieldInfo( );
				//void GetMysqlRoleLiteInfo( );
				virtual ~MySQLField(){};
			};//class MySQLField
		}//namespace mysql
	}//namespace sql
}//namespace hgl
#endif//HGL_DB_MYSQL_FIELD_INCLUDE
