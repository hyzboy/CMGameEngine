#include<hgl/db/Field.h>
#include"FieldData.h"

namespace hgl
{
    namespace db
    {
        bool Write(hgl::io::DataOutputStream *dos,hgl::db::FieldData *field_data)
        {
            if(!dos||!field_data)return(false);

            switch(field_data->GetBaseType())
            {
                case fbtBool:        return dos->WriteBool    (((FieldDataBool *)field_data)->value);

                case fbtInt8:        return dos->WriteInt8    (((FieldDataInteger    <int8    > *)field_data)->value);
                case fbtInt16:        return dos->WriteInt16    (((FieldDataInteger    <int16    > *)field_data)->value);
                case fbtInt32:        return dos->WriteInt32    (((FieldDataInteger    <int32    > *)field_data)->value);
                case fbtInt64:        return dos->WriteInt64    (((FieldDataInteger    <int64    > *)field_data)->value);

                case fbtUint8:        return dos->WriteUint8    (((FieldDataUInteger<uint8    > *)field_data)->value);
                case fbtUint16:        return dos->WriteUint16    (((FieldDataUInteger<uint16    > *)field_data)->value);
                case fbtUint32:        return dos->WriteUint32    (((FieldDataUInteger<uint32    > *)field_data)->value);
                case fbtUint64:        return dos->WriteUint64    (((FieldDataUInteger<uint64    > *)field_data)->value);

                case fbtFloat:        return dos->WriteFloat    (((FieldDataFloat    <float    > *)field_data)->value);
                case fbtDouble:        return dos->WriteDouble    (((FieldDataFloat    <double    > *)field_data)->value);

                case fbtChar16le:    return dos->WriteUTF16Char(((FieldDataUTF16Chars *)field_data)->array_value.GetData(),((FieldDataUTF16Chars *)field_data)->GetCount());
                case fbtChar8:        return dos->WriteFully(((FieldDataUTF8Chars *)field_data)->array_value.GetData(),((FieldDataUTF8Chars *)field_data)->GetCount())==((FieldDataUTF8Chars *)field_data)->GetCount();
                case fbtBinary:        return dos->WriteFully(((FieldDataBinary *)field_data)->array_value.GetData(),((FieldDataBinary *)field_data)->GetCount())==((FieldDataBinary *)field_data)->GetCount();

                case fbtVarChar16le:return dos->WriteUTF16LEString(((FieldDataUTF16String *)field_data)->string_value);
                case fbtVarChar8:    return dos->WriteUTF8String(((FieldDataUTF8String *)field_data)->string_value);
                case fbtVarBinary:    {
                                        if(!dos->WriteInt32(((FieldDataVarBinary *)field_data)->mem_block.GetBytes()))return(false);

                                        return dos->WriteFully(((FieldDataVarBinary *)field_data)->mem_block.GetData(),((FieldDataVarBinary *)field_data)->mem_block.GetBytes());
                                    }

                default:return(false);
            }
        }

        bool Read(hgl::io::DataInputStream *dis,hgl::db::FieldData *field_data)
        {
            if(!dis||!field_data)return(false);

            switch(field_data->GetBaseType())
            {
                case fbtBool:        return dis->ReadBool    (((FieldDataBool *)field_data)->value);

                case fbtInt8:        return dis->ReadInt8    (((FieldDataInteger    <int8    > *)field_data)->value);
                case fbtInt16:        return dis->ReadInt16    (((FieldDataInteger    <int16    > *)field_data)->value);
                case fbtInt32:        return dis->ReadInt32    (((FieldDataInteger    <int32    > *)field_data)->value);
                case fbtInt64:        return dis->ReadInt64    (((FieldDataInteger    <int64    > *)field_data)->value);

                case fbtUint8:        return dis->ReadUint8    (((FieldDataUInteger<uint8    > *)field_data)->value);
                case fbtUint16:        return dis->ReadUint16    (((FieldDataUInteger<uint16    > *)field_data)->value);
                case fbtUint32:        return dis->ReadUint32    (((FieldDataUInteger<uint32    > *)field_data)->value);
                case fbtUint64:        return dis->ReadUint64    (((FieldDataUInteger<uint64    > *)field_data)->value);

                case fbtFloat:        return dis->ReadFloat    (((FieldDataFloat    <float    > *)field_data)->value);
                case fbtDouble:        return dis->ReadDouble    (((FieldDataFloat    <double    > *)field_data)->value);

                case fbtChar16le:    return dis->ReadUTF16Char(((FieldDataUTF16Chars *)field_data)->array_value.GetData(),((FieldDataUTF16Chars *)field_data)->GetCount());
                case fbtChar8:        return dis->ReadFully(((FieldDataUTF8Chars *)field_data)->array_value.GetData(),((FieldDataUTF8Chars *)field_data)->GetCount())==((FieldDataUTF8Chars *)field_data)->GetCount();
                case fbtBinary:        return dis->ReadFully(((FieldDataBinary *)field_data)->array_value.GetData(),((FieldDataBinary *)field_data)->GetCount())==((FieldDataBinary *)field_data)->GetCount();

                case fbtVarChar16le:
                {
                    FieldDataUTF16String *vc_field=(FieldDataUTF16String *)field_data;

                    return dis->ReadUTF16LEString(vc_field->string_value,vc_field->GetCount());
                }
                case fbtVarChar8:
                {
                    FieldDataUTF8String *vc_field=(FieldDataUTF8String *)field_data;

                    return dis->ReadUTF8String(vc_field->string_value,vc_field->GetCount());
                }
                case fbtVarBinary:
                {
                    int32 bytes;

                    if(!dis->ReadInt32(bytes))return(false);

                    ((FieldDataVarBinary *)field_data)->mem_block.SetLength(bytes);

                    return dis->ReadFully(((FieldDataVarBinary *)field_data)->mem_block.GetData(),bytes)==bytes;
                }

                default:return(false);
            }
        }
    }//namespace db
}//namespace hgl
