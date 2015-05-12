#include<hgl/db/Field.h>
#include<hgl/db/FieldDesc.h>
#include<hgl/io/DataInputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/type/BaseString.h>

namespace hgl
{
	using namespace io;

	namespace db
	{
		/**
		 * 单一数据字段
		 */
		template<typename T> class FieldDataValue:public FieldData
		{
		public:

			T value;

		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldData::FieldData;
#else
			FieldDataValue(const FieldDesc *fd):FieldData(fd){}
#endif//HGL_CONSTRUCTION_REUSE
			FieldDataValue(const FieldDesc *fd,const T &v):FieldData(fd),value(v){}

			const bool Get(bool &v			)const HGL_OVERRIDE{v=value;return(true);}

			const bool Get(int8 &v			)const HGL_OVERRIDE{v=value;return(true);}
			const bool Get(int16 &v			)const HGL_OVERRIDE{v=value;return(true);}
			const bool Get(int32 &v			)const HGL_OVERRIDE{v=value;return(true);}
			const bool Get(int64 &v			)const HGL_OVERRIDE{v=value;return(true);}

			const bool Get(uint8 &v			)const HGL_OVERRIDE{v=value;return(true);}
			const bool Get(uint16 &v		)const HGL_OVERRIDE{v=value;return(true);}
			const bool Get(uint32 &v		)const HGL_OVERRIDE{v=value;return(true);}
			const bool Get(uint64 &v		)const HGL_OVERRIDE{v=value;return(true);}

			const bool Get(float &v			)const HGL_OVERRIDE{v=value;return(true);}
			const bool Get(double &v		)const HGL_OVERRIDE{v=value;return(true);}

			const bool Get(UTF16String &v	)const HGL_OVERRIDE{v=value;return(true);}
			const bool Get(UTF8String &v	)const HGL_OVERRIDE{v=value;return(true);}

			const bool Set(const bool v	)HGL_OVERRIDE{value=v;return(true);}

			const bool Set(const int8 v	)HGL_OVERRIDE{value=v;return(true);}
			const bool Set(const int16 v	)HGL_OVERRIDE{value=v;return(true);}
			const bool Set(const int32 v	)HGL_OVERRIDE{value=v;return(true);}
			const bool Set(const int64 v	)HGL_OVERRIDE{value=v;return(true);}

			const bool Set(const uint8 v	)HGL_OVERRIDE{value=v;return(true);}
			const bool Set(const uint16 v	)HGL_OVERRIDE{value=v;return(true);}
			const bool Set(const uint32 v	)HGL_OVERRIDE{value=v;return(true);}
			const bool Set(const uint64 v	)HGL_OVERRIDE{value=v;return(true);}

			const bool Set(const float v	)HGL_OVERRIDE{value=v;return(true);}
			const bool Set(const double v	)HGL_OVERRIDE{value=v;return(true);}
		};//template<typename T> class FieldDataValue:public FieldData

		class FieldDataBool:public FieldDataValue<bool>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldDataValue<bool>::FieldDataValue;
#else
			FieldDataBool(const FieldDesc *fd):FieldDataValue<bool>(fd){}
			FieldDataBool(const FieldDesc *fd,const bool v):FieldDataValue<bool>(fd,v){}
#endif//HGL_CONSTRUCTION_REUSE

			const bool Set(const UTF16String &str) HGL_OVERRIDE{return str.ToBool(this->value);}
			const bool Set(const UTF8String &str)  HGL_OVERRIDE{return str.ToBool(this->value);}
		};//class FieldDataBool:public FieldDataValue<bool>

		template<typename T> class FieldDataInteger:public FieldDataValue<T>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldDataValue<T>::FieldDataValue;
#else
			FieldDataInteger(const FieldDesc *fd):FieldDataValue<T>(fd){}
			FieldDataInteger(const FieldDesc *fd,const T &v):FieldDataValue<T>(fd,v){}
#endif//HGL_CONSTRUCTION_REUSE

			const bool Set(const UTF16String &str) HGL_OVERRIDE{return str.ToInt(this->value);}
			const bool Set(const UTF8String &str)  HGL_OVERRIDE{return str.ToInt(this->value);}
		};//template<typename T> class FieldDataInteger:public FieldDataValue<T>

		template<typename T> class FieldDataUInteger:public FieldDataValue<T>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldDataValue<T>::FieldDataValue;
