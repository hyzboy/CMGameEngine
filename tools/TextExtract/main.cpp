#include<hgl/Console.h>
#include<hgl/type/StringList.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/File.h>
#include<hgl/Str.h>
#include<iostream>

using namespace hgl;

struct TextExtractOption
{
	bool clear_comment;
	bool clear_english;
	bool clear_symbol;

	CharSet char_set;

	OSString input_file;
	OSString save_path;
	OSString save_ext;

public:

	TextExtractOption()
	{
		clear_comment=false;
		clear_english=false;
		clear_symbol=false;
	}
};//struct TextExtractOption

void TextExtract(TextExtractOption *teo,hgl::FileInfo &fi)
{
	if(fi.is_directory)
	{
		//std::cout<<"directory: "<<fi.fullname<<std::endl;
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

	u16char *orgin_text;
	int orgin_size;

	orgin_size=LoadTxtToMemory(fi.fullname,&orgin_text,teo->char_set);

	os_char new_name[HGL_MAX_PATH];

	replace_extname(fi.name,new_name,teo->save_ext.c_str());

	OSString target_filename=teo->save_path+new_name;

	io::FileOutputStream fos;

	if(!fos.Create(target_filename))
	{
		std::cout<<"create file failed, filename: "<<target_filename.c_str()<<std::endl;
		return;
	}

	std::cout<<"create file OK! filename: "<<target_filename.c_str()<<std::endl;

	fos.Write("\xFF\xFE",2);		//utf16le文件bom头

	UTF16StringList txt_list;

	SplitToStringList(txt_list,orgin_text,orgin_size);

	for(int row=0;row<txt_list.GetCount();row++)
	{
		const u16char *str=txt_list.GetString(row);

		const u16char *start,*end;

		if(!str)continue;

		while(*str==' '||*str=='\t')++str;		//去掉行首空格

		if(teo->clear_comment)
			if(str[0]=='/'&&str[1]=='/')continue;	//跳过C/C++注释行

		while(true)
		{
			start=strchr(str,'\"');

			if(!start)break;

			end=strchr(start+1,'\"');

			if(!end)break;

			str=end+1;		//str跳到end的下一个字符处

			++start;		//start后移去掉引号
			--end;			//end前移去掉引号

			if(teo->clear_comment)
			{
				if(start[0]=='/'&&start[1]=='/')
					continue;	//跳过C/C++注释段
			}

			UTF16String result;

			int len=end-start+1;

			if(teo->clear_english)		//将前后的英文去掉
			{
				while(len&&*start>0&&*start<256)
				{
					++start;
					--len;
				}

				while(len&&*end>0&&*end<256)
				{
					--end;
					--len;
				}
			}

			if(len>0)
			{
				fos.Write(start,(end-start+1)*sizeof(u16char));
				fos.Write(U16_TEXT("\n"),2);
			}
		}
	}

	fos.Close();
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
		std::cout<<"\t\t-cc\tclear C/C++ comment"<<std::endl;
		std::cout<<"\t\t-ca\tclear AsciiChar and Symbol"<<std::endl;
		std::cout<<"\t\t-sf\tproc sub folder"<<std::endl;
		std::cout<<std::endl;
		std::cout<<" Example: TextExtract [-cc,-co] <shift_jis|big5|gbk> <input path> <input filename> <output path> <output file extname>"<<std::endl;
		return(0);
	}

	TextExtractOption teo;

	bool sub_folder=false;
	int off=1;

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

		std::cout<<"option: Clear pure english char"<<std::endl;
	}

	if(args.CaseFind("-cs")!=-1)
	{
		teo.clear_symbol=true;
		++off;

		std::cout<<"option: Clear pure symbol char"<<std::endl;
	}

	std::cout<<"CharSet: "<<args[off]<<std::endl;

	teo.char_set=CharSet(args[off++]);

	OSString input_path=args[off++];
	teo.input_file=args[off++];
	teo.save_path=args[off++];
	teo.save_ext=args[off++];

	std::cout<<"Input Path: "<<input_path.c_str()<<std::endl;
	std::cout<<"Input Filename: "<<teo.input_file.c_str()<<std::endl;
	std::cout<<"Output Path: "<<teo.save_path.c_str()<<std::endl;
	std::cout<<"Output ExtName: "<<teo.save_ext.c_str()<<std::endl;

	return EnumFile(input_path,&teo,false,true,sub_folder,(void (*)(void *,hgl::FileInfo &))&TextExtract);
}
