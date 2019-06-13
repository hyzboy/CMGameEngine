#ifndef HGL_DB_MYSQL_CONNECTION_INCLUDE
#define HGL_DB_MYSQL_CONNECTION_INCLUDE

#include<hgl/db/sql/MySQLDriver.h>
namespace hgl
{
    namespace sql
    {
        namespace mysql
        {
            class MySQLConnection:public SQLConnection
            {
                MYSQL *mysql;

            public:

                MySQLConnection(MYSQL *);
                virtual ~MySQLConnection();

                SQLStoreResult *Query(const char *);
                void GetMysqlRoleLiteInfo( );
            };//class MySQLConnection
        }//namespace mysql
    }//namespace sql
}//namespace hgl
#endif//HGL_DB_MYSQL_CONNECTION_INCLUDE
