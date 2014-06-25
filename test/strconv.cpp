#include<hgl/type/BaseString.h>
#include<hgl/type/DateTime.h>
#include<iostream>

using namespace hgl;

class TableName
{
	UTF8String table_name;

public:

	const UTF8String &	GetTableName()const{return table_name;}								///<取得表名
};

int main(int,char **)
{
	Date d;
	TableName *tn=nullptr;

	d.Sync();

	UTF8String str=UTF8String(d.GetYear())+'_'+UTF8String(d.GetMonth())+'_'+UTF8String(d.GetDay())+'_'+tn->GetTableName();

	std::cout<<str.c_str()<<std::endl;
	return(0);
}
