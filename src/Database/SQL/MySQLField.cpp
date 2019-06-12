#include<hgl/db/sql/MySQLField.h>

namespace hgl
{
    namespace sql
    {
        namespace mysql
        {
            MySQLField::MySQLField(MYSQL_FIELD *field)
            {
                m_field = field;
            }

            const char *MySQLField::GetFieldName( )
            {
                return m_field->name;
            }

            const char *MySQLField::GetFieldDefValue( )
            {
                return m_field->def;
            }

            void MySQLField::GetFieldInfo( )
            {
                printf( "Table of column if column was a field is %s\n", m_field->table );
                printf( "Type of field. Se mysql_com.h for types is %d\n", m_field->type );
                printf( "Width of column is %ld\n", m_field->length );
                printf( "Max width of selected set is %ld\n", m_field->max_length );
                printf( "Div flags is %d\n", m_field->flags );
                printf( "Number of decimals in field is %d\n", m_field->decimals );
            }

            /*void MySQLField::GetMysqlRoleLiteInfo( )
            {
                MY_CHARSET_INFO cs;
                mysql_get_character_set_info(&mysql, &cs);
                printf("character set information:\n");
                printf("character set name: %s\n", cs.name);
                printf("collation name: %s\n", cs.csname);
                printf("comment: %s\n", cs.comment);
                printf("directory: %s\n", cs.dir);
                printf("multi byte character min. length: %d\n", cs.mbminlen);
                printf("multi byte character max. length: %d\n", cs.mbmaxlen);
            }*/
        }//namespace mysql
    }//namespace sql
}//namespace hgl
