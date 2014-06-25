#include<hgl/dfs/dfsClientID.h>
#include<hgl/thread/Thread.h>
#include<hgl/type/BaseString.h>
#include<hgl/Str.h>
#include<iostream>

using namespace hgl;
using namespace hgl::dfs;

int main(int args,char **argv)
{
	if(args<3)
	{
		std::cout<<"dfsIDTest [dfsIDServer IP] [key_name]"<<std::endl;
		return(0);
	}

	dfsClientID *dfs_id=new dfsClientID("Role");
	
	int port;
	
	stoi(argv[2],port);

	dfs_id->Init(argv[1],port,"GameServer");

	dfsIDKey key;
	int64 add_id;
	int64 get_id;
	
	UTF16String ws=to_u16(argv[3]);

	StringToKey(key,ws);

	get_id=dfs_id->Get(key);

	std::cout<<"get \""<<argv[3]<<"\" id is "<<get_id<<std::endl;

	if(get_id==-1)
	{
		add_id=dfs_id->Add(key);
		std::cout<<"add \""<<argv[3]<<"\" id is "<<add_id<<std::endl;
	}

	return(0);
}
