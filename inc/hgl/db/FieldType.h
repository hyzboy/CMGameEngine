#ifndef HGL_DB_FIELD_TYPE_INCLUDE
#define HGL_DB_FIELD_TYPE_INCLUDE

#include<hgl/type/List.h>
#include<hgl/type/StringList.h>
namespace hgl
{
    namespace db
    {
        /**
        * 字段基本类型枚举
        */
        enum FieldBaseType
        {
            fbtNone=0,

            fbtBool,

            fbtInt8,            fbtInt16,           fbtInt32,           fbtInt64,
            fbtUint8,           fbtUint16,          fbtUint32,          fbtUint64,

            fbtFloat,
            fbtDouble,

            fbtChar16le,        //定长字符串utf16le
            fbtChar8,           //定长字符串utf8

            fbtBinary,

            fbtVarChar16le,     //非定长字符串utf16le
            fbtVarChar8,        //非定长字符串utf8

            fbtVarBinary,       //非定长二进制

            fbtEnd
        };

        /**
        * 字段类型
        */
        struct FieldType                //字段信息
        {
            FieldBaseType   base;       //基本类型

            int count;                  //个数,为0表示可变长度
            int size;                   //所占字节数

        public:

            FieldType()
            {
                base=fbtNone;
                count=size=0;
            }

            FieldType(const FieldBaseType &bt,int c=1);
        };//struct FieldType

        using FieldTypeList=List<FieldType>;

        FieldType ParseFieldType(const char *str);
        FieldType ParseFieldType(const u16char *str);

        bool ParseFieldType(FieldTypeList &,const UTF16StringList &);
    }//namespace db
}//namespace hgl
#endif//HGL_DB_FIELD_TYPE_INCLUDE
