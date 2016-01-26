#include<hgl/Console.h>
#include<hgl/type/StringList.h>
#include<hgl/db/FieldType.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/DataOutputStream.h>
#include"Common.h"
#include"Loader.h"

using namespace hgl;
using namespace hgl::io;
using namespace hgl::db;

void InitWriteField();

Loader *CreateLoaderCSV();
Loader *CreateLoaderFODS();
Loader *CreateLoaderODS();
Loader *CreateLoaderXLSX();

Loader *CreateLoader(const os_char *filename,const CharSet &cs)
{
	if(!filename)
		return(nullptr);

	const os_char *ext_name=strrchr(filename,(os_char)'.');

	if(!ext_name)
		return(nullptr);

	Loader *l=nullptr;

	if(stricmp(ext_name,OS_TEXT(".csv"))==0||stricmp(ext_name,OS_TEXT(".txt"))==0)
		l=CreateLoaderCSV();
	else
	if(stricmp(ext_name,OS_TEXT(".fods"))==0)
		l=CreateLoaderFODS();
	else
	if(stricmp(ext_name,OS_TEXT(".ods"))==0)
		l=CreateLoaderODS();
	else
	if(stricmp(ext_name,OS_TEXT(".xlsx"))==0)
		l=CreateLoaderXLSX();

	if(!l)return(nullptr);

	if(!l->Load(filename,cs))
	{
		delete l;
		return(nullptr);
	}

	return l;
}

HGL_CONSOLE_MAIN_FUNC()
{
	sii.info.ProjectName=U8_TEXT("csv/ods/fods/xlsx To .table Converter");
	sii.info.ProjectCode=OS_TEXT("CSV2Table");

	app.Init(&sii);

	LOG_INFO(OS_TEXT("CMGDK tools <<.csv/ods/fods/xlsx to .table Converter>> 1.23"));
	LOG_INFO(OS_TEXT("(C)1997-2014 Copyright. Offical Web:www.hyzgame.com.cn"));

	if(args.GetCount()<2)
	{
		LOG_INFO(OS_TEXT("csv2table [-charset=....] <filename.(csv|ods|fods|xlsx)>"));
		return(0);
	}

	const OSString charset_flag=OS_TEXT("-charset");

	Loader *load;
	OSString tab_filename;

	if(args[1].CaseComp(charset_flag)==0)
	{
		tab_filename=args[2];
		load=CreateLoader(args[2],CharSet(args[1].c_str()+charset_flag.Length()));
	}
	else
	{
		tab_filename=args[1];
		load=CreateLoader(args[1],CharSet(ccpUTF8));
	}

	if(!load)
		return(-1);

	InitWriteField();		//初始化字段写入处理

	int row_count=0;
	int *col_count;

	char **buf;

	const int				use_col_count	=load->GetFieldCount();
	const int				ori_row_count	=load->GetRecordCount();
	const UTF16StringList &	name_list		=load->GetFieldNameList();
	const FieldTypeList &	type_list		=load->GetFieldTypeList();
	const UTF16StringList &	typename_list	=load->GetFieldTypeNameList();

	buf=new char *[use_col_count];
	col_count=new int[use_col_count];

	for(int i=0;i<use_col_count;i++)
	{
 		LOG_INFO(U16_TEXT("字段[")+UTF16String(i)+ U16_TEXT("] 类型:")+typename_list[i]+ U16_TEXT(" 名称:")+name_list[i]);

		if(type_list[i].count>0)
		{
			buf[i]=new char[type_list[i].size*(ori_row_count)];

			memset(buf[i],0,type_list[i].size*(ori_row_count));
		}
		else
		{
			buf[i]=nullptr;
		}

		col_count[i]=0;
	}

	for(int i=0;i<ori_row_count;i++)
	{
		UTF16StringList *colume_list=load->GetRecord(i);

		if(!colume_list)
			continue;

		int pc=use_col_count;

		if(pc>colume_list->GetCount())
			pc=colume_list->GetCount();

		for(int c=0;c<pc;c++)
		{
			if(type_list[c].count>0)
			{
				LOG_INFO(name_list[c]+U16_TEXT(":")+typename_list[c]+ U16_TEXT(":")+(*colume_list)[c]);

				if(WriteFieldData(type_list[c],buf[c]+row_count*type_list[c].size,(*colume_list)[c]))
					col_count[c]++;
			}
		}

		load->Release(colume_list);

		row_count++;
	}

	//按列存
	//说明：
	//		1.存在大量固定数据长度字段时，按列存将有利于访问和提升效能
	//		2.若按行存，需要为每一列数据保存偏移，或是将固定长度列分离出来，建立统一的偏移表，过于麻烦

	FileOutputStream fos;
	DataOutputStream *dos=new LEDataOutputStream(&fos);

	{
		int index=tab_filename.FindRightChar('.');

		if(index!=-1)
			tab_filename.ClipLeft(index);				//仅保存前方几个字符

		tab_filename.Strcat(OS_TEXT(".table"));

		if(!fos.Create(tab_filename))
			return(-3);
	}

	int save_col_count=0;

	dos->Write("CDB Table\x1A",10);			//文件头
	dos->WriteFloat(6.0);					//版本号

	for(int i=0;i<use_col_count;i++)
	{
		if(col_count[i])
			save_col_count++;
	}

	dos->WriteInt32(save_col_count);		//字段数量

	//字段名称
	for(int i=0;i<use_col_count;i++)
	{
		if(col_count[i])
			dos->WriteUTF16LEString(name_list[i]);
	}

	dos->WriteInt32(save_col_count);		//字段类型字串数量

	//字段信息
	for(int i=0;i<use_col_count;i++)
	{
		if(col_count[i])
			dos->WriteUTF16LEString(typename_list[i]);
	}

	dos->WriteInt32(row_count);			//记录数量

	for(int i=0;i<use_col_count;i++)
	{
		if(col_count[i])
			dos->Write(buf[i],type_list[i].size*row_count);

		if(buf[i])
			delete[] buf[i];
	}

	delete[] col_count;
	delete[] buf;

	return(0);
}
