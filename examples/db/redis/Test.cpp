#include<iostream>
#include"RedisDB.h"

using namespace std;
using namespace hgl::redis;

int main(int,char **)
{
	RedisDB *db=ConnectRedisDB("127.0.0.1",6379);

	const wchar_t *name=L"hyzboy";

	db->SetBinary("username",name,6*sizeof(wchar_t));

	char username[1024];

	int len=db->GetBinary("username",username,1024);

	wchar_t *new_name=(wchar_t *)username;

	new_name[len/sizeof(wchar_t)]=0;

	wcout<<L"username:"<<new_name<<L"\tlen="<<len<<endl;

	db->Del("username");

	delete db;
	return(0);
}
