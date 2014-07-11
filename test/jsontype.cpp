#include<hgl/type/BaseString.h>
#include<hgl/type/Map.h>
#include<hgl/Str.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/db/FieldType.h>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::db;

template<typename STR_TYPE> struct PropertyValue
{
// public:
//
// 	virtual bool toString(STR_TYPE &)=0;
// 	virtual bool fromString(const STR_TYPE &)=0;
//
// 	virtual bool Write(DataOutputStream *)const=0;
// 	virtual bool Read(DataInputStream *)=0;
};

template<typename STR_TYPE,typename DATA_TYPE> struct PropertyItem:public PropertyValue<STR_TYPE>
{
protected:

	DATA_TYPE data;

// public:
//
// 	virtual bool toString(STR_TYPE &)=0;
// 	virtual bool fromString(const STR_TYPE &)=0;
//
// 	virtual bool Write(DataOutputStream *)const=0;
// 	virtual bool Read(DataInputStream *)=0;

public:

	PropertyItem(const DATA_TYPE &d)
	{
		data=d;
	}

	operator DATA_TYPE &(){return data;}
	operator const DATA_TYPE &()const{return data;}

	DATA_TYPE &operator = (DATA_TYPE &dt){data=dt;return data;}
};

template<typename STR_TYPE,typename DATA_TYPE> struct PropertyItemNumber:PropertyItem<STR_TYPE,DATA_TYPE>
{
public:

	PropertyItemNumber(const DATA_TYPE &d):PropertyItem<STR_TYPE,DATA_TYPE>(d){}
};//struct PropertyItemNumber

// template<int S> bool WriteInteger(DataOutputStream *dos,const void *value);
//
// template<> inline bool WriteInteger<1>(DataOutputStream *dos,const void *value){return dos->WriteInt8(*(int8 *)value);};
// template<> inline bool WriteInteger<2>(DataOutputStream *dos,const void *value){return dos->WriteInt16(*(int16 *)value);};
// template<> inline bool WriteInteger<4>(DataOutputStream *dos,const void *value){return dos->WriteInt32(*(int32 *)value);};
// template<> inline bool WriteInteger<8>(DataOutputStream *dos,const void *value){return dos->WriteInt64(*(int64 *)value);};
//
// template<int S> bool ReadInteger(DataInputStream *dis,const void *value);
//
// template<> inline bool ReadInteger<1>(DataInputStream *dis,void *value){return dis->ReadInt8(*(int8 *)value);};
// template<> inline bool ReadInteger<2>(DataInputStream *dis,void *value){return dis->ReadInt16(*(int16 *)value);};
// template<> inline bool ReadInteger<4>(DataInputStream *dis,void *value){return dis->ReadInt32(*(int32 *)value);};
// template<> inline bool ReadInteger<8>(DataInputStream *dis,void *value){return dis->ReadInt64(*(int64 *)value);};

