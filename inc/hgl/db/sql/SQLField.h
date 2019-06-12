#ifndef HGL_DB_SQL_FIELD_INCLUDE
#define HGL_DB_SQL_FIELD_INCLUDE

namespace hgl
{
    namespace sql
    {
        /**
        * 字段数据类型枚举
        */
        enum FieldType                  ///字段数据类型枚举
        {
            sqlftNone=0,

            sqlftInteger,               ///<整数
            sqlftFloat,                 ///<浮点数
            sqlftDouble,                ///<双精度浮点型
            sqlftBool,                  ///<布尔
            sqlftChar,                  ///<字符
            sqlftString,                ///<字符串

            sqlftDate,                  ///<日期
            sqlftTime,                  ///<时间
            sqlftDateTime,              ///<日期时间

            sqlftBinary,                ///<二进制数据

            sqlftEnd
        };//enum FieldType

        class SQLField
        {
        public:
            virtual ~SQLField(){}
        public:
            virtual const char *GetFieldName( ) = 0;
            virtual const char *GetFieldDefValue( ) = 0;
            virtual void GetFieldInfo() = 0;
            //virtual void GetMysqlRoleLiteInfo() = 0;
        };//class SQLField
    }//namespace sql
}//namespace hgl
#endif//HGL_DB_SQL_FIELD_INCLUDE
