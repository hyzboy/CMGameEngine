#include"Loader.h"
#include"ods_xml_parse.h"
#include<zip.h>

class LoaderFODS:public Loader
{
protected:

    ODSParse ods;

    const UTF16StringList *name_list;
    const UTF16StringList *typename_list;
    FieldTypeList type_list;

    int use_col_count;
    int record_count;

public:

    LoaderFODS()
    {
        ods.Start();
    }

    ~LoaderFODS()
    {
    }

public:

    virtual bool Load(const OSString &filename,const CharSet &)
    {
        if(!XMLParseFile(&ods,filename))
            return(false);

        PrevProcess();

        return(true);
    }

    void PrevProcess()
    {
        name_list=ods.GetRecord(0);
        typename_list=ods.GetRecord(1);

        ParseFieldType(type_list,*typename_list);
        use_col_count=(name_list->GetCount()>typename_list->GetCount()?typename_list->GetCount():name_list->GetCount());

        record_count=ods.GetRowCount()-2;
    }

    const UTF16StringList &GetFieldNameList(){return *name_list;}
    const FieldTypeList &GetFieldTypeList(){return type_list;}
    const UTF16StringList &GetFieldTypeNameList(){return *typename_list;}

    int        GetFieldCount(){return use_col_count;}
    int        GetRecordCount(){return record_count;}

    UTF16StringList *GetRecord(int row)
    {
        if(row<0||row>=record_count)return(nullptr);

        row+=2;

        return ods.GetRecord(row);
    }

    void Release(UTF16StringList *)
    {
    }
};//class LoaderFODS

Loader *CreateLoaderFODS()
{
    return(new LoaderFODS);
}

class LoaderODS:public LoaderFODS
{
public:

    virtual bool Load(const OSString &filename,const CharSet &)
    {
        int errorp;

        struct zip *zpack=zip_open(filename.c_str(),0,&errorp);

        if(!zpack)
            return(false);

        struct zip_stat zs;

        if(zip_stat(zpack,"content.xml",0,&zs))
        {
            zip_close(zpack);
            return(false);
        }

        struct zip_file *zfp=zip_fopen_index(zpack,zs.index,0);

        if(!zfp)
        {
            zip_close(zpack);
            return(false);
        }

        SharedArray<char> filedata=new char[zs.size];

        zip_fread(zfp,filedata,zs.size);
        zip_fclose(zfp);

        if(!ods.Parse(filedata,zs.size,true))
            return(false);

        PrevProcess();

        return(true);
    }
};//class LoaderODS

Loader *CreateLoaderODS()
{
    return(new LoaderODS);
}
