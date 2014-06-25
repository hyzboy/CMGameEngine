#include<hgl/io/DataInputStream.h>
#include<hgl/Other.h>

namespace hgl
{
	namespace io
	{
		template<char T>
		bool ReadUTF16Chars(char16_t *str,DataInputStream *dis,int count)
		{
			if(dis->ReadArrays<char16_t>(str,count)!=count)
				return(false);

			EndianSwap<char16_t>(str,count);
			str[count]=0;

			return(true);
		}

		template<>
		bool ReadUTF16Chars<HGL_ENDIAN>(char16_t *str,DataInputStream *dis,int count)
		{
			return(dis->ReadArrays<char16_t>(str,count)==count);
		}

		bool DataInputStream::ReadUTF16LEChars(char16_t *wstr,int count)
		{
			if(!in||!wstr||count<=0)return(false);

			return ReadUTF16Chars<HGL_LITTLE_ENDIAN>(wstr,this,count);
		}

		bool DataInputStream::ReadUTF16BEChars(char16_t *wstr,int count)
		{
			if(!in||!wstr||count<=0)return(false);

			return ReadUTF16Chars<HGL_BIG_ENDIAN>(wstr,this,count);
		}

		bool DataInputStream::ReadUTF8String(char *u8str,int max_len)
		{
			if(!u8str||max_len<=0)
				return(false);

			int32 count;

			if(!ReadInt32(count))
				return(false);

			if(count<=0)
			{
				*u8str=0;
				return(true);
			}

			if(max_len>=count)
			{
				if(ReadArrays<char>(u8str,count)!=count)
					return(false);

				u8str[count]=0;

				return(true);
			}

			if(ReadArrays<char>(u8str,max_len)!=max_len)
				return(false);

			Skip(count-max_len);
			return(true);
		}

		bool DataInputStream::ReadUTF8String(UTF8String &u8str,int max_len)
		{
			if(!in||max_len<0)return(false);

			int32 count;

			if(!ReadInt32(count))
				return(false);

			if(count<0)
				return(false);

			if(max_len>0)
			{
				if(count>max_len)
					return(false);
			}

			if(count==0)
			{
				u8str.Clear();
				return(true);
			}

			char *utf8_str=new char[count+1];

			if(!utf8_str)
				return(false);

			if(ReadArrays<char>(utf8_str,count)!=count)
			{
				delete[] utf8_str;
				return(false);
			}

			utf8_str[count]=0;

			u8str.Set(utf8_str,count,true);

			return(true);
		}

		bool DataInputStream::ReadUTF8String(UTF16String &wstr,int max_len)
		{
			if(!in||max_len<0)return(false);

			int32 count;

			if(!ReadInt32(count))
				return(false);

			if(count<0)
				return(false);

			if(max_len>0)
			{
				if(count>max_len)
					return(false);
			}

			if(count==0)
			{
				wstr.Clear();
				return(true);
			}

			SharedArray<char> utf8_str=ReadArrays<char>(count);

			if(!utf8_str)
				return(false);

			int wide_count;

			char16_t *wide_str=u8_to_u16(utf8_str,count,wide_count);

			wstr.Set(wide_str,wide_count,true);

			return(true);
		}

		template<char T>
		bool ReadUTF16String(char16_t *str,int max_len,DataInputStream *dis)
		{
			if(!dis)return(false);
			if(!str||max_len<=0)return(false);

			int32 count;

			if(!dis->ReadInt32(count))
				return(false);

			if(count<=0)
			{
				*str=0;
				return(true);
			}

			if(max_len>=count)
			{
				if(dis->ReadArrays<char16_t>(str,count)!=count)
					return(false);

				if(max_len>count)
					str[count]=0;
			}
			else
			{
				if(dis->ReadArrays<char16_t>(str,max_len)!=max_len)
					return(false);

				count=max_len;
			}

			UTF16CharConvert<T>::convert(str,count);

			return(true);
		}

		bool DataInputStream::ReadUTF16LEString(char16_t *str,int max_len)
		{
			if(!in)return(false);

			return ReadUTF16String<HGL_LITTLE_ENDIAN>(str,max_len,this);
		}

		bool DataInputStream::ReadUTF16BEString(char16_t *wstr,int max_len)
		{
			if(!in)return(false);

			return ReadUTF16String<HGL_BIG_ENDIAN>(wstr,max_len,this);
		}

		template<char T>
		bool ReadUTF16String(UTF16String &str,DataInputStream *dis,int max_len)
		{
			if(!dis||max_len<0)return(false);

			int32 count;

			if(!dis->ReadInt32(count))
				return(false);

			if(count<0)
				return(false);

			if(max_len>0)
			{
				if(count>max_len)
					return(false);
			}

			if(count==0)
			{
				str.Clear();
				return(true);
			}

			char16_t *utf16_str=new char16_t[count+1];

			if(!utf16_str)
				return(false);

			if(dis->ReadArrays<char16_t>(utf16_str,count)!=count)
			{
				delete[] utf16_str;
				return(false);
			}

			utf16_str[count]=0;

			UTF16CharConvert<T>::convert(utf16_str,count);

			str.Set(utf16_str,count,true);

			return(true);
		}

		bool DataInputStream::ReadUTF16LEString(UTF16String &wstr,int max_len)
		{
			if(!in)return(false);

			return ReadUTF16String<HGL_LITTLE_ENDIAN>(wstr,this,max_len);
		}

		bool DataInputStream::ReadUTF16BEString(UTF16String &wstr,int max_len)
		{
			if(!in)return(false);

			return ReadUTF16String<HGL_BIG_ENDIAN>(wstr,this,max_len);
		}
	}//namespace io
}//namespace hgl