#else
			FieldDataUInteger(const FieldDesc *fd):FieldDataValue<T>(fd){}
			FieldDataUInteger(const FieldDesc *fd,const T &v):FieldDataValue<T>(fd,v){}
#endif//HGL_CONSTRUCTION_REUSE

			const bool Set(const UTF16String &str) HGL_OVERRIDE{return str.ToUint(this->value);}
			const bool Set(const UTF8String &str)  HGL_OVERRIDE{return str.ToUint(this->value);}
		};//template<typename T> class FieldDataUInteger:public FieldDataValue<T>

		template<typename T> class FieldDataFloat:public FieldDataValue<T>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldDataValue<T>::FieldDataValue;
#else
			FieldDataFloat(const FieldDesc *fd):FieldDataValue<T>(fd){}
			FieldDataFloat(const FieldDesc *fd,const T &v):FieldDataValue<T>(fd,v){}
#endif//HGL_CONSTRUCTION_REUSE

			const bool Set(const UTF16String &str) HGL_OVERRIDE{return str.ToFloat(this->value);}
			const bool Set(const UTF8String &str)  HGL_OVERRIDE{return str.ToFloat(this->value);}
		};//template<typename T> class FieldDataFloat:public FieldDataValue<T>

		/**
		 * 定长阵列数据字段
		 */
		template<typename T> class FieldDataFixedArray:public FieldData
		{
		public:

			FixedList<T> array_value;

		public:

			FieldDataFixedArray(const FieldDesc *fd):FieldData(fd),array_value(this->desc->type.count){}

			FieldDataFixedArray(const FieldDesc *fd,const T *str,int str_length):FieldDataFixedArray(fd)
			{
				hgl_typecpy<T>(array_value.GetData(),str,hgl_min(this->desc->type.count,str_length));
			}

			FieldDataFixedArray(const FieldDesc *fd,const T *str):FieldDataFixedArray(fd,str,hgl::strlen(str)+1){}
			FieldDataFixedArray(const FieldDesc *fd,const BaseString<T> &str):FieldDataFixedArray(fd,str.c_str(),str.Length()+1){}

			const int Get(void *data,int size)const HGL_OVERRIDE
			{
				if(!data)return(-1);
				if(size<=0)return(-2);

				if(size>this->desc->type.count)
					size=this->desc->type.count;

				memcpy(data,this->array_value.GetData(),size);
				return size;
			}

			const bool Set(const void *data,int size) HGL_OVERRIDE
			{
				if(!data)return(false);
				if(size<0)return(false);

				if(size>this->desc->type.count)return(false);

				memcpy(this->array_value.GetData(),data,size);
				return(true);
			}
		};//template<typename T> class FieldDataFixedArray:public FieldData

		template<typename T> class FieldDataFixedString:public FieldDataFixedArray<T>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldDataFixedArray<T>::FieldDataFixedArray;
#else
			FieldDataFixedString(const FieldDesc *fd):FieldDataFixedArray<T>(fd){}
			FieldDataFixedString(const FieldDesc *fd,const T *str,int length):FieldDataFixedArray<T>(fd,str,length){}
			FieldDataFixedString(const FieldDesc *fd,const T *str):FieldDataFixedArray<T>(fd,str){}
			FieldDataFixedString(const FieldDesc *fd,const BaseString<T> &v):FieldDataFixedArray<T>(fd,v){}
