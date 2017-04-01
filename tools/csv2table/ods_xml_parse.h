#ifndef HGL_CSV2TABLE_ODS_XML_PARSE_INCLUDE
#define HGL_CSV2TABLE_ODS_XML_PARSE_INCLUDE

#include<hgl/ut/XMLParse.h>
#include<hgl/type/StringList.h>
#include<hgl/type/List.h>
#include<hgl/Str.h>

using namespace hgl;

class ODSParse:public XMLParse
{
	ObjectList<UTF16StringList> record;

	int row_count;
	int col_count;

	UTF16StringList *row;
	bool cell;
	bool text_p;
	bool data;

	int repeated;	//重复添加次数

public:

	ODSParse()
	{
		row_count=0;
		col_count=0;
		cell=false;
		text_p=false;
		data=false;
		row=nullptr;
	}

	const int GetRowCount()const{return row_count;}
//	const int GetColCount()const{return col_count;}
	UTF16StringList *GetRecord(int row){return record[row];}

	void ParseCell(const char **atts)
	{
		while(*atts)
		{
			const char *flag=*atts;atts++;
			const char *info=*atts;atts++;

			if(strcmp(flag,"table:number-columns-repeated")==0)		//重复
				stoi(info,repeated);
		}
	}

	void StartElement(const char *element_name, const char **atts)
	{
		if(strcmp(element_name,"table:table-row")==0)
		{
			if(!row)
				row=new UTF16StringList;
		}
		else
		if(strcmp(element_name,"table:table-cell")==0)
		{
			if(data)						//还有数据没添加,放在这里是因为OpenOffice/LibreOffice不管有没有数据，都会存1024列或更多，所以这行的话，最后的空格就不添加了
				for(int i=0;i<repeated;i++)
					row->Add(UTF16String());			//添加一个空数据

			cell=true;
			repeated=1;
			ParseCell(atts);
			data=true;
		}
		else
		if(strcmp(element_name,"text:p")==0)
		{
			text_p=true;
		}
	}

	void CharData(const char *str,int len)
	{
		if(cell&&text_p)
		{
			UTF16String ws=to_u16(str,len);

			LOG_INFO(ws);

			for(int i=0;i<repeated;i++)
				row->Add(ws);

			data=false;
		}
	}

	void EndElement(const char *element_name)
	{
		if(strcmp(element_name,"table:table-row")==0)
		{
			if(row->GetCount()>0)
			{
				if(row->GetString(0)[0]!=UNREAD_LINE_FRONT)		//第一个字符为;的不读
					record.Add(row);
				else
					delete row;

				row_count++;
			}

			data=false;

			col_count=0;
			row=nullptr;
		}
		else
		if(strcmp(element_name,"table:table-cell")==0)
		{
			if(row_count==0)
				col_count++;

			cell=false;
		}
		else
		if(strcmp(element_name,"text:p")==0)
		{
			text_p=false;
		}
	}
};//class ODSParse
#endif//HGL_CSV2TABLE_ODS_XML_PARSE_INCLUDE
