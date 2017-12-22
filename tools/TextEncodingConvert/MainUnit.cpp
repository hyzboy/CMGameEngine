#include<hgl/Console.h>
#include<hgl/type/StringList.h>
#include<hgl/FileSystem.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::filesystem;

static OSStringList convert_ext_list;       //要转换的文件

static OSStringList discard_file_list;      //要抛弃的文件
static OSStringList discard_folder_list;    //要抛弃的目录

static OSString in_folder_base;
static OSString out_folder_base;

static bool fn_japanese_hw2fw=false;
static bool fn_english_fw2hw=false;

static bool japanese_hw2fw=false;
static bool english_fw2hw=false;

static bool sub_folder=false;
static bool include_bom=true;

static CharSet in_charset;
static CharSet out_charset;

void ConvertQtoB(char16_t *str,int len)
{
    constexpr char16_t QC[]=u"ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ１２３４５６７８９０";
    constexpr char16_t BC[]=u"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";

    const int tl=sizeof(QC)/sizeof(char16_t)-1;

    char16_t *p=str;

    while(len--)
    {
        for(int i=0;i<tl;i++)
        {
            if(*p==QC[i])
            {
                *p=BC[i];
                break;
            }
        }

        p++;
    }
}

EnumFileConfig *ConvertSubFolderConfig(struct EnumFileConfig *efc,const OSString &sub_folder_name)
{
    if(sub_folder_name.GetBeginChar()=='.')return(nullptr);         //跳过第一个字符为.的目录

    for(int i=0;i<discard_folder_list.GetCount();i++)
    {
        if(discard_folder_list[i].Comp(sub_folder_name)==0)
            return(nullptr);
    }

    OSString old_path;
    OSString new_path;

    MergeFilename(old_path,efc->folder_name,sub_folder_name);
    MergeFilename(new_path,out_folder_base,old_path.c_str()+in_folder_base.Length());

#if HGL_OS != HGL_OS_Windows
    UTF16String u16_path=to_u16(new_path);

    if(fn_english_fw2hw)
    {
        ConvertQtoB(u16_path.c_str(),u16_path.Length());

        new_path=to_u8(u16_path);
    }
#else
    ConvertQtoB(new_path.c_str(),new_path.Length());
#endif//

    if(!MakePath(new_path))
    {
        std::cout<<"[ERR] Create Directory failed: "<<new_path.c_str()<<std::endl;
        return(nullptr);
    }
    else
    {
        std::cout<<"[DIR] Create Directory: "<<new_path.c_str()<<std::endl;
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

template<typename TOS> void Convert(const OSString &in_filename,const OSString &out_filename)
{
    UTF16StringList old_text;

    if(LoadStringListFromTextFile(old_text,in_filename,in_charset)<=0)
        return;

    FileOutputStream fos;

    if(!fos.CreateTrunc(out_filename))
    {
        std::cout<<"[ERR] Create file failed: "<<out_filename.c_str()<<std::endl;
        return;
    }

    TOS tos(&fos);

    if(include_bom)
        tos.WriteBOM();

    if(!tos.WriteText(old_text))
    {
        std::cout<<"[ERR] Write Text failed"<<std::endl;
        return;
    }

    std::cout<<"[OK] filename: "<<out_filename.c_str()<<std::endl;
};

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

    std::cout<<"[IN] filename: "<<fi.fullname<<std::endl;

    //生成新文件名

    OSString new_filename=out_folder_base+(fi.fullname+in_folder_base.Length());

    #if HGL_OS != HGL_OS_Windows
    UTF16String u16_filename=to_u16(new_filename);

    if(fn_english_fw2hw)
    {
        ConvertQtoB(u16_filename.c_str(),u16_filename.Length());

        new_filename=to_u8(u16_filename);
    }
    #else
    ConvertQtoB(new_filename.c_str(),new_filename.Length());
    #endif//

    if(out_charset.codepage==ccpUTF8    )Convert<UTF8TextOutputStream   >(fi.fullname,new_filename);else
    if(out_charset.codepage==ccpUTF16LE )Convert<UTF16LETextOutputStream>(fi.fullname,new_filename);else
    if(out_charset.codepage==ccpUTF16BE )Convert<UTF16BETextOutputStream>(fi.fullname,new_filename);
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
        std::cout<<"\t-fjh\tconvert shift-jis half-width japanese filename to full-width"<<std::endl;       //转换日文半角文件名到全角
        std::cout<<"\t-jh\tconvert shift-jis half-width japanese text to full-width"<<std::endl;            //转换日文半角文本到全角
        std::cout<<"\t-fbl\tconvert full-width english and number filename to half-width"<<std::endl;       //转换全角英文和数字文件名到半角
        std::cout<<"\t-bl\tconvert full-width english and number text to half-width"<<std::endl;            //转换全角英文和数字文本到半角
        std::cout<<"\t-no_bom\ttarget file add BOM header"<<std::endl;                                      //新文件不需要BOM头
        std::cout<<"\t-sf\tproc sub-folder"<<std::endl;                                                     //处理子目录
        std::cout<<std::endl;
        std::cout<<" Format:  TextEncodingConvert [-fjh|-jh|-fbl|-bl|-no_bom|-sf] -in <shift_jis|big5|gbk|...> <input path> -out <utf8|utf16le> <output path>"<<std::endl;
        std::cout<<" Example: TextEncodingConvert -in shift_jis -no_bom /home/hyzboy/input/ -out utf8 /home/hyzboy/output/"<<std::endl;
        std::cout<<std::endl;

        out_string_list(convert_ext_list,OS_TEXT("filter"));
        out_string_list(discard_file_list,OS_TEXT("discard file"));
        out_string_list(discard_folder_list,OS_TEXT("discard folder"));

        std::cout<<std::endl;
        return(0);
    }

    if(args.CaseFind("-fjh")!=-1)
    {
        fn_japanese_hw2fw=true;

        std::cout<<"option[-fjh]: conevrt shift-jis half-width japanese filename to full-width"<<std::endl;
    }

    if(args.CaseFind("-jh")!=-1)
    {
        japanese_hw2fw=true;

        std::cout<<"option[-jh]: conevrt shift-jis half-width japanese text to full-width"<<std::endl;
    }

    if(args.CaseFind("-fbl")!=-1)
    {
        fn_english_fw2hw=true;

        std::cout<<"option[-fbl]: conevrt full-width english and number filename to half-width"<<std::endl;
    }

    if(args.CaseFind("-bl")!=-1)
    {
        english_fw2hw=true;

        std::cout<<"option[-bl]: conevrt full-width english and number text to half-width"<<std::endl;
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

    in_folder_base=args[in_off+2];
    out_folder_base=args[out_off+2];

    std::cout<<"in folder: "<<in_folder_base.c_str()<<std::endl;
    std::cout<<"out folder: "<<out_folder_base.c_str()<<std::endl;

    if(!MakePath(out_folder_base))
    {
        std::cout<<"[ERR] Create Directory failed: "<<out_folder_base.c_str()<<std::endl;
        return(-3);
    }

    std::cout<<std::endl;

    EnumFileConfig efc;

    efc.folder_name=in_folder_base;
    efc.sub_folder=sub_folder;

    efc.CreateSubConfig=ConvertSubFolderConfig;
    efc.cb_file=ConvertFile;

    EnumFile(&efc);

    return 0;
}
