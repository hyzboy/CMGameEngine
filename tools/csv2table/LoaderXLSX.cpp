#include"Loader.h"
#include<hgl/ut/XMLParse.h>
#include<zip.h>

class ContentTypesParse:public XMLParse
{
public:

    UTF8String sharedStrings;
    UTF8String sheet;

public:

    void ParseOverride(const char **atts)
    {
        UTF8String PartName;
        UTF8String ContentType;

        while(*atts)
        {
            const char *flag=*atts;atts++;
            const char *info=*atts;atts++;

            if(strcmp(flag,"PartName")==0)PartName=info;else
            if(strcmp(flag,"ContentType")==0)ContentType=info;
        }

        if(ContentType==u8"application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml")
            sharedStrings=PartName;
        else
        if(ContentType==u8"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml")
            sheet=PartName;
    }

    void StartElement(const char *element_name, const char **atts)
    {
        if(strcmp(element_name,"Override")==0)
            ParseOverride(atts);
    }
};//class ContentTypesParse

class SharedStringsParse:public XMLParse
{
    bool t;
    int cur_count;

public:

    int count;
    int unique_count;

    UTF16String *ws;

public:

    SharedStringsParse()
    {
        ws=nullptr;
        cur_count=0;
        unique_count=0;
        t=false;
    }

    ~SharedStringsParse()
    {
        delete[] ws;
    }

    void ParseSST(const char **atts)
    {
        while(*atts)
        {
            const char *flag=*atts;atts++;
            const char *info=*atts;atts++;

            if(strcmp(flag,"count")==0)
                stoi(info,count);
            else
            if(strcmp(flag,"uniqueCount")==0)
                stoi(info,unique_count);
        }

        ws=new UTF16String[hgl_max(unique_count,count)];
    }

    void StartElement(const char *element_name, const char **atts)
    {
        if(strcmp(element_name,"sst")==0)
            ParseSST(atts);
        else
        if(strcmp(element_name,"t")==0)
            t=true;
    }

    void CharData(const char *str,int len)
    {
        if(!t)return;

        ws[cur_count++]=to_u16(str,len);
    }

    void EndElement(const char *element_name)
    {
        if(strcmp(element_name,"t")==0)
            t=false;
    }
};//class SharedStringsParse

class SheetParse:public XMLParse
{
    ObjectList<UTF16StringList> record;

    UTF16StringList *row_data;

    UTF16String *shared_string;

private:

    int col_count;
    int row_count;

    int col,row;
    int last_col;
    int cur_count;        //当前列数量

    bool v;
    char t;        //数据类型

private:

    void ParsePos(const char *str,int &col,int &row)
    {
        int bit=0;
        int col_value[16];

        while(isalpha(*str))
        {
            col_value[bit]=toupper(*str)-'A';
            str++;
            bit++;
        }

        if(bit==4)
            col=(col_value[0]+1)*26*26*26+col_value[1]*26*26+col_value[2]*26+col_value[3];
        else
        if(bit==3)
            col=(col_value[0]+1)*26*26+col_value[1]*26+col_value[2];
        else
        if(bit==2)
            col=(col_value[0]+1)*26+col_value[1];
        else
        if(bit==1)
            col=col_value[0];

        stoi(str,row);
    }

public:

    SheetParse()
    {
        row_data=nullptr;
        row_count=0;
    }

    ~SheetParse()
    {
    }

    void SetSharedString(UTF16String *ws)
    {
        shared_string=ws;
    }

    const int GetRowCount()const{return row_count;}
    const int GetColCount()const{return col_count;}
    UTF16StringList *GetRecord(int row){return record[row];}

    void ParseCell(const char **atts)
    {
        t=0;

        while(*atts)
        {
            const char *flag=*atts;atts++;
            const char *info=*atts;atts++;

            if(strcmp(flag,"r")==0)
                ParsePos(info,col,row);
            else
            if(strcmp(flag,"t")==0)
                t=*info;
        }
    }

    void StartElement(const char *element_name, const char **atts)
    {
        if(strcmp(element_name,"row")==0)
        {
            if(!row_data)
                row_data=new UTF16StringList;

            cur_count=0;
            last_col=-1;
        }
        else
        if(strcmp(element_name,"c")==0)
            ParseCell(atts);
        else
        if(strcmp(element_name,"v")==0)
            v=true;
    }

    void CharData(const char *str,int len)
    {
        if(!v)return;

        for(int i=last_col+1;i<col;i++)        //增加空数据
            row_data->Add(u"");

        if(t=='s')        //不知道是字符串还是共享字串的意思，反正数据是一个共享字串的索引
        {
            int index;

            stoi(str,index);

            row_data->Add(shared_string[index]);
            cur_count++;
        }
        else
        if(t=='n')        //直接的数字
        {
            row_data->Add(to_u16(str,len));
            cur_count++;
        }
        else            //直接的文本
        {
            row_data->Add(to_u16(str,len));
            cur_count++;
        }

        last_col=col;
    }

    void EndElement(const char *element_name)
    {
        if(strcmp(element_name,"row")==0)
        {
            if(cur_count)
            {
                if(row_data->GetString(0)[0]!=UNREAD_LINE_FRONT)
                    record.Add(row_data);
                else
                    delete row_data;
                
                row_count++;
            }

            row_data=nullptr;
        }
        else
        if(strcmp(element_name,"v")==0)
            v=false;
    }
};//class SheetParse

class LoaderXLSX:public Loader
{
    ContentTypesParse ctp;
    SharedStringsParse ssp;
    SheetParse sp;

    const UTF16StringList *name_list;
    const UTF16StringList *typename_list;
    FieldTypeList type_list;

    int use_col_count;
    int record_count;

private:

    bool XMLProc(struct zip *zpack,const char *filename,XMLParse *xml)
    {
        struct zip_stat zs;

        if(zip_stat(zpack,filename,0,&zs))
            return(false);

        struct zip_file *zfp=zip_fopen_index(zpack,zs.index,0);

        if(!zfp)
            return(false);

        SharedArray<char> filedata=new char[zs.size];

        zip_fread(zfp,filedata,zs.size);
        zip_fclose(zfp);

        xml->Start();

        return xml->Parse(filedata,zs.size,true);
    }

    void PrevProcess()
    {
        name_list=sp.GetRecord(0);
        typename_list=sp.GetRecord(1);

        ParseFieldType(type_list,*typename_list);
        use_col_count=(name_list->GetCount()>typename_list->GetCount()?typename_list->GetCount():name_list->GetCount());

        record_count=sp.GetRowCount()-2;
    }

public:

    bool    Load            (const OSString &filename,const CharSet &)                    ///<加载文件
    {
        int errop;

        struct zip *zpack=zip_open(filename.c_str(),0,&errop);

        if(!zpack)
            return(false);

        if(XMLProc(zpack,"[Content_Types].xml",&ctp))
        {
            if(XMLProc(zpack,ctp.sharedStrings.c_str()+1,&ssp))
            {
                sp.SetSharedString(ssp.ws);

                if(XMLProc(zpack,ctp.sheet.c_str()+1,&sp))
                {
                    zip_close(zpack);
                    PrevProcess();
                    return(true);
                }
            }
        }

        zip_close(zpack);
        return(false);
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

        return sp.GetRecord(row);
    }

    void Release(UTF16StringList *)
    {
    }
};//class LoaderXLSX

Loader *CreateLoaderXLSX()
{
    return(new LoaderXLSX);
}
