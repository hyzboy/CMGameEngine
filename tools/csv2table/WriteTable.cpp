#include"Common.h"
#include<hgl/type/List.h>
#include<hgl/db/FieldType.h>

using namespace hgl;
using namespace hgl::db;

bool WriteBool(void *buf,int max_count,const UTF16String &str)
{
    UTF16StringList sl;

    ParseField(&sl,str,u',');

    int count=sl.GetCount();
    bool *p=(bool *)buf;

    if(count>max_count)
        count=max_count;

    if(count<=0)return(false);

    for(int i=0;i<count;i++)
        sl[i].ToBool(*p++);

    return(true);
}

template<typename T> bool WriteNumber(void *buf,int max_count,const UTF16String &str)
{
    parse_float_array(str.c_str(),(T *)buf,max_count);return(true);
}

bool WriteUTF16LE(void *buf,int max_count,const UTF16String &str)
{
    int count=str.Length();

    if(count>max_count)
        count=max_count;

    if(count<=0)return(false);

    if((*(str.c_str())==u'"')
     &&(str.GetEndChar()==u'"'))
    {
        hgl_typecpy((u16char *)buf,str.c_str()+1,count-2);    //去掉前后的引号
    }
    else
    {
        hgl_typecpy((u16char *)buf,str.c_str(),count);
    }

    return(true);
}

bool WriteUTF8(void *buf,int max_count,const UTF16String &str)
{
    int count=str.Length();

    if(count>max_count)
        count=max_count;

    if(count<=0)return(false);

    UTF8String u8str=to_u8(str);

    if((*(u8str.c_str())=='"')
     &&(u8str.GetEndChar()=='"'))
    {
        memcpy(buf,u8str.c_str()+1,count-2);        //去掉前后的引号
    }
    else
    {
        memcpy(buf,u8str.c_str(),count);
    }

    return(true);
}

typedef bool (*WriteFieldFunc)(void *,int,const UTF16String &);

WriteFieldFunc WriteField[fbtEnd];

void InitWriteField()
{
    memset(WriteField,0,fbtEnd*sizeof(WriteFieldFunc));

    WriteField[fbtBool    ]=WriteBool;

    WriteField[fbtInt8    ]=WriteNumber<int8>;
    WriteField[fbtInt16    ]=WriteNumber<int16>;
    WriteField[fbtInt32    ]=WriteNumber<int32>;
    WriteField[fbtInt64    ]=WriteNumber<int64>;

    WriteField[fbtUint8    ]=WriteNumber<uint8>;
    WriteField[fbtUint16]=WriteNumber<uint16>;
    WriteField[fbtUint32]=WriteNumber<uint32>;
    WriteField[fbtUint64]=WriteNumber<uint64>;

    WriteField[fbtFloat    ]=WriteNumber<float>;
    WriteField[fbtDouble]=WriteNumber<double>;

    //限定仅使用parse_float_array，因为excel等工具在表示极小或极大数时，会使用指数形式

    WriteField[fbtChar16le    ]=WriteUTF16LE;
    WriteField[fbtChar8        ]=WriteUTF8;
}

bool WriteFieldData(const FieldType &ct,void *buf,UTF16String &str)
{
    if(ct.base<=fbtNone||ct.base>=fbtEnd)return(false);

    if(str.Length()<=0)
        return(false);

    memset(buf,0,ct.size);

    if(!WriteField[ct.base])
        return false;

    if(str[0]==u'"'&&str.GetEndChar()==u'"')    //首尾字符为引号
    {
        str.SubString(1,str.Length()-2);        //截取指定字符串为子串
    }

    return WriteField[ct.base](buf,ct.count,str);
}
