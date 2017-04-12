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

			using FieldData::FieldData;
			FieldDataValue(const FieldDesc *fd,const T &v):FieldData(fd),value(v){}

			const bool Get(bool &v			)const override{v=value;return(true);}

			const bool Get(int8 &v			)const override{v=value;return(true);}
			const bool Get(int16 &v			)const override{v=value;return(true);}
			const bool Get(int32 &v			)const override{v=value;return(true);}
			const bool Get(int64 &v			)const override{v=value;return(true);}

			const bool Get(uint8 &v			)const override{v=value;return(true);}
			const bool Get(uint16 &v		)const override{v=value;return(true);}
			const bool Get(uint32 &v		)const override{v=value;return(true);}
			const bool Get(uint64 &v		)const override{v=value;return(true);}

			const bool Get(float &v			)const override{v=value;return(true);}
			const bool Get(double &v		)const override{v=value;return(true);}

			const bool Get(UTF16String &v	)const override{v=value;return(true);}
			const bool Get(UTF8String &v	)const override{v=value;return(true);}

			const bool Set(const bool v	)override{value=v;return(true);}

			const bool Set(const int8 v	)override{value=v;return(true);}
			const bool Set(const int16 v	)override{value=v;return(true);}
			const bool Set(const int32 v	)override{value=v;return(true);}
			const bool Set(const int64 v	)override{value=v;return(true);}

			const bool Set(const uint8 v	)override{value=v;return(true);}
			const bool Set(const uint16 v	)override{value=v;return(true);}
			const bool Set(const uint32 v	)override{value=v;return(true);}
			const bool Set(const uint64 v	)override{value=v;return(true);}

			const bool Set(const float v	)override{value=v;return(true);}
			const bool Set(const double v	)override{value=v;return(true);}
		};//template<typename T> class FieldDataValue:public FieldData

		class FieldDataBool:public FieldDataValue<bool>
		{
		public:

			using FieldDataValue<bool>::FieldDataValue;

			const bool Set(const UTF16String &str) override{return str.ToBool(this->value);}
			const bool Set(const UTF8String &str)  override{return str.ToBool(this->value);}
		};//class FieldDataBool:public FieldDataValue<bool>

		template<typename T> class FieldDataInteger:public FieldDataValue<T>
		{
		public:

			using FieldDataValue<T>::FieldDataValue;

			const bool Set(const UTF16String &str) override{return str.ToInt(this->value);}
			const bool Set(const UTF8String &str)  override{return str.ToInt(this->value);}
		};//template<typename T> class FieldDataInteger:public FieldDataValue<T>

		template<typename T> class FieldDataUInteger:public FieldDataValue<T>
		{
		public:

			using FieldDataValue<T>::FieldDataValue;

			const bool Set(const UTF16String &str) override{return str.ToUint(this->value);}
			const bool Set(const UTF8String &str)  override{return str.ToUint(this->value);}
		};//template<typename T> class FieldDataUInteger:public FieldDataValue<T>

		template<typename T> class FieldDataFloat:public FieldDataValue<T>
		{
		public:

			using FieldDataValue<T>::FieldDataValue;

			const bool Set(const UTF16String &str) override{return str.ToFloat(this->value);}
			const bool Set(const UTF8String &str)  override{return str.ToFloat(this->value);}
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

			const int Get(void *data,int size)const override
			{
				if(!data)return(-1);
				if(size<=0)return(-2);

				if(size>this->desc->type.count)
					size=this->desc->type.count;

				memcpy(data,this->array_value.GetData(),size);
				return size;
			}

			const bool Set(const void *data,int size) override
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

			using FieldDataFixedArray<T>::FieldDataFixedArray;

			const bool Get(bool &v		)const override{return stob(this->array_value.GetData(),v);}

			const bool Get(int8 &v		)const override{return stoi(this->array_value.GetData(),v);}
			const bool Get(int16 &v		)const override{return stoi(this->array_value.GetData(),v);}
			const bool Get(int32 &v		)const override{return stoi(this->array_value.GetData(),v);}
			const bool Get(int64 &v		)const override{return stoi(this->array_value.GetData(),v);}

			const bool Get(uint8 &v		)const override{return stou(this->array_value.GetData(),v);}
			const bool Get(uint16 &v	)const override{return stou(this->array_value.GetData(),v);}
			const bool Get(uint32 &v	)const override{return stou(this->array_value.GetData(),v);}
			const bool Get(uint64 &v	)const override{return stou(this->array_value.GetData(),v);}

			const bool Get(float &v		)const override{return etof(this->array_value.GetData(),v);}
			const bool Get(double &v	)const override{return etof(this->array_value.GetData(),v);}

			const int Get(T *str,int size)const override
			{
				if(!str||size<0)return(-1);

				if(size>this->desc->type.count)
					size=this->desc->type.count;

				hgl_typecpy<T>(str,this->array_value.GetData(),size);
				return(size);
			}

			const bool Set(const bool v		)override{*(this->array_value.GetData())=(v?'1':'0');return(true);}

			const bool Set(const int8 v		)override{return itos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const int16 v		)override{return itos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const int32 v		)override{return itos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const int64 v		)override{return itos(this->array_value.GetData(),this->desc->type.count,v);}

			const bool Set(const uint8 v		)override{return utos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const uint16 v		)override{return utos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const uint32 v		)override{return utos(this->array_value.GetData(),this->desc->type.count,v);}
			const bool Set(const uint64 v		)override{return utos(this->array_value.GetData(),this->desc->type.count,v);}

			const bool Set(const float v		)override{return ftos(this->array_value.GetData(),3,this->desc->type.count,v);}
			const bool Set(const double v		)override{return ftos(this->array_value.GetData(),6,this->desc->type.count,v);}

			const bool Set(const T *str,int size) override
			{
				if(!str||size<0)return(false);

				hgl_typecpy<T>(this->array_value.GetData(),str,hgl_min(this->desc->type.count,size));
				return(true);
			}

			const bool Set(const BaseString<T> &str) override{return Set(str.c_str(),str.Length()+1);}
		};//template<typename T> class FieldDataFixedString:public FieldDataFixedArray<T>

		class FieldDataUTF16Chars:public FieldDataFixedString<u16char>
		{
		public:

			using FieldDataFixedString<u16char>::FieldDataFixedString;

			const int Get(char *str,int size)const override
			{
				if(!str||size<=0)return(-1);

				return u16_to_u8(str,size,this->array_value.GetData(),this->desc->type.count);
			}

			const bool Get(UTF16String &str)const override
			{
				str.Strcpy(this->array_value.GetData(),this->desc->type.count);
				return(true);
			}

			const bool Get(UTF8String &str)const override
			{
				str=to_u8(this->array_value.GetData(),this->desc->type.count);
				return(true);
			}

			const bool Set(const char *str,int size) override
			{
				if(!str||size<0)return(false);

				u8_to_u16(this->array_value.GetData(),this->desc->type.count,str,size);
				return(true);
			}
		};//class FieldDataUTF16Chars:public FieldDataFixedArray<u16char>

		class FieldDataUTF8Chars:public FieldDataFixedString<char>
		{
		public:

			using FieldDataFixedString<char>::FieldDataFixedString;

			const int Get(u16char *str,int size)const override
			{
				if(!str||size<=0)return(-1);

				return u8_to_u16(str,size,this->array_value.GetData(),this->desc->type.count);
			}

			const bool Get(UTF16String &str)const override
			{
				str=to_u16(this->array_value.GetData(),this->desc->type.count);
				return(true);
			}

			const bool Get(UTF8String &str)const override
			{
				str.Strcpy(this->array_value.GetData(),this->desc->type.count);
				return(true);
			}

			const bool Set(const u16char *str,int size) override
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

			using FieldDataFixedArray<char>::FieldDataFixedArray;

			const int Get(char *str,int size)const override
			{
				if(!str||size<0)return(-1);

				if(size>this->desc->type.count)
					size=this->desc->type.count;

				hgl_typecpy<char>(str,this->array_value.GetData(),size);
				return(size);
			}

			const int Get(u16char *str,int size)const override
			{
				if(!str||size<0)return(false);

				size*=sizeof(u16char);

				if(size>this->desc->type.count)
					size=this->desc->type.count;

				memcpy(str,this->array_value.GetData(),size);
				return(size/sizeof(u16char));
			}

			const bool Get(UTF16String &str)const override
			{
				str.Strcpy((u16char *)(this->array_value.GetData()),this->desc->type.count/sizeof(u16char));
				return(true);
			}

			const bool Get(UTF8String &str)const override
			{
				str.Strcpy(this->array_value.GetData(),this->desc->type.count);
				return(true);
			}

			const bool Set(const char *str,	int size) override
			{
				if(size>this->desc->type.count)
					size=this->desc->type.count;

				memcpy(this->array_value.GetData(),str,size);
				return(true);
			}

			const bool Set(const u16char *str,int size) override
			{
				size<<=1;

				if(size>this->desc->type.count)
					size=this->desc->type.count;

				memcpy(this->array_value.GetData(),str,size);
				return(true);
			}

			const bool Set(const UTF16String &str) override
			{
				return Set(str.c_str(),str.Length()+1);
			}

			const bool Set(const UTF8String &str) override
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

			using FieldData::FieldData;
            
			FieldDataString(const FieldDesc *fd,const T *str,int len):FieldData(fd),string_value(str,len){}
			FieldDataString(const FieldDesc *fd,const T *str):FieldData(fd),string_value(str){}
			FieldDataString(const FieldDesc *fd,const BaseString<T> &str):FieldData(fd),string_value(str){}

			const bool Get(bool &v		)const override{return string_value.ToBool(v);}

			const bool Get(int8 &v		)const override{return string_value.ToInt(v);}
			const bool Get(int16 &v		)const override{return string_value.ToInt(v);}
			const bool Get(int32 &v		)const override{return string_value.ToInt(v);}
			const bool Get(int64 &v		)const override{return string_value.ToInt(v);}

			const bool Get(uint8 &v		)const override{return string_value.ToUint(v);}
			const bool Get(uint16 &v	)const override{return string_value.ToUint(v);}
			const bool Get(uint32 &v	)const override{return string_value.ToUint(v);}
			const bool Get(uint64 &v	)const override{return string_value.ToUint(v);}

			const bool Get(float &v		)const override{return string_value.ToFloat(v);}
			const bool Get(double &v	)const override{return string_value.ToFloat(v);}

			const int Get(T *str,int size)const override
			{
				if(!str||size<0)return(false);

				if(size>string_value.Length())
					size=string_value.Length();

				hgl_typecpy<T>(str,string_value.c_str(),size);
				return(size);
			}

			const bool Get(BaseString<T> &str)const override
			{
				str=this->string_value;
				return(true);
			}

			const int Get(void *data,int size)const override
			{
				if(!data)return(-1);
				if(size<=0)return(-2);

				if(size/sizeof(T)>string_value.Length()+1)
					size=(string_value.Length()+1)*sizeof(T);

				memcpy(data,string_value.c_str(),size);
				return size;
			}

			const bool Set(const int8 v		)override{string_value=v;return(true);}
			const bool Set(const int16 v		)override{string_value=v;return(true);}
			const bool Set(const int32 v		)override{string_value=v;return(true);}
			const bool Set(const int64 v		)override{string_value=v;return(true);}

			const bool Set(const uint8 v		)override{string_value=v;return(true);}
			const bool Set(const uint16 v		)override{string_value=v;return(true);}
			const bool Set(const uint32 v		)override{string_value=v;return(true);}
			const bool Set(const uint64 v		)override{string_value=v;return(true);}

			const bool Set(const float v		)override{string_value=v;return(true);}
			const bool Set(const double v		)override{string_value=v;return(true);}

			const bool Set(const T *str,int size)override
			{
				if(!str||size<0)
					return(false);

				string_value.Set(str,size);
				return(true);
			}

			const bool Set(const BaseString<T> &str) override
			{
				string_value=str;
				return(true);
			}

			const bool Set(const void *data,int size) override
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

			using FieldDataString<u16char>::FieldDataString;

			const bool Set(const bool v)override{ this->string_value = (v ? U16_TEXT('1') : U16_TEXT('0')); return(true); }

			const bool Get(UTF8String &str)const override{str=to_u8(this->string_value);return(true);}
			const int Get(char *str,int size)const override
			{
				if(!str||size<=0)return(false);

				return u16_to_u8(str,size,this->string_value.c_str(),this->string_value.Length()+1);
			}

			const bool Set(const UTF8String &str)override{this->string_value=to_u16(str);return(true);}
			const bool Set(const char *str,int size)override
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

			using FieldDataString<char>::FieldDataString;

			const bool Set(const bool v		)override{this->string_value=(v?'1':'0');return(true);}

			const bool Get(UTF16String &str)const override{str=to_u16(this->string_value);return(true);}
			const int Get(u16char *str,int size)const override
			{
				if(!str||size<=0)return(false);

				return u8_to_u16(str,size,this->string_value.c_str(),this->string_value.Length()+1);
			}

			const bool Set(const UTF8String &str)override{this->string_value=str;return(true);}
			const bool Set(const u16char *str,int size) override
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

			using FieldData::FieldData;

			const bool Get(UTF16String &str)const override{str.Set((u16char *)(mem_block.GetData()),mem_block.GetBytes()/sizeof(u16char));return(true);}
			const bool Get(UTF8String &str)const override{str.Set(mem_block.GetData(),mem_block.GetBytes());return(true);}
			const int  Get(void *data,int size)const override
			{
				if(size>mem_block.GetBytes())
					size=mem_block.GetBytes();

				memcpy(data,mem_block.GetData(),size);
				return(size);
			}

			const int Get(char *data,int size)const override
			{
				return Get((void *)data,size);
			}

			const int Get(u16char *data,int size)const override
			{
				return Get((void *)data,size*sizeof(u16char));
			}

			const bool Set(const void *data,int size)override
			{
				if(!data||size<0)return(false);

				mem_block.SetLength(size);
				memcpy(mem_block.GetData(),data,size);
				return(true);
			}

			const bool Set(const char *data,int size)override
			{
				if(!data||size<0)return(false);

				return Set((void *)data,size);
			}

			const bool Set(const u16char *data,int size)override
			{
				if(!data||size<0)return(false);

				return Set((void *)data,size*sizeof(u16char));
			}

			const bool Set(const UTF16String &str	)override{return Set(str.c_str(),str.Length()+1);}
			const bool Set(const UTF8String &str	)override{return Set(str.c_str(),str.Length()+1);}
		};//class FieldDataVarBinary:public FieldData
	}//namespace db
}//namespace hgl
