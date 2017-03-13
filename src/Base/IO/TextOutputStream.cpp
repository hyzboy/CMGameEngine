#include<hgl/io/TextOutputStream.h>
#include<hgl/io/DataOutputStream.h>
namespace hgl
{
    namespace io
    {
        template<> EndianTextOutputStream<bomUTF8>::EndianTextOutputStream(OutputStream *os):TextOutputStream(bomUTF8,new DirectDataOutputStream(os)){}
        template<> EndianTextOutputStream<bomUTF16LE>::EndianTextOutputStream(OutputStream *os):TextOutputStream(bomUTF16LE,new LEDataOutputStream(os)){}
        template<> EndianTextOutputStream<bomUTF16BE>::EndianTextOutputStream(OutputStream *os):TextOutputStream(bomUTF16BE,new BEDataOutputStream(os)){}
    }//namespace io

    namespace io        //WriteChars函数
    {
        template<> bool EndianTextOutputStream<bomUTF8>::WriteChars(const char *str,int64 size)
        {
            return out?out->WriteUTF8Chars(str,size):false;
        }

        template<> bool EndianTextOutputStream<bomUTF8>::WriteChars(const u16char *str,int64 size)
        {
            return out?out->WriteUTF8Chars(str,size):false;
        }

        template<> bool EndianTextOutputStream<bomUTF16LE>::WriteChars(const char *str,int64 size)
        {
            return out?out->WriteUTF16LEChars(str,size):false;
        }

        template<> bool EndianTextOutputStream<bomUTF16LE>::WriteChars(const u16char *str,int64 size)
        {
            return out?out->WriteUTF16LEChars(str,size):false;
        }

        template<> bool EndianTextOutputStream<bomUTF16BE>::WriteChars(const char *str,int64 size)
        {
            return out?out->WriteUTF16BEChars(str,size):false;
        }

        template<> bool EndianTextOutputStream<bomUTF16BE>::WriteChars(const u16char *str,int64 size)
        {
            return out?out->WriteUTF16BEChars(str,size):false;
        }
    }//namespace io
}//namespace hgl
