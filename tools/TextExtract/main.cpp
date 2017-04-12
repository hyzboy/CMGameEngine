#include<hgl/Console.h>
#include<hgl/type/StringList.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/FileSystem.h>
#include<hgl/Str.h>
#include<iostream>

using namespace hgl;

struct TextExtractOption
{
    bool test;

    bool clear_comment;
    bool clear_english;
    bool clear_symbol;
    bool clear_edge;

    CharSet char_set;

    OSString input_path;
    OSString input_file;
    OSString save_path;
    OSString save_ext;

public:

    TextExtractOption()
    {
        test=false;
        clear_comment=false;
        clear_english=false;
        clear_symbol=false;
        clear_edge=false;
    }
};//struct TextExtractOption

void TextExtract(TextExtractOption *teo,hgl::filesystem::FileInfo &fi)
{
    if(fi.is_directory)
    {
        std::cout<<"directory: "<<fi.fullname<<std::endl;

        return;
    }
    else
    if(fi.is_file)
    {
        if(!fi.can_read)
        {
            //std::cout<<"can't read file: "<<fi.fullname<<std::endl;
            return;
        }
    }

    if(!stristr(fi.fullname,teo->input_file.c_str()))
        return;

    std::cout<<"read from file: "<<fi.fullname<<std::endl;

    if(teo->test)
        return;

    u16char *orgin_text;
    int orgin_size;

    orgin_size=LoadTxtToMemory(fi.fullname,&orgin_text,teo->char_set);

    os_char new_name[HGL_MAX_PATH];

    replace_extname(fi.name,new_name,HGL_MAX_PATH,teo->save_ext.c_str());

    OSString target_filename=teo->save_path+new_name;

    io::FileOutputStream fos;

    if(!fos.Create(target_filename))
    {
        std::cout<<"create file failed, filename: "<<target_filename.c_str()<<std::endl;
        return;
    }

    std::cout<<"create file OK! filename: "<<target_filename.c_str()<<std::endl;

    {
        const BOMFileHeader &bfh=BOMData[bomUTF16LE];    //取utf16le BOM头数据

        fos.Write(bfh.data,bfh.size);    //写入utf16le文件bom头
    }

    int write_total=0;

    UTF16StringList txt_list;

    SplitToStringList(txt_list,orgin_text,orgin_size);

    for(int row=0;row<txt_list.GetCount();row++)
    {
        const u16char *str=txt_list.GetString(row);

        const u16char *start,*end;

        if(!str)continue;

        while(*str==' '||*str=='\t')++str;        //去掉行首空格

        if(teo->clear_comment)
            if(str[0]=='/'&&str[1]=='/')continue;    //跳过C/C++注释行

        while(true)
        {
            start=strchr(str,'\"');

            if(!start)break;

            end=strchr(start+1,'\"');

            if(!end)break;

            str=end+1;        //str跳到end的下一个字符处

            ++start;        //start后移去掉引号
            --end;            //end前移去掉引号

            if(teo->clear_comment)
            {
                if(start[0]=='/'&&start[1]=='/')
                    continue;    //跳过C/C++注释段
            }

            UTF16String result;

            int len=end-start+1;

            if(teo->clear_symbol)        //将前后空格符号去掉
            {
                while(len&&(*start==' '||*start=='\t'))
                {
                    ++start;
                    --len;
                }

                while(len&&(*end==' '||*end=='\t'))
                {
                    ++end;
                    --len;
                }
            }

            const u16char *ps=start;
            const u16char *pe=end;

            if(teo->clear_english)        //计算前后的英文
            {
                while(len&&*ps>0&&*ps<256)
                {
                    ++ps;
                    --len;
                }

                while(len&&*pe>0&&*pe<256)
                {
                    --pe;
                    --len;
                }
            }

            if(len>0)        //如果不是全英文
            {
                if(teo->clear_edge)        //如果要求清除首尾字符
                {
                    fos.Write(ps,(pe-ps+1)*sizeof(u16char));
                }
                else
                {
                    fos.Write(start,(end-start+1)*sizeof(u16char));
                }

                fos.Write(U16_TEXT("\n"),2);
                ++write_total;
            }
        }
    }

    fos.Close();

    if(write_total==0)
        FileDelete(target_filename);

    delete[] orgin_text;
}