// template<typename STR_TYPE,typename DATA_TYPE> struct PropertyInteger:public PropertyItem<STR_TYPE,DATA_TYPE>
// {
// public:
//
// 	PropertyInteger(const DATA_TYPE &dt)
// 	{
// 		this->data=dt;
// 	}
//
// // 	bool Write(DataOutputStream *dos)const
// // 	{
// // 		return WriteInteger<sizeof(DATA_TYPE)>(dos,&(this->data));
// // 	}
// //
// // 	bool Read(DataInputStream *dis)
// // 	{
// // 		return ReadInteger<sizeof(DATA_TYPE)>(dis,&(this->data));
// // 	}
// //
// // 	bool toString(STR_TYPE &str)
// // 	{
// // 		str=this->data;
// // 		return(true);
// // 	}
// //
// // 	bool fromString(const STR_TYPE &str)
// // 	{
// // 		return str.ToInt(this->data);
// // 	}
// };
//
// // template<int S> bool WriteUnsignedInteger(DataOutputStream *dos,const void *value);
// //
// // template<> bool WriteUnsignedInteger<1>(DataOutputStream *dos,const void *value){return dos->WriteUint8(*(uint8 *)value);};
// // template<> bool WriteUnsignedInteger<2>(DataOutputStream *dos,const void *value){return dos->WriteUint16(*(uint16 *)value);};
// // template<> bool WriteUnsignedInteger<4>(DataOutputStream *dos,const void *value){return dos->WriteUint32(*(uint32 *)value);};
// // template<> bool WriteUnsignedInteger<8>(DataOutputStream *dos,const void *value){return dos->WriteUint64(*(uint64 *)value);};
// //
// // template<int S> bool ReadUnsignedInteger(DataInputStream *dis,const void *value);
// //
// // template<> bool ReadUnsignedInteger<1>(DataInputStream *dis,void *value){return dis->ReadUint8(*(uint8 *)value);};
// // template<> bool ReadUnsignedInteger<2>(DataInputStream *dis,void *value){return dis->ReadUint16(*(uint16 *)value);};
// // template<> bool ReadUnsignedInteger<4>(DataInputStream *dis,void *value){return dis->ReadUint32(*(uint32 *)value);};
// // template<> bool ReadUnsignedInteger<8>(DataInputStream *dis,void *value){return dis->ReadUint64(*(uint64 *)value);};
//
// template<typename STR_TYPE,typename DATA_TYPE> struct PropertyUnsignedInteger:public PropertyItem<STR_TYPE,DATA_TYPE>
// {
// public:
//
// 	PropertyUnsignedInteger(const DATA_TYPE &dt)
// 	{
// 		this->data=dt;
// 	}
//
// // 	bool Write(DataOutputStream *dos)const
// // 	{
// // 		return WriteUnsignedInteger<sizeof(DATA_TYPE)>(dos,&(this->data));
// // 	}
// //
// // 	bool Read(DataInputStream *dis)
// // 	{
// // 		return ReadUnsignedInteger<sizeof(DATA_TYPE)>(dis,&(this->data));
// // 	}
// //
// // 	bool toString(STR_TYPE &str)
// // 	{
// // 		str=this->data;
// // 		return(true);
// // 	}
// //
// // 	bool fromString(const STR_TYPE &str)
// // 	{
// // 		return str.ToUint(this->data);
// // 	}
// };
//
// // template<typename T> bool WriteFloat(DataOutputStream *dos,const T &value);
// //
// // template<> bool WriteFloat<float>(DataOutputStream *dos,const float value){return dos->WriteFloat(value);};
// // template<> bool WriteFloat<double>(DataOutputStream *dos,const double value){return dos->WriteDouble(value);};
// //
// // template<typename T> bool ReadFloat(DataInputStream *dis,const T &value);
// //
// // template<> bool ReadFloat<float>(DataInputStream *dis,float &value){return dis->ReadFloat(value);};
// // template<> bool ReadFloat<double>(DataInputStream *dis,double &value){return dis->ReadDouble(value);};
//
// template<typename STR_TYPE,typename DATA_TYPE> struct PropertyFloat:public PropertyItem<STR_TYPE,DATA_TYPE>
// {
// public:
//
// 	PropertyFloat(const DATA_TYPE &dt)
// 	{
// 		this->data=dt;
// 	}
//
// // 	bool Write(DataOutputStream *dos)const
// // 	{
// // 		return WriteFloat<DATA_TYPE>::Write(dos,this->data);
// // 	}
// //
// // 	bool Read(DataInputStream *dis)
// // 	{
// // 		return ReadFloat<DATA_TYPE>::Read(dis,this->data);
// // 	}
// //
// // 	bool toString(STR_TYPE &str)
// // 	{
// // 		str=this->data;
// // 		return(true);
// // 	}
// //
// // 	bool fromString(const STR_TYPE &str)
// // 	{
// // 		return str.ToFloat(this->data);
// // 	}
// };
//
// template<typename STR_TYPE> struct PropertyBool:public PropertyItem<STR_TYPE,bool>
// {
// public:
//
// 	PropertyBool(const bool dt)
// 	{
// 		this->data=dt;
// 	}
//
// // 	bool Write(DataOutputStream *dos)const{return dos->WriteBool(this->data);}
// // 	bool Read(DataInputStream *dis){return dis->ReadBool(this->data);}
// //
// // 	bool toString(STR_TYPE &str)
// // 	{
// // 		str=this->data;
// // 		return(true);
// // 	}
// //
// // 	bool fromString(const STR_TYPE &str)
// // 	{
// // 		return str.ToBool(this->data);
// // 	}
// };
//
// template<typename STR_TYPE> struct PropertyString:public PropertyItem<STR_TYPE,STR_TYPE>
// {
// public:
//
// 	PropertyString(const STR_TYPE &dt)
// 	{
// 		this->data=dt;
// 	}
//
// // 	bool Write(DataOutputStream *)const;
// // 	bool Read(DataInputStream *);
// //
// // 	bool toString(STR_TYPE &str)
// // 	{
// // 		str=this->data;
// // 		return(true);
// // 	}
// //
// // 	bool fromString(const STR_TYPE &str)
// // 	{
// // 		this->data=str;
// // 		return(true);
// // 	}
// };
//
// // template<> bool PropertyString<UTF8String>::Write(DataOutputStream *dos)const{return dos->WriteUTF8String(this->data);}
// // template<> bool PropertyString<UTF16String>::Write(DataOutputStream *dos)const{return dos->WriteUTF16LEString(this->data);}
// //
// // template<> bool PropertyString<UTF8String>::Read(DataInputStream *dis){return dis->ReadUTF8String(this->data);}
// // template<> bool PropertyString<UTF16String>::Read(DataInputStream *dis){return dis->ReadUTF16LEString(this->data);}
//
// template<typename STR_TYPE,typename DATA_TYPE> struct PropertyArray:public PropertyValue<STR_TYPE>
// {
// protected:
//
// 	List<DATA_TYPE> data_list;
//
// public:
//
// // 	bool toString(STR_TYPE &str)
// // 	{
// // 		const int count=data_list.GetCount();
// //
// // 		if(count<=0)return(true);
// //
// // 		DATA_TYPE *p=data_list.GetData();
// //
// // 		for(int i=1;i<count;i++)
// // 		{
// //
// // 		}
// // 	}
// //
// // 	bool fromString(const STR_TYPE &)
// // 	{
// // 	}
// };

