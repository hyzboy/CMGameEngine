#include<hgl/db/sql/MySQLRow.h>

namespace hgl
{
	namespace sql
	{
		namespace mysql
		{
			MySQLRow::MySQLRow(MYSQL_ROW mr)
			{
				row=mr;
			}

			char *MySQLRow::GetString( int index )
			{
				return row[index];
			}
		}//namespace mysql
	}//namespace sql
}//namespace hgl
