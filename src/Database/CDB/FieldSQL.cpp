#include<hgl/db/Field.h>
#include"FieldData.h"

namespace hgl
{
    namespace db
    {
        bool ToSQLString(UTF8String &str,const FieldData *field)
        {
            if(!field)return(false);

            switch(field->GetBaseType())
            {
                case fbtBool:        str=(((FieldDataBool *)field)->value?'1':'0');return(true);

                case fbtInt8:        str=((FieldDataInteger<int8> *)field)->value;return(true);
                case fbtInt16:        str=((FieldDataInteger<int16> *)field)->value;return(true);
                case fbtInt32:        str=((FieldDataInteger<int32> *)field)->value;return(true);
                case fbtInt64:        str=((FieldDataInteger<int64> *)field)->value;return(true);

                case fbtUint8:        str=((FieldDataUInteger<uint8> *)field)->value;return(true);
                case fbtUint16:        str=((FieldDataUInteger<uint16> *)field)->value;return(true);
                case fbtUint32:        str=((FieldDataUInteger<uint32> *)field)->value;return(true);
                case fbtUint64:        str=((FieldDataUInteger<uint64> *)field)->value;return(true);

                case fbtFloat:        str=((FieldDataFloat<float> *)field)->value;return(true);
                case fbtDouble:        str=((FieldDataFloat<double> *)field)->value;return(true);

                case fbtChar16le:    str='\''+to_u8(((FieldDataUTF16Chars *)field)->array_value.GetData(),field->GetCount())+'\'';return(true);
                case fbtChar8:        str='\'';str.Strcat(((FieldDataUTF8Chars *)field)->array_value.GetData(),field->GetCount());str+='\'';return(true);
                case fbtBinary:        str='\'';str.Strcat(((FieldDataBinary     *)field)->array_value.GetData(),field->GetCount());str+='\'';return(true);

                case fbtVarChar16le:str='\'';str+=to_u8(((FieldDataUTF16String *)field)->string_value);str+='\'';return(true);
                case fbtVarChar8:    str='\'';str+=((FieldDataUTF8String *)field)->string_value;str+='\'';return(true);

                case fbtVarBinary:
                {
                    FieldDataVarBinary *vb=(FieldDataVarBinary *)field;

                    str.Strcpy(vb->mem_block.GetData(),vb->mem_block.GetBytes());
                    return(true);
                }

                default:            return(false);
            }
        }

        bool ParseSQLString(FieldData *field,const UTF8String &str)
        {
            if(!field)return(false);

            switch(field->GetBaseType())
            {
                case fbtBool:        return str.ToBool(((FieldDataBool *)field)->value);

                case fbtInt8:        return str.ToInt<int8>(((FieldDataInteger<int8> *)field)->value);
                case fbtInt16:        return str.ToInt<int16>(((FieldDataInteger<int16> *)field)->value);
                case fbtInt32:        return str.ToInt<int32>(((FieldDataInteger<int32> *)field)->value);
                case fbtInt64:        return str.ToInt<int64>(((FieldDataInteger<int64> *)field)->value);

                case fbtUint8:        return str.ToUint<uint8>(((FieldDataUInteger<uint8> *)field)->value);
                case fbtUint16:        return str.ToUint<uint16>(((FieldDataUInteger<uint16> *)field)->value);
                case fbtUint32:        return str.ToUint<uint32>(((FieldDataUInteger<uint32> *)field)->value);
                case fbtUint64:        return str.ToUint<uint64>(((FieldDataUInteger<uint64> *)field)->value);

                case fbtFloat:        return str.ToFloat<float>(((FieldDataFloat<float> *)field)->value);
                case fbtDouble:        return str.ToFloat<double>(((FieldDataFloat<double> *)field)->value);

                case fbtChar16le:    u8_to_u16(((FieldDataUTF16Chars *)field)->array_value.GetData(),field->GetCount(),str.c_str(),str.Length());return(true);
                case fbtChar8:        memcpy(((FieldDataUTF8Chars *)field)->array_value.GetData(),str.c_str(),hgl_min(str.Length()+1,field->GetCount()));return(true);
                case fbtBinary:        memcpy(((FieldDataBinary     *)field)->array_value.GetData(),str.c_str(),hgl_min(str.Length()+1,field->GetCount()));return(true);

                case fbtVarChar16le:((FieldDataUTF16String *)field)->string_value=to_u16(str);return(true);
                case fbtVarChar8:    ((FieldDataUTF8String *)field)->string_value=str;return(true);

                case fbtVarBinary:
                {
                    FieldDataVarBinary *vb=(FieldDataVarBinary *)field;

                    vb->mem_block.SetLength(str.Length()+1);

                    memcpy(vb->mem_block.GetData(),str.c_str(),str.Length()+1);
                    return(true);
                }

                default:            return(false);
            }
        }
    }//namespace db
}//namespace hgl
