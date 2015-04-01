#include<hgl/io/DataOutputStream.h>
#include<hgl/Other.h>

namespace hgl
{
	namespace io	// write utf8 chars
	{
		template<> bool DataOutputStream::WriteUTF8Chars<char>		(const char *str,int64 size)
		{
			if(!out)return(false);
			if(size==0)return(true);

			if(!str||!*str||size<0)return(false);

			return(WriteArrays(str,size)==size);
		}

		template<> bool DataOutputStream::WriteUTF8Chars<u16char>	(const u16char *str,int64 size)
		{
			if(!out)return(false);
			if(size==0)return(true);

			if(!str||!*str||size<0)return(false);

			return WriteUTF8Chars(to_u8(str,size));
		}
	}

	namespace io	//write utf8 string
	{
		bool DataOutputStream::WriteUTF8String(const char *str,int32 length)
		{
			if(!out)return(false);

			if(length==-1)
				length=strlen(str);

			if(length<0)return(false);
			if(length&&!str)return(false);

			if(!WriteInt32(length))
				return(false);

			if(length<=0)
				return(true);

			return WriteUTF8Chars(str,length);
		}

		bool DataOutputStream::WriteUTF8String(const UTF16String &str)
		{
			if(!out)return(false);

			int32 utf8_count;
			char *utf8_str=u16_to_u8(str.c_str(),str.Length(),utf8_count);

			if(!WriteInt32(utf8_count))
				return(false);

			if(utf8_count<=0)
				return(true);

			return WriteArrays<char>(utf8_str,utf8_count)==utf8_count;
		}
	}

	namespace io	//write utf16 chars
	{
		template<char T>		//非特例化版本，用于需要swap的
		bool WriteUTF16Chars(DataOutputStream *dos,const u16char *str,int64 count)
		{
			SharedArray<u16char> swap_str=new u16char[count];

			EndianSwap<u16char>(swap_str,str,count);

			return(dos->WriteArrays<u16char>(swap_str,count)==count);
		}

		template<>				//特例化版本，用于直接写入的
		bool WriteUTF16Chars<HGL_ENDIAN>(DataOutputStream *dos,const u16char *str,int64 count)
		{
			return(dos->WriteArrays(str,count)==count);
		}
	}

	namespace io	//write utf16-le chars
	{
		template<> bool DataOutputStream::WriteUTF16LEChars<char>(const char *str,int64 length)
		{
			if(length==0)return(true);
			if(!out||!str||!*str||length<0)return(false);

			UTF16String u16_str=to_u16(str);

			return WriteUTF16Chars<HGL_LITTLE_ENDIAN>(this,u16_str.c_str(),u16_str.Length());
		}

		template<> bool DataOutputStream::WriteUTF16LEChars<u16char>(const u16char *str,int64 length)
		{
			if(length==0)return(true);
			if(!out||!str||!*str||length<0)return(false);

			return WriteUTF16Chars<HGL_LITTLE_ENDIAN>(this,str,length);
		}
	}

	namespace io	//write utf16-be chars
	{
		template<> bool DataOutputStream::WriteUTF16BEChars<char>(const char *str,int64 length)
		{
			if(length==0)return(true);
			if(!out||!str||!*str||length<0)return(false);

			UTF16String u16_str=to_u16(str,length);

			return WriteUTF16Chars<HGL_BIG_ENDIAN>(this,u16_str.c_str(),u16_str.Length());
		}

		template<> bool DataOutputStream::WriteUTF16BEChars<u16char>(const u16char *str,int64 length)
		{
			if(length==0)return(true);
			if(!out||!str||!*str||length<0)return(false);

			return WriteUTF16Chars<HGL_BIG_ENDIAN>(this,str,length);
		}
	}//namespace io

	namespace io	//write utf16-le string
	{
		bool DataOutputStream::WriteUTF16LEString(const UTF8String &str)
		{
			return WriteUTF16LEString(to_u16(str));
		}

		bool DataOutputStream::WriteUTF16LEString(const u16char *str,int32 len)
		{
			if(!out)return(false);

			if(!str)
			{
				len=0;
			}
			else
			{
				if(len<0)
					len=hgl::strlen(str);
			}

			if(!WriteInt32(len))
				return(false);

			if(len<=0)return(true);

			return WriteUTF16Chars<HGL_LITTLE_ENDIAN>(this,str,len);
		}

		bool DataOutputStream::WriteUTF16LEString(const UTF16String &str)
		{
			if(!out)return(false);

			return WriteUTF16LEString(str.c_str(),str.Length());
		}
	}

	namespace io	//write utf16-be string
	{
		bool DataOutputStream::WriteUTF16BEString(const UTF8String &str)
		{
			return WriteUTF16BEString(to_u16(str));
		}

		bool DataOutputStream::WriteUTF16BEString(const u16char *str,int32 len)
		{
			if(!out)return(false);

			if(!str)
			{
				len=0;
			}
			else
			{
				if(len<0)
					len=hgl::strlen(str);
			}

			if(!WriteInt32(len))
				return(false);

			if(len<=0)return(true);

			return WriteUTF16Chars<HGL_BIG_ENDIAN>(this,str,len);
		}

		bool DataOutputStream::WriteUTF16BEString(const UTF16String &str)
		{
			if(!out)return(false);

			return WriteUTF16BEString(str.c_str(),str.Length());
		}
	}//namespace io
}//namespace hgl