#endif//HGL_CONSTRUCTION_REUSE

			const bool Get(bool &v		)const HGL_OVERRIDE{return stob(this->array_value.GetData(),v);}

			const bool Get(int8 &v		)const HGL_OVERRIDE{return stoi(this->array_value.GetData(),v);}
			const bool Get(int16 &v		)const HGL_OVERRIDE{return stoi(this->array_value.GetData(),v);}
			const bool Get(int32 &v		)const HGL_OVERRIDE{return stoi(this->array_value.GetData(),v);}
			const bool Get(int64 &v		)const HGL_OVERRIDE{return stoi(this->array_value.GetData(),v);}

			const bool Get(uint8 &v		)const HGL_OVERRIDE{return stou(this->array_value.GetData(),v);}
			const bool Get(uint16 &v	)const HGL_OVERRIDE{return stou(this->array_value.GetData(),v);}
			const bool Get(uint32 &v	)const HGL_OVERRIDE{return stou(this->array_value.GetData(),v);}
			const bool Get(uint64 &v	)const HGL_OVERRIDE{return stou(this->array_value.GetData(),v);}

			const bool Get(float &v		)const HGL_OVERRIDE{return etof(this->array_value.GetData(),v);}
			const bool Get(double &v	)const HGL_OVERRIDE{return etof(this->array_value.GetData(),v);}

			const int Get(T *str,int size)const HGL_OVERRIDE
			{
				if(!str||size<0)return(-1);

				if(size>this->desc->type.count)
					size=this->desc->type.count;

				hgl_typecpy<T>(str,this->array_value.GetData(),size);
				return(size);
			}

			const bool Set(const bool v		)HGL_OVERRIDE{*(this->array_value.GetData())=(v?'1':'0');return(true);}

			const bool Set(const int8 v		)HGL_OVERRIDE{return itos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const int16 v		)HGL_OVERRIDE{return itos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const int32 v		)HGL_OVERRIDE{return itos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const int64 v		)HGL_OVERRIDE{return itos(this->array_value.GetData(),this->desc->type.count,v);}

			const bool Set(const uint8 v		)HGL_OVERRIDE{return utos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const uint16 v		)HGL_OVERRIDE{return utos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const uint32 v		)HGL_OVERRIDE{return utos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const uint64 v		)HGL_OVERRIDE{return utos(this->array_value.GetData(),this->desc->type.count,v);}

			const bool Set(const float v		)HGL_OVERRIDE{return ftos(this->array_value.GetData(),3,this->desc->type.count,v);}
			const bool Set(const double v		)HGL_OVERRIDE{return ftos(this->array_value.GetData(),6,this->desc->type.count,v);}

			const bool Set(const T *str,int size) HGL_OVERRIDE
			{
				if(!str||size<0)return(false);

				hgl_typecpy<T>(this->array_value.GetData(),str,hgl_min(this->desc->type.count,size));
				return(true);
			}

			const bool Set(const BaseString<T> &str) HGL_OVERRIDE{return Set(str.c_str(),str.Length()+1);}
		};//template<typename T> class FieldDataFixedString:public FieldDataFixedArray<T>

		class FieldDataUTF16Chars:public FieldDataFixedString<u16char>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldDataFixedString<u16char>::FieldDataFixedString;
#else
			FieldDataUTF16Chars(const FieldDesc *fd):FieldDataFixedString<u16char>(fd){}
			FieldDataUTF16Chars(const FieldDesc *fd,const u16char *str,int length):FieldDataFixedString<u16char>(fd,str,length){}
			FieldDataUTF16Chars(const FieldDesc *fd,const u16char *str):FieldDataFixedString<u16char>(fd,str){}
			FieldDataUTF16Chars(const FieldDesc *fd,const UTF16String &v):FieldDataFixedString<u16char>(fd,v){}
#endif//HGL_CONSTRUCTION_REUSE

			const int Get(char *str,int size)const HGL_OVERRIDE
			{
				if(!str||size<=0)return(-1);

				return u16_to_u8(str,size,this->array_value.GetData(),this->desc->type.count);
			}

			const bool Get(UTF16String &str)const HGL_OVERRIDE
			{
				str.Strcpy(this->array_value.GetData(),this->desc->type.count);
				return(true);
			}

			const bool Get(UTF8String &str)const HGL_OVERRIDE
			{
				str=to_u8(this->array_value.GetData(),this->desc->type.count);
				return(true);
			}

			const bool Set(const char *str,int size) HGL_OVERRIDE
			{
				if(!str||size<0)return(false);

				u8_to_u16(this->array_value.GetData(),this->desc->type.count,str,size);
				return(true);
			}
		};//class FieldDataUTF16Chars:public FieldDataFixedArray<u16char>

		class FieldDataUTF8Chars:public FieldDataFixedString<char>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldDataFixedString<char>::FieldDataFixedString;