HGL_CONSOLE_MAIN_FUNC()
{
    std::cout<<"CMGDK tools << Text Extract v2.0 >>"<<std::endl;
    std::cout<<"(C)1997-2014 Copyright. Offical Web:www.hyzgame.com.cn"<<std::endl;
    std::cout<<std::endl;

    if(args.GetCount()<7)
    {
        std::cout<<" option:"<<std::endl;
        std::cout<<"\t\t-test\ttest"<<std::endl;
        std::cout<<"\t\t-cc\tclear C/C++ comment"<<std::endl;
        std::cout<<"\t\t-ce\tclear pure english string"<<std::endl;
        std::cout<<"\t\t-cs\tclear pure symbol char"<<std::endl;
        std::cout<<"\t\t-cb\tclear broder english&symbol"<<std::endl;
        std::cout<<"\t\t-sf\tproc sub folder"<<std::endl;
        std::cout<<std::endl;
        std::cout<<" Format: TextExtract [-cc,-ce,-cs,-cb,-sf] <shift_jis|big5|gbk|...> <input path> <input filename> <output path> <output file extname>"<<std::endl;
        std::cout<<" Example: TextExtract -cc -ce -cs -cb -sf shift_jis /home/hyzboy/input_csv/ .csv /home/hyzboy/output_txt/ txt"<<std::endl;
        return(0);
    }

    TextExtractOption teo;

    bool sub_folder=false;
    int off=1;

    if(args.CaseFind("-test")!=-1)
    {
        teo.test=true;
        ++off;

        std::cout<<"option: test"<<std::endl;
    }

    if(args.CaseFind("-cc")!=-1)
    {
        teo.clear_comment=true;
        ++off;

        std::cout<<"option: Clear C/C++ comment"<<std::endl;
    }

    if(args.CaseFind("-ce")!=-1)
    {
        teo.clear_english=true;
        ++off;

        std::cout<<"option: Clear pure english string"<<std::endl;
    }

    if(args.CaseFind("-cs")!=-1)
    {
        teo.clear_symbol=true;
        ++off;

        std::cout<<"option: Clear pure symbol char"<<std::endl;
    }

    if(args.CaseFind("-cb")!=-1)
    {
        teo.clear_edge=true;
        ++off;

        std::cout<<"option: Clip broder english&symbol"<<std::endl;
    }

    if(args.CaseFind("-sf")!=-1)
    {
        sub_folder=true;
        ++off;

        std::cout<<"option: proc sub folder"<<std::endl;
    }

    std::cout<<"CharSet: "<<args[off]<<std::endl;

    teo.char_set=CharSet(args[off++]);

    teo.input_path=args[off++];
    teo.input_file=args[off++];
    teo.save_path=args[off++];
    teo.save_ext=args[off++];

    if(teo.input_path.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
        teo.input_path+=HGL_DIRECTORY_SEPARATOR;

    if(teo.save_path.GetEndChar()!=HGL_DIRECTORY_SEPARATOR)
        teo.save_path+=HGL_DIRECTORY_SEPARATOR;

    MakePath(teo.save_path);

    std::cout<<"Input Path: "<<teo.input_path.c_str()<<std::endl;
    std::cout<<"Input Filename: "<<teo.input_file.c_str()<<std::endl;
    std::cout<<"Output Path: "<<teo.save_path.c_str()<<std::endl;
    std::cout<<"Output ExtName: "<<teo.save_ext.c_str()<<std::endl;

    return EnumFile(teo.input_path,&teo,false,true,sub_folder,(EnumFileFunc)&TextExtract);
}
