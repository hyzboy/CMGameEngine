#include<iostream>
#include<hgl/type/BaseString.h>
#include"RedisDB.h"
#include <../../../project/LobbyServer/Manor/MetallurgyItem.h>

using namespace std;
using namespace hgl;
using namespace hgl::redis;

int main(int,char **)
{
	RedisDB *db=ConnectRedisDB("127.0.0.1",6379);

	redis_hashs_item_list item_list;

	int count=db->HGetAll("test",item_list);

	cout<<"HGetAll test, result count="<<count<<endl;

	for(int i=0;i<count;i++)
	{
		UTF8String name;
		UTF8String value;
		HashsItem *item;

		item_list.Get(i,name,item);

		value.Set((const char *)(item->GetData()),item->GetSize());

		cout<<"field["<<i<<"] name is \""<<name.c_str()<<"\" value is \""<<value.c_str()<<"\""<<endl;
	}

	int age;
	item_list["Age"]->GetInteger(age);
	item_list["Age"]->Set("18");

	bool sex;
	item_list["Sex"]->GetBool(sex);

	cout<<"age="<<age<<endl;
	cout<<"sex="<<sex<<endl;

	count=db->HSetUpdate("test",item_list);

	if(count>=0)
	{
		cout<<"set update ok,count="<<count<<endl;
	}
	else
	{
		cout<<"set update failed."<<endl;
	}

	delete db;
	return(0);
}