#else
			FieldDataUTF8Chars(const FieldDesc *fd):FieldDataFixedString<char>(fd){}
			FieldDataUTF8Chars(const FieldDesc *fd,const char *str,int length):FieldDataFixedString<char>(fd,str,length){}
			FieldDataUTF8Chars(const FieldDesc *fd,const char *str):FieldDataFixedString<char>(fd,str){}
			FieldDataUTF8Chars(const FieldDesc *fd,const UTF8String &v):FieldDataFixedString<char>(fd,v){}
#endif//HGL_CONSTRUCTION_REUSE

			const int Get(u16char *str,int size)const HGL_OVERRIDE
			{
				if(!str||size<=0)return(-1);

				return u8_to_u16(str,size,this->array_value.GetData(),this->desc->type.count);
			}

			const bool Get(UTF16String &str)const HGL_OVERRIDE
			{
				str=to_u16(this->array_value.GetData(),this->desc->type.count);
				return(true);
			}

			const bool Get(UTF8String &str)const HGL_OVERRIDE
			{
				str.Strcpy(this->array_value.GetData(),this->desc->type.count);
				return(true);
			}

			const bool Set(const u16char *str,int size) HGL_OVERRIDE
			{
				if(!str||size<0)return(false);

				u16_to_u8(this->array_value.GetData(),this->desc->type.count,str,size);
				return(true);
			}
		};//class FieldDataUTF8Chars:public FieldDataFixedArray<char>

		/**
		 * 定长二进制串字段类
		 */
		class FieldDataBinary:public FieldDataFixedArray<char>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldDataFixedArray<char>::FieldDataFixedArray;
#else
			FieldDataBinary(const FieldDesc *fd):FieldDataFixedArray<char>(fd){}
			FieldDataBinary(const FieldDesc *fd,const char *str,int length):FieldDataFixedArray<char>(fd,str,length){}
