#ifndef HGL_IO_JAVA_INPUT_STREAM_INCLUDE
#define HGL_IO_JAVA_INPUT_STREAM_INCLUDE

#include<hgl/io/DataInputStream.h>
namespace hgl
{
    namespace io
    {
        /**
        * Java数据输入流<br>
        * 接口类似于java.io.DataInputStream，用于和Java程序进行交互
        */
        class JavaInputStream
        {
        protected:

            BEDataInputStream *in;

        public:

            JavaInputStream(InputStream *is)
            {
                in=new BEDataInputStream(is);
            }

            virtual ~JavaInputStream()
            {
                delete in;
            }

            int64   read                (void *ptr,int size){return in?in->ReadFully(ptr,size):-1;}
            int     skipBytes           (int size)          {return in?in->Seek(size,soCurrent):-1;}

            bool    readBoolean         (bool       &b)     {return in?in->ReadBool     (b):false;}
            bool    readByte            (int8       &i)     {return in?in->ReadInt8     (i):false;}
            bool    readUnsignedByte    (uint8      &i)     {return in?in->ReadUint8    (i):false;}

            bool    readShort           (int16      &i)     {return in?in->ReadInt16    (i):false;}
            bool    readUnsignedShort   (uint16     &i)     {return in?in->ReadUint16   (i):false;}
            bool    readInt             (int32      &i)     {return in?in->ReadInt32    (i):false;}
            bool    readLong            (int64      &i)     {return in?in->ReadInt64    (i):false;}
            bool    readFloat           (float      &f)     {return in?in->ReadFloat    (f):false;}
            bool    readDouble          (double     &d)     {return in?in->ReadDouble   (d):false;}

            bool    readChar            (u16char    &c)
            {
                if(!in)
                    return(false);

                uint16 c16;

                if(!in->Read(c16))
                    return(false);

                c=BigToCurrentEndian(c16);
                return(true);
            }

            bool    readChars           (u16char *wstr,const int count)
            {
                return in?in->ReadUTF16BEChars(wstr,count):false;
            }

            bool    readUTF             (UTF16String &str)
            {
                return in?in->ReadUTF8ShortString(str);
            }
        };//class JavaInputStream
    }//namespace io
}//namespace hgl
#endif//HGL_IO_JAVA_INPUT_STREAM_INCLUDE
