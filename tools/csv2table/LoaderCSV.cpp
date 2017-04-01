#include"Loader.h"
#include"Common.h"
#include<hgl/type/StringList.h>

using namespace hgl;

class LoaderCSV:public Loader
{
    UTF16StringList sl;
    UTF16StringList name_list;
    UTF16StringList typename_list;
    FieldTypeList type_list;

    int record_count;
    int use_col_count;

public:

    LoaderCSV()
    {
        record_count=0;
        use_col_count=0;
    }

    ~LoaderCSV()
    {
    }

    bool    Load(const OSString &filename,const CharSet &default_cs)
    {
        LoadStringListFromTextFile(sl,filename,default_cs);

        record_count=sl.GetCount()-2;

        if(record_count<1)return(false);

        ParseFieldName(&name_list,        sl[0]);    //解析字段名称
        ParseFieldData(&typename_list,    sl[1]);    //解析字段类型
        ParseFieldType(type_list,typename_list);

        use_col_count=(name_list.GetCount()>typename_list.GetCount()?typename_list.GetCount():name_list.GetCount());

        return(true);
    }

    const UTF16StringList &GetFieldNameList(){return name_list;}
    const FieldTypeList &GetFieldTypeList(){return type_list;}
    const UTF16StringList &GetFieldTypeNameList(){return typename_list;}

    int        GetFieldCount(){return use_col_count;}
    int        GetRecordCount(){return record_count;}

    UTF16StringList *GetRecord(int row)
    {
        if(row<0||row>=record_count)return(nullptr);

        row+=2;

        if(sl[row].Length()<=0)return(nullptr);    //空行跳过
        if(sl[row][0]==UNREAD_LINE_FRONT)return(nullptr);        //分号开头的为注释

        UTF16StringList *wsl=new UTF16StringList;

        if(ParseFieldData(wsl,sl[row])<=0)
        {
            delete wsl;
            return(nullptr);
        }

        return wsl;
    }

    void Release(UTF16StringList *wsl)
    {
        delete wsl;
    }
};//class LoaderCSV

Loader *CreateLoaderCSV()
{
    return(new LoaderCSV);
}
