#include<hgl/Console.h>
#include<hgl/type/StringList.h>
#include<hgl/FileSystem.h>

using namespace hgl;
using namespace hgl::filesystem;

static OSStringList convert_ext_list;       //要转换的文件

static OSStringList discard_file_list;      //要抛弃的文件
static OSStringList discard_folder_list;    //要抛弃的目录

static bool japanese_hw2fw=false;
static bool english_fw2hw=false;
static bool sub_folder=false;
static bool include_bom=true;

static CharSet in_charset;
static CharSet out_charset;

EnumFileConfig *ConvertSubFolderConfig(struct EnumFileConfig *efc,const OSString &sub_folder_name)
{
    if(sub_folder_name.GetBeginChar()=='.')return(nullptr);         //跳过第一个字符为.的目录

    for(int i=0;i<discard_folder_list.GetCount();i++)
    {
        if(discard_folder_list[i].Comp(sub_folder_name)==0)
            return(nullptr);
    }

    return DefaultCreateSubConfig(efc,sub_folder_name);
}

bool ExtNameCheck(const os_char *ext_name)
{
    const int len=strlen(ext_name);

    for(int i=0;i<convert_ext_list.GetCount();i++)
    {
        const OSString &fn=convert_ext_list[i];

        if(fn.Length()!=len)continue;

        if(fn.CaseComp(ext_name)==0)
            return(true);
    }

    return(false);
}

void ConvertFile(struct EnumFileConfig *efc,FileInfo &fi)
{
    if(fi.name[0]=='.')return;

    for(int i=0;i<discard_file_list.GetCount();i++)
    {
        if(discard_file_list[i].Comp(fi.name)==0)
            return;
    }

    const char *ext_name=strrchr<os_char>(fi.name,'.');

    if(!ext_name)return;

    if(!ExtNameCheck(ext_name))return;

    std::cout<<"filename: "<<fi.fullname<<std::endl;
}

void out_string_list(const OSStringList &sl,const os_char *name)
{
    std::cout<<" "<<name<<": "<<std::endl;

    for(int i=0;i<sl.GetCount();i++)
    {
        if(sl[i].Length()<=0)continue;
        std::cout<<"\t"<<i<<": "<<sl[i].c_str()<<std::endl;
    }

    std::cout<<std::endl;
}

HGL_CONSOLE_MAIN_FUNC()
{
    std::cout<<std::endl;
    std::cout<<"CMGDK Tools << Text Encoding Convert v2.0 >>"<<std::endl;
    std::cout<<"(C)1997-2017 Copyright. Offical Web:www.hyzgame.com.cn"<<std::endl;
    std::cout<<std::endl;

    LoadStringListFromTextFile(convert_ext_list,OS_TEXT("TextEncodingConvert.filter"),UTF8CharSet);
    LoadStringListFromTextFile(discard_file_list,OS_TEXT("TextEncodingConvert.discard_file"),UTF8CharSet);
    LoadStringListFromTextFile(discard_folder_list,OS_TEXT("TextEncodingConvert.discard_folder"),UTF8CharSet);

    if(args.GetCount()<5)
    {
        std::cout<<" option:"<<std::endl;
        std::cout<<"\t-jh\tconvert shift-jis half-width japanese characters to full-width"<<std::endl;        //转换日文半角到全角
        std::cout<<"\t-bl\tconvert full-width english and number to half-width"<<std::endl;                   //转换全角英文和数字到半角
        std::cout<<"\t-no_bom\ttarget file add BOM header"<<std::endl;                                        //新文件不需要BOM头
        std::cout<<"\t-sf\tproc sub-folder"<<std::endl;                                                       //处理子目录
        std::cout<<std::endl;
        std::cout<<" Format:  TextEncodingConvert [-jh|-bl|-sf] -in <shift_jis|big5|gbk|...> <input path> -out <utf8|utf16le> <output path>"<<std::endl;
        std::cout<<" Example: TextEncodingConvert -in shift_jis -no_bom /home/hyzboy/input/ -out utf8 /home/hyzboy/output/"<<std::endl;
        std::cout<<std::endl;

        out_string_list(convert_ext_list,OS_TEXT("filter"));
        out_string_list(discard_file_list,OS_TEXT("discard file"));
        out_string_list(discard_folder_list,OS_TEXT("discard folder"));

        std::cout<<std::endl;
        return(0);
    }

    if(args.CaseFind("-jh")!=-1)
    {
        japanese_hw2fw=true;

        std::cout<<"option[-jh]: conevrt shift-jis half-width japanese characters to full-width"<<std::endl;
    }

    if(args.CaseFind("-bl")!=-1)
    {
        english_fw2hw=true;

        std::cout<<"option[-bl]: conevrt full-width english and number to half-width"<<std::endl;
    }

    if(args.CaseFind("-no_bom")!=-1)
    {
        include_bom=false;

        std::cout<<"option[-no_bom]: don't include BOM Header"<<std::endl;
    }

    if(args.CaseFind("-sf")!=-1)
    {
        sub_folder=true;

        std::cout<<"option[-sf]: proc sub-folder"<<std::endl;
    }

    int in_off=args.CaseFind("-in");

    if(in_off!=-1)
    {
        const OSString &in_cs_str=args[in_off+1];

        in_charset=CharSet(in_cs_str);

        if(in_charset.codepage==ccpNone)
        {
            std::cout<<"in charset error"<<std::endl;
            return(-1);
        }

        std::cout<<"in CharSet: "<<in_charset.charset<<std::endl;
    }

    int out_off=args.CaseFind("-out");

    if(out_off!=-1)
    {
        const OSString &out_cs_str=args[out_off+1];

        if(out_cs_str.CaseComp("utf8")==0)out_charset=CharSet(ccpUTF8);else
        if(out_cs_str.CaseComp("utf16")==0
         ||out_cs_str.CaseComp("utf16le")==0
         ||out_cs_str.CaseComp("utf16-le")==0
         ||out_cs_str.CaseComp("utf-16le")==0
        )out_charset=CharSet(ccpUTF16LE);else
        {
            std::cout<<"out charset error,only support utf8 or utf16le"<<std::endl;
            return(-2);
        }

        std::cout<<"out CharSet: "<<out_charset.charset<<std::endl;
    }

    const OSString in_folder=args[in_off+2];

    std::cout<<"in folder: "<<in_folder.c_str()<<std::endl;
    std::cout<<"out folder: "<<args[out_off+2].c_str()<<std::endl;

    std::cout<<std::endl;

    EnumFileConfig efc;

    efc.folder_name=in_folder;
    efc.sub_folder=sub_folder;

    efc.CreateSubConfig=ConvertSubFolderConfig;
    efc.cb_file=ConvertFile;

    EnumFile(&efc);

    return 0;
}
