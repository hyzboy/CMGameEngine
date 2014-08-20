#include<hgl/db/FieldType.h>
#include<hgl/db/CDBField.h>
#include<hgl/LogInfo.h>
namespace hgl
{
	namespace db
	{
		template<typename T>
		struct cdb_type_name_convert
		{
			const T name[32];
			FieldBaseType type;
		};

		#define FIELD_TYPE_CONVERY(name,type)	{U16_TEXT(name),type},
			const cdb_type_name_convert<u16char> typename_convert_list_u16[]=
			#include"FieldTypeConvert.h"
		#undef FIELD_TYPE_CONVERY

		#define FIELD_TYPE_CONVERY(name,type)	{U8_TEXT(name),type},
			const cdb_type_name_convert<char> typename_convert_list_u8[]=
			#include"FieldTypeConvert.h"
		#undef FIELD_TYPE_CONVERY

		int GetFieldSize(FieldBaseType base,int count)
		{
			if(count<0)return -1;						//错误长度
			if(count==0)return 0;						//未知长度

			if(base==fbtBinary	)return count;

			if(base==fbtInt8
			 ||base==fbtUint8
			 ||base==fbtChar8	)return count;

			if(base==fbtBool	)return count;

			if(base==fbtInt16
			 ||base==fbtUint16
			 ||base==fbtChar16le)return count<<1;

			if(base==fbtInt32
			 ||base==fbtUint32
			 ||base==fbtFloat	)return count<<2;

			if(base==fbtInt64
			 ||base==fbtUint64
			 ||base==fbtDouble	)return count<<4;

/*
			if(base==fbtString
			 ||base==fbtText
			 ||base==fbtImage
			 ||base==fbtAudio
			 ||base==fbtMovie	)return 0;				//未知长度
*/
			return -1;										//格式错误
		}

		FieldType::FieldType(const FieldBaseType &bt,int c)
		{
			base=bt;
			count=1;

			size=GetFieldSize(bt,c);
		}

		template<typename T>
		FieldType ParseStringToFieldType(const cdb_type_name_convert<T> typename_convert_list[],const T &left,const T &right,const T *str)
		{
			int n=0;
			int nl=0;
			const T *p;
			FieldType ct;

			p=hgl::strchr(str,left);

			if(p)
			{
				nl=p-str;
				if(*(p+1)==right)
				{
					ct.count=0;
				}
				else
				{
					stoi(p+1,ct.count);
				}
			}
			else
			{
				ct.count=1;
				nl=strlen(str);
			}

			while(typename_convert_list[n].type!=fbtNone)
			{
				const int len=strlen(typename_convert_list[n].name);

				if(nl==len)		//长度不一样，直接就不用比了
				{
					if(strcmp(typename_convert_list[n].name,str,len)==0)
					{
						ct.base=typename_convert_list[n].type;

						ct.size=GetFieldSize(ct.base,ct.count);

						return ct;
					}
				}

				n++;
			}

			ct.base=fbtNone;
			ct.count=-1;
			return ct;
		}

		FieldType ParseFieldType(const char *str)
		{
			return ParseStringToFieldType<char>(typename_convert_list_u8,'[',']',str);
		}

		FieldType ParseFieldType(const u16char *str)
		{
			return ParseStringToFieldType<u16char>(typename_convert_list_u16,U16_TEXT('['),U16_TEXT(']'),str);
		}

		bool ParseFieldType(FieldTypeList &ctl,const UTF16StringList &type_list)
		{
			for(int i=0;i<type_list.GetCount();i++)
			{
				type_list[i].LowerCase();

				FieldType ft=ParseFieldType(type_list[i].c_str());

				ctl.Add(ft);
			}

			return(true);
		}
#ifdef LOG_CDB_LOADER_LOG
		void CDBFiledOutputLog(int index,const db::CDBField *field)
		{
			LOG_INFO(UTF16String(u"Field[")+UTF16String(index)+UTF16String(u"]:")+
					field->Name+
					UTF16String(u" 基本类型: ")+UTF16String(typename_convert_list_u16[field->Type.base].name)+
					UTF16String(u" 数据个数: ")+UTF16String(field->Type.count));
		}
#endif//LOG_CDB_LOADER_LOG
	}//namespace db
}//namespace hgl
