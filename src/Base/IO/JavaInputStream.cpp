#include<hgl/io/JavaInputStream.h>
#include<hgl/CodePage.h>
namespace hgl
{
    namespace io
    {
        bool JavaInputStream::readUTF(UTF16String &ws)
        {
            if(!in)return(false);

            uint16 utf8_length;

            if(!in->ReadUint16(utf8_length))
                return(false);

            SharedArray<char> utf8_str=new char[utf8_length];

            int wide_len;

            if(in->Read(utf8_str,utf8_length)!=utf8_length)
                return(false);

            u16char *wide_str=u8_to_u16(utf8_str,utf8_length,wide_len);

            ws.Set(wide_str,wide_len,true);

            return(true);
        }
    }//namespace io
}//namespace hgl