#endif//HGL_CONSTRUCTION_REUSE

			const int Get(char *str,int size)const HGL_OVERRIDE
			{
				if(!str||size<0)return(-1);

				if(size>this->desc->type.count)
					size=this->desc->type.count;

				hgl_typecpy<char>(str,this->array_value.GetData(),size);
				return(size);
			}

			const int Get(u16char *str,int size)const HGL_OVERRIDE
			{
				if(!str||size<0)return(false);

				size*=sizeof(u16char);

				if(size>this->desc->type.count)
					size=this->desc->type.count;

				memcpy(str,this->array_value.GetData(),size);
				return(size/sizeof(u16char));
			}

			const bool Get(UTF16String &str)const HGL_OVERRIDE
			{
				str.Strcpy((u16char *)(this->array_value.GetData()),this->desc->type.count/sizeof(u16char));
				return(true);
			}

			const bool Get(UTF8String &str)const HGL_OVERRIDE
			{
				str.Strcpy(this->array_value.GetData(),this->desc->type.count);
				return(true);
			}

			const bool Set(const char *str,	int size) HGL_OVERRIDE
			{
				if(size>this->desc->type.count)
					size=this->desc->type.count;

				memcpy(this->array_value.GetData(),str,size);
				return(true);
			}

			const bool Set(const u16char *str,int size) HGL_OVERRIDE
			{
				size<<=1;

				if(size>this->desc->type.count)
					size=this->desc->type.count;

				memcpy(this->array_value.GetData(),str,size);
				return(true);
			}

			const bool Set(const UTF16String &str) HGL_OVERRIDE
			{
				return Set(str.c_str(),str.Length()+1);
			}

			const bool Set(const UTF8String &str) HGL_OVERRIDE
			{
				return Set(str.c_str(),str.Length()+1);
			}
		};//class FieldDataBinary:public FieldDataFixedArray<uchar>

		/**
		 * 字符串数据字段
		 */
		template<typename T> class FieldDataString:public FieldData
		{
		public:

			BaseString<T> string_value;

		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldData::FieldData;
#else
			FieldDataString(const FieldDesc *fd):FieldData(fd){}
#endif//HGL_CONSTRUCTION_REUSE
			FieldDataString(const FieldDesc *fd,const T *str,int len):FieldData(fd),string_value(str,len){}
			FieldDataString(const FieldDesc *fd,const T *str):FieldData(fd),string_value(str){}
			FieldDataString(const FieldDesc *fd,const BaseString<T> &str):FieldData(fd),string_value(str){}

			const bool Get(bool &v		)const HGL_OVERRIDE{return string_value.ToBool(v);}

			const bool Get(int8 &v		)const HGL_OVERRIDE{return string_value.ToInt(v);}
			const bool Get(int16 &v		)const HGL_OVERRIDE{return string_value.ToInt(v);}
			const bool Get(int32 &v		)const HGL_OVERRIDE{return string_value.ToInt(v);}
			const bool Get(int64 &v		)const HGL_OVERRIDE{return string_value.ToInt(v);}

			const bool Get(uint8 &v		)const HGL_OVERRIDE{return string_value.ToUint(v);}
			const bool Get(uint16 &v	)const HGL_OVERRIDE{return string_value.ToUint(v);}
			const bool Get(uint32 &v	)const HGL_OVERRIDE{return string_value.ToUint(v);}
			const bool Get(uint64 &v	)const HGL_OVERRIDE{return string_value.ToUint(v);}

			const bool Get(float &v		)const HGL_OVERRIDE{return string_value.ToFloat(v);}
			const bool Get(double &v	)const HGL_OVERRIDE{return string_value.ToFloat(v);}

			const int Get(T *str,int size)const HGL_OVERRIDE
			{
				if(!str||size<0)return(false);

				if(size>string_value.Length())
					size=string_value.Length();

				hgl_typecpy<T>(str,string_value.c_str(),size);
				return(size);
			}

			const bool Get(BaseString<T> &str)const HGL_OVERRIDE
			{
				str=this->string_value;
				return(true);
			}

			const int Get(void *data,int size)const HGL_OVERRIDE
			{
				if(!data)return(-1);
				if(size<=0)return(-2);

				if(size/sizeof(T)>string_value.Length()+1)
					size=(string_value.Length()+1)*sizeof(T);

				memcpy(data,string_value.c_str(),size);
				return size;
			}

			const bool Set(const int8 v		)HGL_OVERRIDE{string_value=v;return(true);}
			const bool Set(const int16 v		)HGL_OVERRIDE{string_value=v;return(true);}
			const bool Set(const int32 v		)HGL_OVERRIDE{string_value=v;return(true);}
			const bool Set(const int64 v		)HGL_OVERRIDE{string_value=v;return(true);}

			const bool Set(const uint8 v		)HGL_OVERRIDE{string_value=v;return(true);}
			const bool Set(const uint16 v		)HGL_OVERRIDE{string_value=v;return(true);}
			const bool Set(const uint32 v		)HGL_OVERRIDE{string_value=v;return(true);}
			const bool Set(const uint64 v		)HGL_OVERRIDE{string_value=v;return(true);}

			const bool Set(const float v		)HGL_OVERRIDE{string_value=v;return(true);}
			const bool Set(const double v		)HGL_OVERRIDE{string_value=v;return(true);}

			const bool Set(const T *str,int size)HGL_OVERRIDE
			{
				if(!str||size<0)
					return(false);

				string_value.Set(str,size);
				return(true);
			}

			const bool Set(const BaseString<T> &str) HGL_OVERRIDE
			{
				string_value=str;
				return(true);
			}

			const bool Set(const void *data,int size) HGL_OVERRIDE
			{
				if(!data)return(false);
				if(size<0)return(false);

				string_value.Set((T *)data,size/sizeof(T));
				return(true);
			}
		};//template<typename T> class FieldDataString:public FieldData

		/**
		 * UTF16LE字符串数据字段
		 */
		class FieldDataUTF16String:public FieldDataString<u16char>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldDataString<u16char>::FieldDataString;
#else
			FieldDataUTF16String(const FieldDesc *fd):FieldDataString<u16char>(fd){}
			FieldDataUTF16String(const FieldDesc *fd,const u16char *str,int len):FieldDataString<u16char>(fd,str,len){}
			FieldDataUTF16String(const FieldDesc *fd,const u16char *str):FieldDataString<u16char>(fd,str){}
			FieldDataUTF16String(const FieldDesc *fd,const BaseString<u16char> &str):FieldDataString<u16char>(fd,str){}
