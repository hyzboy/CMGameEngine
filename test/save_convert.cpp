#include<stdio.h>
#include<iostream>
#include<hgl/File.h>
#include<hgl/io/FileInputStream.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>

using namespace hgl;
using namespace hgl::io;

struct RoleAppearance
{
	u32		head;					//头部
	u32		body;					//身体

	bool	isExist;				//标识有没穿上这套装备（有可能会同时穿上好几套，脱的时候得一件一件的脱，按穿的显示的优先级来）

public:

	bool ReadFromDB(DataInputStream* dis)
	{
		if(!dis->ReadUint32(head))RETURN_FALSE;
		if(!dis->ReadUint32(body))RETURN_FALSE;
		if(!dis->ReadBool(isExist))RETURN_FALSE;

		std::cout<<"head="<<head<<",body="<<body<<",is_exist:"<<isExist<<std::endl;
		return(true);
	}

	bool WriteToDB(DataOutputStream* dos) const
	{
		if(!dos->WriteUint32(head))RETURN_FALSE;
		if(!dos->WriteUint32(body))RETURN_FALSE;
		if(!dos->WriteBool(isExist))RETURN_FALSE;
		return(true);
	}
};//struct RoleAppearance


void Proc(void *,hgl::FileInfo &fi)
{
	std::cout<<fi.fullname<<std::endl;

	uint32 weapon;
	RoleAppearance ra[2];
	int32 monster_id;
	uint32 title;
	uint32 offical;
	int64 con_id;
	UTF16String con_name;
	uint32 appear_type;
	int32 level;

	int32 sa_type,sa_level;

	FileInputStream fis;

	fis.Open(fi.fullname);

	DataInputStream *dis=new LEDataInputStream(&fis);

	dis->ReadUint32(weapon);	std::cout<<"weapon="<<weapon<<std::endl;

	ra[0].ReadFromDB(dis);
	ra[1].ReadFromDB(dis);

	/*for(int i=0;i<8;i++)
{
	dis->ReadInt32(sa_type);
	dis->ReadInt32(sa_level);

	std::cout<<"sa["<<i<<"] type:"<<sa_type<<",level:"<<sa_level<<std::endl;
}*/

	dis->Skip(8*8);

	dis->ReadInt32(monster_id);			std::cout<<"monster_id:"<<monster_id<<std::endl;
	dis->ReadUint32(title);				std::cout<<"title:"<<title<<std::endl;
	dis->ReadUint32(offical);			std::cout<<"offical:"<<offical<<std::endl;
	dis->ReadInt64(con_id);				std::cout<<"con_id:"<<con_id<<std::endl;
	dis->ReadUTF16LEString(con_name);	//std::cout<<"con_name:"<<to_u8(con_name).c_str()<<std::endl;
	dis->ReadUint32(appear_type);		std::cout<<"appear_type:"<<appear_type<<std::endl;
	dis->ReadInt32(level);				std::cout<<"level:"<<level<<std::endl;

	delete dis;
	fis.Close();

	FileOutputStream fos;

	fos.CreateTrunc(fi.fullname);

	DataOutputStream *dos=new LEDataOutputStream(&fos);

	dos->WriteInt32(weapon);
	ra[0].WriteToDB(dos);
	ra[1].WriteToDB(dos);

	dos->WriteInt32(monster_id);
	dos->WriteUint32(title);
	dos->WriteUint32(offical);
	dos->WriteInt64(con_id);
	dos->WriteUTF16LEString(con_name);
	dos->WriteUint32(appear_type);
	dos->WriteBool(false);
	dos->WriteInt32(level);

	delete dos;
	fos.Close();
}

int main(int,char **)
{
	return EnumFile(".",nullptr,false,true,false,Proc);
}
