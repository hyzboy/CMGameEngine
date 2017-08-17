#include<hgl/db/sql/MySQLConnection.h>
#include<hgl/db/sql/MySQLStoreResult.h>
namespace hgl
{
	namespace sql
	{
		namespace mysql
		{
			MySQLConnection::MySQLConnection(MYSQL *ms)
			{
				mysql=ms;
			}

			MySQLConnection::~MySQLConnection()
			{
				if(!mysql)
				{
					mysql_close(mysql);
					free( mysql );
				}
			}

			SQLStoreResult *MySQLConnection::Query(const char *sql)
			{
				if(mysql_query(mysql,sql))
					return(nullptr);

				MYSQL_RES *query=mysql_store_result(mysql);

				if(!query)
					return(nullptr);

				return(new MySQLStoreResult(query));
			}

			void MySQLConnection::GetMysqlRoleLiteInfo( )
			{
				MY_CHARSET_INFO cs;    
				mysql_get_character_set_info(mysql, &cs);    
				printf("character set information:\n");    
				printf("character set name: %s\n", cs.name);    
				printf("collation name: %s\n", cs.csname);    
				printf("comment: %s\n", cs.comment);    
				printf("directory: %s\n", cs.dir);    
				printf("multi byte character min. length: %d\n", cs.mbminlen);    
				printf("multi byte character max. length: %d\n", cs.mbmaxlen);
				printf("New client character set: %s\n", mysql_character_set_name(mysql));
			}
		}//namespace mysql
	}//namespace sql
}//namespace hglC
