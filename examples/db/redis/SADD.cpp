#include<iostream>
#include<hgl/db/RedisDB.h>
#include<hgl/Str.h>
#include<hgl/type/StringList.h>
#include<hgl/io/FileInputStream.h>

using namespace std;
using namespace hgl;
using namespace hgl::io;
using namespace hgl::redis;

int main(int argc,char **argv)
{
    std::cout<<"<<CMGDK>> RedisDB BatchSADD"<<std::endl;

    if(argc!=5
     &&argc!=3)
    {
        std::cout<<" use 1: SADD RedisIP RedisPort setname filename"<<std::endl;
        std::cout<<"     2: SADD setname filename"<<std::endl;
        return 1;
    }

    std::cout<<std::endl;

    char *set_name=nullptr;
    char *filename=nullptr;
    RedisDB *db=nullptr;

    if(argc==5)
    {
        uint port;

        stou(argv[2],port);

        std::cout<<"RedisIP:   " <<argv[1]<<std::endl;
        std::cout<<"RedisPort: " <<argv[2]<<std::endl;

        set_name=argv[3];
        filename=argv[4];

        RedisDB *db=ConnectRedisDB(argv[1],port);
    }
    else
    {
        set_name=argv[1];
        filename=argv[2];
    }

    std::cout<<"SET NAME:  " <<set_name<<std::endl;
    std::cout<<"Filename:  " <<filename<<std::endl;

    UTF8StringList sl;

    LoadUTF8FileToStringList(sl,filename);

    if(db)
        db->Multi();

    for(int i=0;i<sl.GetCount();i++)
    {
        std::cout<<"SADD "<<set_name<<" "<<sl[i].c_str()<<std::endl;

        if(db)
            db->SAdd(set_name,sl[i]);
    }

    if(db)
    {
        db->Exec();

        delete db;
    }
    return(0);
}