template<typename STR_TYPE> class PropertyList:public MapObject<STR_TYPE,PropertyValue<STR_TYPE> >
{
public:

	template<typename DATA_TYPE> bool AddInteger(const STR_TYPE &name,const DATA_TYPE &value)
	{
		if(this->Find(name)!=-1)return(false);
		this->Add(name,new PropertyItemNumber<STR_TYPE,DATA_TYPE>(value));
		return(true);
	}

	template<typename DATA_TYPE> bool AddUInteger(const STR_TYPE &name,const DATA_TYPE &value)
	{
		if(this->Find(name)!=-1)return(false);
		this->Add(name,new PropertyItemNumber<STR_TYPE,DATA_TYPE>(value));
		return(true);
	}

	template<typename DATA_TYPE> bool AddFloat(const STR_TYPE &name,const DATA_TYPE &value)
	{
		if(this->Find(name)!=-1)return(false);
		this->Add(name,new PropertyItemNumber<STR_TYPE,DATA_TYPE>(value));
		return(true);
	}

	template<typename DATA_TYPE> bool AddBool(const STR_TYPE &name,const DATA_TYPE &value)
	{
		if(this->Find(name)!=-1)return(false);
		this->Add(name,new PropertyItem<STR_TYPE,bool>(value));
		return(true);
	}

	bool AddString(const STR_TYPE &name,const STR_TYPE &value)
	{
		if(this->Find(name)!=-1)return(false);
		this->Add(name,new PropertyItem<STR_TYPE,STR_TYPE>(value));
	}
};

typedef PropertyList<UTF8String> UTF8PropertyList;
typedef PropertyList<UTF16String> WidePropertyList;

int main(int,char **)
{
	UTF8PropertyList pl;

	pl.AddString	("name","HuYingzhuo");
	pl.AddInteger	("age",	32);
	pl.AddBool		("sex",	true);
	pl.AddFloat		("high",1.72);

	return 0;
}
