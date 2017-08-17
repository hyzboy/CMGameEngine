#include<hgl/db/sql/MySQLStoreResult.h>
#include<hgl/db/sql/MySQLRow.h>
#include<hgl/db/sql/MySQLField.h>

namespace hgl
{
	namespace sql
	{
		namespace mysql
		{
			MySQLStoreResult::MySQLStoreResult(MYSQL_RES *query)
			{
				m_query = query;
				field_count=(int)mysql_num_fields(m_query);
				rows = (int)mysql_num_rows(m_query);
				m_row_result = nullptr;

				m_row_result = (MYSQL_ROW*)malloc( rows * sizeof( MYSQL_ROW ) );

				MYSQL_ROW row;
				int n = 0;
				while( ( row = mysql_fetch_row(m_query) ) )
				{
					m_row_result[n] = row;
					n++;
				}
			}

			MySQLStoreResult::~MySQLStoreResult()
			{			
				mysql_free_result(m_query);
				free( m_row_result );
				m_query = nullptr;
				m_row_result = nullptr;
			}
			
			int MySQLStoreResult::GetRowNumber()
			{
				return rows;
			}

			int MySQLStoreResult::GetFieldCount()
			{
				return field_count;
			}

			const char *MySQLStoreResult::GetFieldName(int index)
			{
				MYSQL_FIELD *field=mysql_fetch_field_direct(m_query,index);

				if(!field)
					return(nullptr);

				return field->name;
			}

			SQLRow *MySQLStoreResult::GetRow(int row_index)
			{
				if(!m_row_result)
					return(nullptr);

				return(new MySQLRow(m_row_result[row_index]));
			}

			SQLField *MySQLStoreResult::GetField(int field_index)
			{
				MYSQL_FIELD *field = mysql_fetch_field_direct(m_query,field_index);

				if(!field)
					return(nullptr);

				return(new MySQLField(field));
			}
		}//namespace mysql
	}//namespace sql
}//namespace hgl;