#endif//HGL_CONSTRUCTION_REUSE

			const bool Set(const bool v)HGL_OVERRIDE{ this->string_value = (v ? U16_TEXT('1') : U16_TEXT('0')); return(true); }

			const bool Get(UTF8String &str)const HGL_OVERRIDE{str=to_u8(this->string_value);return(true);}
			const int Get(char *str,int size)const HGL_OVERRIDE
			{
				if(!str||size<=0)return(false);

				return u16_to_u8(str,size,this->string_value.c_str(),this->string_value.Length()+1);
			}

			const bool Set(const UTF8String &str)HGL_OVERRIDE{this->string_value=to_u16(str);return(true);}
			const bool Set(const char *str,int size)HGL_OVERRIDE
			{
				if(!str||size<=0)return(false);

				this->string_value=to_u16(str,size);
				return(true);
			}
		};//class FieldDataUTF16String:public FieldDataString<u16char>

		/**
		 * UTF8字符串数据字段
		 */
		class FieldDataUTF8String:public FieldDataString<char>
		{
		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldDataString<char>::FieldDataString;
#else
			FieldDataUTF8String(const FieldDesc *fd):FieldDataString<char>(fd){}
			FieldDataUTF8String(const FieldDesc *fd,const char *str,int len):FieldDataString<char>(fd,str,len){}
			FieldDataUTF8String(const FieldDesc *fd,const char *str):FieldDataString<char>(fd,str){}
			FieldDataUTF8String(const FieldDesc *fd,const UTF8String &str):FieldDataString<char>(fd,str){}
#endif//HGL_CONSTRUCTION_REUSE

			const bool Set(const bool v		)HGL_OVERRIDE{this->string_value=(v?'1':'0');return(true);}

			const bool Get(UTF16String &str)const HGL_OVERRIDE{str=to_u16(this->string_value);return(true);}
			const int Get(u16char *str,int size)const HGL_OVERRIDE
			{
				if(!str||size<=0)return(false);

				return u8_to_u16(str,size,this->string_value.c_str(),this->string_value.Length()+1);
			}

			const bool Set(const UTF8String &str)HGL_OVERRIDE{this->string_value=str;return(true);}
			const bool Set(const u16char *str,int size) HGL_OVERRIDE
			{
				if(!str||size<=0)return(false);

				this->string_value=to_u8(str,size);
				return(true);
			}
		};//class FieldDataUTF8String:public FieldDataString<char>

		/**
		 * 可变长度二进制数据字段
		 */
		class FieldDataVarBinary:public FieldData
		{
		public:

			MemBlock<char> mem_block;

		public:

#ifdef HGL_CONSTRUCTION_REUSE
			using FieldData::FieldData;
#else
			FieldDataVarBinary(const FieldDesc *fd):FieldData(fd){}
#endif//HGL_CONSTRUCTION_REUSE

			const bool Get(UTF16String &str)const HGL_OVERRIDE{str.Set((u16char *)(mem_block.GetData()),mem_block.GetBytes()/sizeof(u16char));return(true);}
			const bool Get(UTF8String &str)const HGL_OVERRIDE{str.Set(mem_block.GetData(),mem_block.GetBytes());return(true);}
			const int  Get(void *data,int size)const HGL_OVERRIDE
			{
				if(size>mem_block.GetBytes())
					size=mem_block.GetBytes();

				memcpy(data,mem_block.GetData(),size);
				return(size);
			}

			const int Get(char *data,int size)const HGL_OVERRIDE
			{
				return Get((void *)data,size);
			}

			const int Get(u16char *data,int size)const HGL_OVERRIDE
			{
				return Get((void *)data,size*sizeof(u16char));
			}

			const bool Set(const void *data,int size)HGL_OVERRIDE
			{
				if(!data||size<0)return(false);

				mem_block.SetLength(size);
				memcpy(mem_block.GetData(),data,size);
				return(true);
			}

			const bool Set(const char *data,int size)HGL_OVERRIDE
			{
				if(!data||size<0)return(false);

				return Set((void *)data,size);
			}

			const bool Set(const u16char *data,int size)HGL_OVERRIDE
			{
				if(!data||size<0)return(false);

				return Set((void *)data,size*sizeof(u16char));
			}

			const bool Set(const UTF16String &str	)HGL_OVERRIDE{return Set(str.c_str(),str.Length()+1);}
			const bool Set(const UTF8String &str	)HGL_OVERRIDE{return Set(str.c_str(),str.Length()+1);}
		};//class FieldDataVarBinary:public FieldData
	}//namespace db
}//namespace hgl
