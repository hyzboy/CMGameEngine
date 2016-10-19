#ifndef HGL_DB_FIELD_INCLUDE
#define HGL_DB_FIELD_INCLUDE

#include<hgl/type/BaseString.h>
#include<hgl/type/List.h>
#include<hgl/type/FixedList.h>
#include<hgl/db/FieldDesc.h>
#include<hgl/MemBlock.h>
namespace hgl
{
	namespace io
	{
		class DataInputStream;
		class DataOutputStream;
	}//namespace io

	using namespace io;

	namespace db
	{
		/**
		 * 字段数据定义
		 */
		class FieldData
		{
		protected:

			const FieldDesc *desc;

		public:

			FieldData(const FieldDesc *fd){desc=fd;}
			virtual ~FieldData()=default;

			const FieldDesc *GetDesc()const{return desc;}											///<取得字段描述
			const UTF8String &GetName()const{return desc->name;}									///<取得字段名称
			const FieldBaseType &GetBaseType()const{return desc->type.base;}						///<取得字段基类数据类型
			const int GetCount()const{return desc->type.count;}										///<取得字段内数据最大个数

		public:	//字段内容读写函数

			virtual const bool Get(bool &		)const{return(false);}

			virtual const bool Get(int8 &		)const{return(false);}
			virtual const bool Get(int16 &		)const{return(false);}
			virtual const bool Get(int32 &		)const{return(false);}
			virtual const bool Get(int64 &		)const{return(false);}

			virtual const bool Get(uint8 &		)const{return(false);}
			virtual const bool Get(uint16 &		)const{return(false);}
			virtual const bool Get(uint32 &		)const{return(false);}
			virtual const bool Get(uint64 &		)const{return(false);}

			virtual const bool Get(float &		)const{return(false);}
			virtual const bool Get(double &		)const{return(false);}

			virtual const bool Get(UTF16String &	)const{return(false);}
			virtual const bool Get(UTF8String &		)const{return(false);}
			virtual const int  Get(char *,		int	)const{return(false);}
			virtual const int  Get(u16char *,	int	)const{return(false);}
			virtual const int  Get(void *,		int	)const{return(false);}

			virtual const bool Set(const bool 			){return(false);}

			virtual const bool Set(const int8 			){return(false);}
			virtual const bool Set(const int16 		){return(false);}
			virtual const bool Set(const int32 		){return(false);}
			virtual const bool Set(const int64 		){return(false);}

			virtual const bool Set(const uint8 		){return(false);}
			virtual const bool Set(const uint16 		){return(false);}
			virtual const bool Set(const uint32 		){return(false);}
			virtual const bool Set(const uint64 		){return(false);}

			virtual const bool Set(const float 		){return(false);}
			virtual const bool Set(const double 		){return(false);}

			virtual const bool Set(const UTF16String &	){return(false);}
			virtual const bool Set(const UTF8String &	){return(false);}
			virtual const bool Set(const char *,	int	){return(false);}
			virtual const bool Set(const u16char *,int	){return(false);}

					const bool Set(const char *str		){return Set(str,hgl::strlen(str)+1);}
					const bool Set(const u16char *str	){return Set(str,hgl::strlen(str)+1);}

			virtual const bool Set(const void *,	int	){return(false);}
		};//class FieldData

		FieldData *CreateFieldData(const FieldDesc *field_desc);							///<创建一个数据字段

		template<typename ...T> FieldData *CreateFieldData(const FieldDesc *desc,T...value)	///<创建一个数据字段，并附带数据
		{
			FieldData *data=CreateFieldData(desc);

			if(!data)return(nullptr);

			if(!data->Set(value...))
			{
				delete data;
				return(nullptr);
			}

			return(data);
		}

		bool Write(hgl::io::DataOutputStream *,hgl::db::FieldData *);		///<将一个字段写入流
		bool Read(hgl::io::DataInputStream *,hgl::db::FieldData *);			///<从流中读取一个字段

		bool ToSQLString(UTF8String &,const FieldData *);					///<将一个字段转换成SQL字符串数据
		bool ParseSQLString(FieldData *,const UTF8String &);				///<解晰一个SQL字段数据

		bool ToJsonString(UTF8String &,const FieldData *);					///<将一个字段转换成Json字符串
		bool ParseJsonString(FieldData *,const UTF8String &);				///<解晰一个json字段数据
	}//namespace db
}//namespace hgl
#endif//HGL_DB_FIELD_INCLUDE
