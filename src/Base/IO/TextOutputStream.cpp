#include<hgl/io/TextOutputStream.h>
#include<hgl/io/DataOutputStream.h>
namespace hgl
{
	namespace io
	{
		template<> TextOutputStream<bomUTF8>::TextOutputStream(OutputStream *os)
		{
			out=new DirectDataOutputStream(os);
		}

		template<> TextOutputStream<bomUTF16LE>::TextOutputStream(OutputStream *os)
		{
			out=new LEDataOutputStream(os);
		}

		template<> TextOutputStream<bomUTF16BE>::TextOutputStream(OutputStream *os)
		{
			out=new BEDataOutputStream(os);
		}
	}//namespace io

	namespace io		//WriteChars函数
	{
		template<> bool TextOutputStream<bomUTF8>::WriteChars(const char *str,int64 size)
		{
			return out?out->WriteUTF8Chars(str,size):false;
		}

		template<> bool TextOutputStream<bomUTF8>::WriteChars(const u16char *str,int64 size)
		{
			return out?out->WriteUTF8Chars(str,size):false;
		}

		template<> bool TextOutputStream<bomUTF16LE>::WriteChars(const char *str,int64 size)
		{
			return out?out->WriteUTF16LEChars(str,size):false;
		}

		template<> bool TextOutputStream<bomUTF16LE>::WriteChars(const u16char *str,int64 size)
		{
			return out?out->WriteUTF16LEChars(str,size):false;
		}

		template<> bool TextOutputStream<bomUTF16BE>::WriteChars(const char *str,int64 size)
		{
			return out?out->WriteUTF16BEChars(str,size):false;
		}

		template<> bool TextOutputStream<bomUTF16BE>::WriteChars(const u16char *str,int64 size)
		{
			return out?out->WriteUTF16BEChars(str,size):false;
		}
	}//namespace io
}//namespace hgl
