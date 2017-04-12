#include"FieldData.h"

namespace hgl
{
    using namespace io;

    namespace db
    {
        FieldData *CreateFieldData(const FieldDesc *field_desc)
        {
            if(!field_desc)return(nullptr);

            switch(field_desc->type.base)
            {
                case fbtBool:        return(new FieldDataBool(field_desc));

                case fbtInt8:        return(new FieldDataInteger<int8>(field_desc));
                case fbtInt16:        return(new FieldDataInteger<int16>(field_desc));
                case fbtInt32:        return(new FieldDataInteger<int32>(field_desc));
                case fbtInt64:        return(new FieldDataInteger<int64>(field_desc));

                case fbtUint8:        return(new FieldDataUInteger<uint8>(field_desc));
                case fbtUint16:        return(new FieldDataUInteger<uint16>(field_desc));
                case fbtUint32:        return(new FieldDataUInteger<uint32>(field_desc));
                case fbtUint64:        return(new FieldDataUInteger<uint64>(field_desc));

                case fbtFloat:        return(new FieldDataFloat<float>(field_desc));
                case fbtDouble:        return(new FieldDataFloat<double>(field_desc));

                case fbtChar16le:    return(new FieldDataUTF16Chars(field_desc));
                case fbtChar8:        return(new FieldDataUTF8Chars(field_desc));
                case fbtBinary:        return(new FieldDataBinary(field_desc));

                case fbtVarChar16le:return(new FieldDataUTF16String(field_desc));
                case fbtVarChar8:    return(new FieldDataUTF8String(field_desc));
                case fbtVarBinary:    return(new FieldDataVarBinary(field_desc));

                default:            return(nullptr);
            }
        }
    }//namespace db
}//namespace hgl
