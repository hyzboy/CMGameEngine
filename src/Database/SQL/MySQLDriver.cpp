#include<hgl/db/sql/MySQLDriver.h>
#include<hgl/db/sql/MySQLConnection.h>

namespace hgl
{
	namespace sql
	{
		SQLDriver *CreateMySQLDriver()
		{
			return(new mysql::MySQLDriver());
		}

		namespace mysql
		{
			MySQLDriver::MySQLDriver()
			{
				mysql_library_init(0,nullptr,nullptr);
			}

			MySQLDriver::~MySQLDriver()
			{	
			}

			SQLConnection *MySQLDriver::CreateConnection(const char *hostname,const char *username,const char *password,const char *dbname,const char *charset_name)
			{
				if(strlen(hostname)<=7)		//IP地址不可能少于7个字符
					return(nullptr);

				MYSQL *mysql = (MYSQL*)malloc( sizeof( MYSQL ) );
				
				mysql_init(mysql);

                if(charset_name)
                    mysql_options(mysql,MYSQL_SET_CHARSET_NAME,charset_name);

				if(mysql_real_connect(mysql,hostname,username,password,dbname,3306,nullptr,0))
				{
					return(new MySQLConnection(mysql));
				}

				fprintf(stderr, "Failed to connect to database: Error: %s\n",mysql_error(mysql));

				mysql_close(mysql);
				free( mysql );
				return(nullptr);
			}
		}//namespace mysql
	}//namespace sql
}//namespace hgl
