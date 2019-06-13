#ifndef HGL_DB_MYSQL_STORE_RESULT_INCLUDE
#define HGL_DB_MYSQL_STORE_RESULT_INCLUDE

#include<hgl/db/sql/MySQLDriver.h>
#include<hgl/db/sql/SQLField.h>
namespace hgl
{
    namespace sql
    {
        namespace mysql
        {
            class MySQLStoreResult:public SQLStoreResult
            {
            protected:

                MYSQL_FIELD *fields;
                int rows;
                int field_count;
                MYSQL_RES *m_query;
                MYSQL_ROW *m_row_result;

            public:

                MySQLStoreResult(MYSQL_RES *mr);
                virtual ~MySQLStoreResult();

                int GetRowNumber();
                int GetFieldCount();
                const char *GetFieldName(int);

                SQLRow *GetRow(int);
                SQLField *GetField(int);
                //MYSQL_FIELD *GetField(int);
            };//class MySQLStoreResult
        }//namespace mysql
    }//namespace sql
}//namespace hgl
#endif//HGL_DB_MYSQL_STORE_RESULT_INCLUDE
