#include<hgl/type/BaseString.h>
#include<hgl/type/List.h>
#include<iostream>

using namespace hgl;
using namespace std;

bool SplitName(UTF8String &table_name,UTF8String &file_name,const UTF8String &name)
{
	const int index=name.FindChar('.');

	if(index<=0||index>=name.Length())return(false);

	table_name.Strcpy(name.c_str()+index+1,name.Length()-index-1);
	file_name.Strcpy(name.c_str(),index);

	return(true);
}

int main(int,char **)
{
	UTF16String DBUserID=u8_to_u16("9");
	UTF16String name=u"NAME";
	bool sex=true;
	int job=3;

	UTF16String buf=u"insert into RoleBase(user_id,rolename,arm,job,sex,Head,Body) values("
					+DBUserID+u",'"+UTF16String(name)
					+(sex?L"',1,":u"',0,")+UTF16String(job)
					+u",0,0,0)";

	wcout<<buf.c_str()<<endl;

// 	UTF16String null_buf=u"";
//
// 	wcout<<L"null_buf:"<<null_buf.c_str()<<endl;
// 	wcout<<L"null_buf_size:"<<null_buf.Length()<<endl;

	List<int> li={1,2,3,4,5,6};

	li={9,8,7,6,5,4,3,2,1};

	for(int i=0;i<li.GetCount();i++)
		wcout<<L"List<int> items "<<i<<L" : "<<li[i]<<endl;

	char str[]="1.2";
	float value;

	etof(str,value);

	wcout<<L"1e10="<<value<<endl;

	UTF8String filename="1.role";
	UTF8String tn,fn;

	SplitName(tn,fn,filename);

	cout<<"fullname:"<<filename.c_str()<<endl;
	cout<<"table name:"<<tn.c_str()<<endl;
	cout<<"field name:"<<fn.c_str()<<endl;

	return 0;
}
