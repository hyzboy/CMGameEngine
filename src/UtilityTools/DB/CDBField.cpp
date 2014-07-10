#include<hgl/db/CDBField.h>
#include<hgl/endian/Endian.h>
#include<hgl/type/BaseString.h>

namespace hgl
{
	namespace db
	{
		/**
		 * 固定长度数据字段
		 */
		template<typename T> class CDBFieldFixed:public CDBField
		{
			T *data;

		private:

			void LoadFieldData(io::DataInputStream *dis)
			{
				dis->ReadFully(data,Type.size*data_count);
			}

		public:

			CDBFieldFixed(const UTF16String &name,const FieldType &ft,io::DataInputStream *dis,int count):CDBField(name,ft)
			{
				data_count=count;
				data=new T[ft.count*count];

				LoadFieldData(dis);
			}

			~CDBFieldFixed()
			{
				SAFE_CLEAR_ARRAY(data);
			}

			bool	ToString	(int index,UTF16String &str)
			{
				if(index<0||index>=data_count)return(false);

				if(Type.base==fbtChar16le)
				{
					if(Type.count>1)
					{
 						str=UTF16String((const char16_t *)(data+Type.count*index),Type.count);
					}
					else
					{
						char16_t value;

						if(!GetChar(index,value))
							return(false);

						str=UTF16String(value);
					}

					return(true);
				}
				else
				if(Type.base==fbtChar8)
				{
					if(Type.count>1)
					{
						str=to_u16((const char *)(data+Type.count*index),Type.count);
					}
					else
					{
						char value;

						if(!GetChar(index,value))
							return(false);

						str=UTF16String(value);
					}

					return(true);
				}
				else
				if(Type.base==fbtBool)
				{
					if(Type.count>1)
					{
					}
					else
					{
						bool value;

						if(!GetBool(index,value))
							return(false);

						str=(value?u"YES":u"no");
					}

					return(true);
				}

				const T *p=data+Type.count*index;

				str=UTF16String(*p++);

				for(int i=1;i<Type.count;i++)
				{
					str+=u',';
					str+=UTF16String(*p++);
				}

				return(true);
			}

			template<typename V>
			bool GetData(int index,V &value)
			{
				if(index<0||index>=data_count||Type.base<fbtBool||Type.base>fbtDouble)return(false);

				value=data[index];
				return(true);
			}

			bool GetBool	(int index,bool &value){return GetData(index,value);}
			bool GetInteger	(int index,int &value){return GetData(index,value);}
			bool GetUInteger(int index,uint &value){return GetData(index,value);}
			bool GetFloat	(int index,float &value){return GetData(index,value);}
			bool GetDouble	(int index,double &value){return GetData(index,value);}
			bool GetChar	(int index,char &value){return GetData(index,value);}
			bool GetChar	(int index,char16_t &value){return GetData(index,value);}

			template<typename P>
			bool GetDataArray(int index,P *buf,int start,int number)
			{
				if(index<0||index>=data_count)return(false);

				if(start==-1&&number==-1)
				{
					start=0;
					number=Type.count;
				}
				else
				if(start<0||start>=Type.count||start+number>Type.count)return(false);

				memcpy(buf,data+Type.count*index+start,number*sizeof(P));
				return(true);
			}

			bool	GetArray	(int index,bool *	buf,	int start,int number){return(Type.base==fbtBool?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个布尔型数

			bool	GetArray	(int index,int8 *	buf,	int start,int number){return(Type.base==fbtInt8 ?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个8位有符号整型数
			bool	GetArray	(int index,int16 *	buf,	int start,int number){return(Type.base==fbtInt16?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个16位有符号整型数
			bool	GetArray	(int index,int32 *	buf,	int start,int number){return(Type.base==fbtInt32?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个32位有符号整型数
			bool	GetArray	(int index,int64 *	buf,	int start,int number){return(Type.base==fbtInt64?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个64位有符号整型数

			bool	GetArray	(int index,uint8 *	buf,	int start,int number){return(Type.base==fbtUint8 ?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个8位有符号整型数
			bool	GetArray	(int index,uint16 *	buf,	int start,int number){return(Type.base==fbtUint16?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个16位有符号整型数
			bool	GetArray	(int index,uint32 *	buf,	int start,int number){return(Type.base==fbtUint32?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个32位有符号整型数
			bool	GetArray	(int index,uint64 *	buf,	int start,int number){return(Type.base==fbtUint64?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个64位有符号整型数

			bool	GetArray	(int index,float *	buf,	int start,int number){return(Type.base==fbtFloat	?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个浮点数
			bool	GetArray	(int index,double *	buf,	int start,int number){return(Type.base==fbtDouble	?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个浮点数
			bool	GetArray	(int index,char *	buf,	int start,int number){return(Type.base==fbtChar8	?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个utf8字符
			bool	GetArray	(int index,char16_t *buf,	int start,int number){return(Type.base==fbtChar16le	?GetDataArray(index,buf,start,number):false);}			///<从字段中取得一个utf16字符

			template<typename V>
			int		FindData(const V &value)
			{
				if(Type.base<fbtInt8||Type.base>fbtFloat||Type.count!=1)return(-1);

				T *p=data;

				for(int i=0;i<data_count;i++)
					if(*p++==value)
						return(i);

				return(-1);
			}

			int		FindInteger		(const int		value){return FindData(value);}
			int		FindUInteger	(const uint		value){return FindData(value);}
			int		FindFloat		(const float 	value){return FindData(value);}
			int		FindDouble		(const double 	value){return FindData(value);}
			int		FindChar		(const char		value){return FindData(value);}
			int		FindChar		(const char16_t	value){return FindData(value);}

			template<typename P>
			int		FindDataArray	(const P *value)
			{
				const T *p=data;

				for(int i=0;i<data_count;i++)
				{
					if(!memcmp(value,p,Type.count*sizeof(T)))
						return i;

					p+=Type.count;
				}

				return(-1);
			}

			int		FindArray(const int8 *		buf){return(Type.base==fbtInt8 ?FindDataArray(buf):-1);}
			int		FindArray(const int16 *		buf){return(Type.base==fbtInt16?FindDataArray(buf):-1);}
			int		FindArray(const int32 *		buf){return(Type.base==fbtInt32?FindDataArray(buf):-1);}
			int		FindArray(const int64 *		buf){return(Type.base==fbtInt64?FindDataArray(buf):-1);}

			int		FindArray(const uint8 *		buf){return(Type.base==fbtUint8 ?FindDataArray(buf):-1);}
			int		FindArray(const uint16 *	buf){return(Type.base==fbtUint16?FindDataArray(buf):-1);}
			int		FindArray(const uint32 *	buf){return(Type.base==fbtUint32?FindDataArray(buf):-1);}
			int		FindArray(const uint64 *	buf){return(Type.base==fbtUint64?FindDataArray(buf):-1);}

			int		FindArray(const float *		buf){return(Type.base==fbtFloat?FindDataArray(buf):-1);}
			int		FindArray(const double *	buf){return(Type.base==fbtDouble?FindDataArray(buf):-1);}
			int		FindArray(const char *		buf){return(Type.base==fbtChar8?FindDataArray(buf):-1);}
			int		FindArray(const char16_t *	buf){return(Type.base==fbtChar16le?FindDataArray(buf):-1);}

			int		FindCharArray	(const char *str)
			{
				if(Type.base!=fbtChar8)return(-1);

				T *p=data;

				for(int i=0;i<data_count;i++)
				{
					if(!hgl::strcmp(str,p))
						return i;

					p+=Type.count;
				}

				return(-1);
			}

			int		FindCharArray	(const char16_t *str)
			{
				if(Type.base!=fbtChar16le)return(-1);

				T *p=data;

				for(int i=0;i<data_count;i++)
				{
					if(!hgl::strcmp(str,p))
						return i;

					p+=Type.count;
				}

				return(-1);
			}
		};//class FieldFixed

		template<> void CDBFieldFixed<int16		>::LoadFieldData(io::DataInputStream *dis){dis->ReadInt16		(data,Type.count*data_count);}
		template<> void CDBFieldFixed<uint16	>::LoadFieldData(io::DataInputStream *dis){dis->ReadUint16		(data,Type.count*data_count);}
		template<> void CDBFieldFixed<int32		>::LoadFieldData(io::DataInputStream *dis){dis->ReadInt32		(data,Type.count*data_count);}
		template<> void CDBFieldFixed<uint32	>::LoadFieldData(io::DataInputStream *dis){dis->ReadUint32		(data,Type.count*data_count);}
		template<> void CDBFieldFixed<int64		>::LoadFieldData(io::DataInputStream *dis){dis->ReadInt64		(data,Type.count*data_count);}
		template<> void CDBFieldFixed<uint64	>::LoadFieldData(io::DataInputStream *dis){dis->ReadUint64		(data,Type.count*data_count);}
		template<> void CDBFieldFixed<float		>::LoadFieldData(io::DataInputStream *dis){dis->ReadFloat		(data,Type.count*data_count);}
		template<> void CDBFieldFixed<double	>::LoadFieldData(io::DataInputStream *dis){dis->ReadDouble		(data,Type.count*data_count);}
		template<> void CDBFieldFixed<char		>::LoadFieldData(io::DataInputStream *dis){dis->ReadArrays<char>(data,Type.count*data_count);}
		template<> void CDBFieldFixed<char16_t	>::LoadFieldData(io::DataInputStream *dis){dis->ReadUTF16LEChars(data,Type.count*data_count);}
	}//namespace db

	namespace db
	{
		CDBField::CDBField(const UTF16String &name,const FieldType &ft)
		{
			Name=name;
			Type=ft;
		}

		CDBField *CreateCDBField(const UTF16String &name,const FieldType &ft,io::DataInputStream *dis,int count)
		{
			if(count<=0||!dis)
				return(nullptr);

			if(ft.base==fbtBool		)return(new CDBFieldFixed<bool		>(name,ft,dis,count));else
			if(ft.base==fbtInt8		)return(new CDBFieldFixed<int8		>(name,ft,dis,count));else
			if(ft.base==fbtInt16	)return(new CDBFieldFixed<int16		>(name,ft,dis,count));else
			if(ft.base==fbtInt32	)return(new CDBFieldFixed<int32		>(name,ft,dis,count));else
			if(ft.base==fbtInt64	)return(new CDBFieldFixed<int64		>(name,ft,dis,count));else
			if(ft.base==fbtUint8	)return(new CDBFieldFixed<uint8		>(name,ft,dis,count));else
			if(ft.base==fbtUint16	)return(new CDBFieldFixed<uint16	>(name,ft,dis,count));else
			if(ft.base==fbtUint32	)return(new CDBFieldFixed<uint32	>(name,ft,dis,count));else
			if(ft.base==fbtUint64	)return(new CDBFieldFixed<uint64	>(name,ft,dis,count));else
			if(ft.base==fbtFloat	)return(new CDBFieldFixed<float		>(name,ft,dis,count));else
			if(ft.base==fbtDouble	)return(new CDBFieldFixed<double	>(name,ft,dis,count));else
			if(ft.base==fbtChar8	)return(new CDBFieldFixed<char		>(name,ft,dis,count));else
			if(ft.base==fbtChar16le	)return(new CDBFieldFixed<char16_t	>(name,ft,dis,count));else
				return(nullptr);
		}
	}//namespace db
}//namespace hgl
