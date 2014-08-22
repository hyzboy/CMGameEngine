#ifndef HGL_IO_JAVA_OUTPUT_STREAM_INCLUDE
#define HGL_IO_JAVA_OUTPUT_STREAM_INCLUDE

#include<hgl/io/DataOutputStream.h>
namespace hgl
{
	namespace io
	{
		/**
		* Java数据输出流<br>
		* 接口类似于java.io.DataOutputStream，用于和Java程序进行交互
		*/
		class JavaOutputStream
		{
		protected:

			BEDataOutputStream *out;

		public:

			JavaOutputStream(OutputStream *os)
			{
				out=new BEDataOutputStream(os);
			}

			virtual ~JavaOutputStream()
			{
				delete out;
			}

			int64	write				(const void *ptr,int size)	{return out?out->WriteFully(ptr,size):-1;}

			bool	writeBoolean		(const bool		&b)			{return out?out->WriteBool	(b):false;}
			bool	writeByte			(const int8		&i)			{return out?out->WriteInt8	(i):false;}
			bool	writeUnsignedByte	(const uint8	&i)			{return out?out->WriteUint8	(i):false;}

			bool	writeShort			(const int16	&i)			{return out?out->WriteInt16	(i):false;}
			bool	writeUnsignedShort	(const uint16	&i)			{return out?out->WriteUint16(i):false;}
			bool	writeInt			(const int32	&i)			{return out?out->WriteInt32	(i):false;}
			bool	writeLong			(const int64	&i)			{return out?out->WriteInt64	(i):false;}
			bool	writeFloat			(const float	&f)			{return out?out->WriteFloat	(f):false;}
			bool	writeDouble			(const double	&d)			{return out?out->WriteDouble(d):false;}

			bool	writeChar			(const u16char	&c)
			{
				if(!out)
					return(false);

				return out->Write(ToBigEndian(c));
			}

			bool	writeChars			(const u16char *wstr,const int64 count)
			{
				return out?out->WriteUTF16BEChars(wstr,count):false;
			}

			bool	writeUTF			(const UTF8String	&);
			bool	writeUTF			(const UTF16String	&);
		};//class JavaOutputStream
	}//namespace io
}//namespace hgl
#endif//HGL_IO_JAVA_OUTPUT_STREAM_INCLUDE
