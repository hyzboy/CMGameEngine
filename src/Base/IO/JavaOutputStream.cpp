#include<hgl/io/JavaOutputStream.h>
#include<hgl/type/BaseString.h>
#include<hgl/LogInfo.h>
namespace hgl
{
	namespace io
	{
		bool JavaOutputStream::writeUTF(const UTF8String &utf8_str)
		{
			const uint32 utf8_len=utf8_str.Length();

			if(utf8_len>0xffff)
			{
				LOG_ERROR(OS_TEXT("Java String max length 65535"));
				return(false);
			}

			if(!out->WriteUint16(utf8_len))
				return(false);

			return(out->Write(utf8_str.c_str(),utf8_len)==utf8_len);
		}

		bool JavaOutputStream::writeUTF(const UTF16String &ws)
		{
			if(!out)
				return(false);

			return writeUTF(to_u8(ws));
		}
	}//namespace io
}//namespace hgl
