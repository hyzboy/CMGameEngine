#ifndef HGL_DB_MYSQL_ROW_INCLUDE
#define HGL_DB_MYSQL_ROW_INCLUDE

#include<hgl/db/sql/MySQLDriver.h>
namespace hgl
{
    namespace sql
    {
        namespace mysql
        {
            class MySQLRow:public SQLRow
            {
            protected:

                MYSQL_ROW row;

            public:

                MySQLRow(MYSQL_ROW);
                char *GetString( int index );
                virtual ~MySQLRow(){};
            };//class MySQLRow
        }//namespace mysql
    }//namespace sql
}//namespace hgl
#endif//HGL_DB_MYSQL_ROW_INCLUDE
