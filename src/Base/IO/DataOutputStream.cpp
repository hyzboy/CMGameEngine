#include<hgl/io/DataOutputStream.h>

namespace hgl
{
    namespace io    // write utf8 chars
    {
        template<> bool DataOutputStream::WriteUTF8Chars<char>        (const char *str,uint size)
        {
            if(!out)return(false);
            if(size==0)return(true);

            if(!str||!*str||size<0)return(false);

            return(WriteArrays(str,size)==size);
        }

        template<> bool DataOutputStream::WriteUTF8Chars<u16char>    (const u16char *str,uint size)
        {
            if(!out)return(false);
            if(size==0)return(true);

            if(!str||!*str||size<0)return(false);

            return WriteUTF8Chars(to_u8(str,size));
        }
    }

    namespace io    //write utf8 string
    {
        template<typename T> bool DataOutputStream::WriteUTF8StringWithLength(const char *str,const uint length)
        {
            if(!out)return(false);

            if(length&&!str)return(false);

            if(!WriteNumber<T>(length))
                return(false);

            if(!length)
                return(true);

            return WriteUTF8Chars(str,length);
        }

        template<typename T> bool DataOutputStream::WriteUTF8StringWithLength(const UTF16String &str)
        {
            if(!out)return(false);

            int utf8_count;
            char *utf8_str=u16_to_u8(str.c_str(),str.Length(),utf8_count);

            const bool result=WriteUTF8StringWithLength<T>(utf8_str,utf8_count);

            delete[] utf8_str;

            return result;
        }
    }

    namespace io    //write utf16 chars
    {
        template<char T>        //非特例化版本，用于需要swap的
        bool WriteUTF16Chars(DataOutputStream *dos,const u16char *str,uint count)
        {
            SharedArray<u16char> swap_str=new u16char[count];

            EndianSwap<u16char>(swap_str,str,count);

            return(dos->WriteArrays<u16char>(swap_str,count)==count);
        }

        template<>                //特例化版本，用于直接写入的
        bool WriteUTF16Chars<HGL_ENDIAN>(DataOutputStream *dos,const u16char *str,uint count)
        {
            return(dos->WriteArrays(str,count)==count);
        }
    }

    namespace io    //write utf16-le chars
    {
        template<> bool DataOutputStream::WriteUTF16LEChars<char>(const char *str,uint length)
        {
            if(length==0)return(true);
            if(!out||!str||!*str||length<0)return(false);

            UTF16String u16_str=to_u16(str);

            return WriteUTF16Chars<HGL_LITTLE_ENDIAN>(this,u16_str.c_str(),u16_str.Length());
        }

        template<> bool DataOutputStream::WriteUTF16LEChars<u16char>(const u16char *str,uint length)
        {
            if(length==0)return(true);
            if(!out||!str||!*str||length<0)return(false);

            return WriteUTF16Chars<HGL_LITTLE_ENDIAN>(this,str,length);
        }
    }

    namespace io    //write utf16-be chars
    {
        template<> bool DataOutputStream::WriteUTF16BEChars<char>(const char *str,uint length)
        {
            if(length==0)return(true);
            if(!out||!str||!*str||length<0)return(false);

            UTF16String u16_str=to_u16(str,length);

            return WriteUTF16Chars<HGL_BIG_ENDIAN>(this,u16_str.c_str(),u16_str.Length());
        }

        template<> bool DataOutputStream::WriteUTF16BEChars<u16char>(const u16char *str,uint length)
        {
            if(length==0)return(true);
            if(!out||!str||!*str||length<0)return(false);

            return WriteUTF16Chars<HGL_BIG_ENDIAN>(this,str,length);
        }
    }//namespace io

    namespace io    //write utf16 string
    {
        template<uchar C,typename T> bool DataOutputStream::WriteUTF16StringWithLength(const u16char *str,const uint len)
        {
            if(!out)return(false);            
            if(len&&!str)return(false);

            if(!WriteNumber<T>(len))
                return(false);

            if(!len)return(true);

            return WriteUTF16Chars<C>(this,str,len);
        }
    }//namespace io

    namespace io
    {
        bool DataOutputStream::WriteUTF8String        (const char *str,uint length){return WriteUTF8StringWithLength<uint32>(str,         length);}                               ///<按utf16-le格式写入字符串(前置4字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF8String        (const char *str            ){return WriteUTF8StringWithLength<uint32>(str,         hgl::strlen(str));}                     ///<按utf8格式写入字符串(前置4字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF8String        (const UTF8String &str      ){return WriteUTF8StringWithLength<uint32>(str.c_str(), str.Length());}                         ///<按utf8格式写入字符串(前置4字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF8String        (const UTF16String &str     ){return WriteUTF8StringWithLength<uint32>(str);}                                               ///<按utf8格式写入字符串(前置4字节字符串长度,再写入字符阵列)

        bool DataOutputStream::WriteUTF16LEString     (const u16char *str,uint len){return WriteUTF16StringWithLength<HGL_LITTLE_ENDIAN,  uint32>(str,        len);}              ///<按utf16-le格式写入字符串(前置4字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF16BEString     (const u16char *str,uint len){return WriteUTF16StringWithLength<HGL_BIG_ENDIAN,     uint32>(str,        len);}              ///<按utf16-be格式写入字符串(前置4字节字符串长度,再写入字符阵列)

        bool DataOutputStream::WriteUTF16LEString     (const UTF16String &str     ){return WriteUTF16StringWithLength<HGL_LITTLE_ENDIAN,  uint32>(str.c_str(),str.Length());}     ///<按utf16-le格式写入字符串(前置4字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF16BEString     (const UTF16String &str     ){return WriteUTF16StringWithLength<HGL_BIG_ENDIAN,     uint32>(str.c_str(),str.Length());}     ///<按utf16-be格式写入字符串(前置4字节字符串长度,再写入字符阵列)

        bool DataOutputStream::WriteUTF16LEString     (const u16char *str         ){return WriteUTF16StringWithLength<HGL_LITTLE_ENDIAN,  uint32>(str,        hgl::strlen(str));} ///<按utf16-le格式写入字符串(前置4字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF16BEString     (const u16char *str         ){return WriteUTF16StringWithLength<HGL_BIG_ENDIAN,     uint32>(str,        hgl::strlen(str));} ///<按utf16-be格式写入字符串(前置4字节字符串长度,再写入字符阵列)


        
        bool DataOutputStream::WriteUTF8ShortString   (const char *str,uint length){return WriteUTF8StringWithLength<uint16>(str,         length);}                               ///<按utf16-le格式写入字符串(前置2字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF8ShortString   (const char *str            ){return WriteUTF8StringWithLength<uint16>(str,         hgl::strlen(str));}                     ///<按utf8格式写入字符串(前置2字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF8ShortString   (const UTF8String &str      ){return WriteUTF8StringWithLength<uint16>(str.c_str(), str.Length());}                         ///<按utf8格式写入字符串(前置2字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF8ShortString   (const UTF16String &str     ){return WriteUTF8StringWithLength<uint16>(str);}                                               ///<按utf8格式写入字符串(前置2字节字符串长度,再写入字符阵列)

        bool DataOutputStream::WriteUTF16LEShortString(const u16char *str,uint len){return WriteUTF16StringWithLength<HGL_LITTLE_ENDIAN,  uint16>(str,        len);}              ///<按utf16-le格式写入字符串(前置2字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF16BEShortString(const u16char *str,uint len){return WriteUTF16StringWithLength<HGL_BIG_ENDIAN,     uint16>(str,        len);}              ///<按utf16-be格式写入字符串(前置2字节字符串长度,再写入字符阵列)

        bool DataOutputStream::WriteUTF16LEShortString(const UTF16String &str     ){return WriteUTF16StringWithLength<HGL_LITTLE_ENDIAN,  uint16>(str.c_str(),str.Length());}     ///<按utf16-le格式写入字符串(前置2字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF16BEShortString(const UTF16String &str     ){return WriteUTF16StringWithLength<HGL_BIG_ENDIAN,     uint16>(str.c_str(),str.Length());}     ///<按utf16-be格式写入字符串(前置2字节字符串长度,再写入字符阵列)

        bool DataOutputStream::WriteUTF16LEShortString(const u16char *str         ){return WriteUTF16StringWithLength<HGL_LITTLE_ENDIAN,  uint16>(str,        hgl::strlen(str));} ///<按utf16-le格式写入字符串(前置2字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF16BEShortString(const u16char *str         ){return WriteUTF16StringWithLength<HGL_BIG_ENDIAN,     uint16>(str,        hgl::strlen(str));} ///<按utf16-be格式写入字符串(前置2字节字符串长度,再写入字符阵列)



        bool DataOutputStream::WriteUTF8TinyString    (const char *str,uint length){return WriteUTF8StringWithLength<uint8>(str,          length);}                               ///<按utf16-le格式写入字符串(前置1字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF8TinyString    (const char *str            ){return WriteUTF8StringWithLength<uint8>(str,          hgl::strlen(str));}                     ///<按utf8格式写入字符串(前置1字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF8TinyString    (const UTF8String &str      ){return WriteUTF8StringWithLength<uint8>(str.c_str(),  str.Length());}                         ///<按utf8格式写入字符串(前置1字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF8TinyString    (const UTF16String &str     ){return WriteUTF8StringWithLength<uint8>(str);}                                                ///<按utf8格式写入字符串(前置1字节字符串长度,再写入字符阵列)

        bool DataOutputStream::WriteUTF16LETinyString (const u16char *str,uint len){return WriteUTF16StringWithLength<HGL_LITTLE_ENDIAN,  uint8>(str,         len);}              ///<按utf16-le格式写入字符串(前置1字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF16BETinyString (const u16char *str,uint len){return WriteUTF16StringWithLength<HGL_BIG_ENDIAN,     uint8>(str,         len);}              ///<按utf16-be格式写入字符串(前置1字节字符串长度,再写入字符阵列)

        bool DataOutputStream::WriteUTF16LETinyString (const UTF16String &str     ){return WriteUTF16StringWithLength<HGL_LITTLE_ENDIAN,  uint8>(str.c_str(), str.Length());}     ///<按utf16-le格式写入字符串(前置1字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF16BETinyString (const UTF16String &str     ){return WriteUTF16StringWithLength<HGL_BIG_ENDIAN,     uint8>(str.c_str(), str.Length());}     ///<按utf16-be格式写入字符串(前置1字节字符串长度,再写入字符阵列)

        bool DataOutputStream::WriteUTF16LETinyString (const u16char *str         ){return WriteUTF16StringWithLength<HGL_LITTLE_ENDIAN,  uint8>(str,         hgl::strlen(str));} ///<按utf16-le格式写入字符串(前置1字节字符串长度,再写入字符阵列)
        bool DataOutputStream::WriteUTF16BETinyString (const u16char *str         ){return WriteUTF16StringWithLength<HGL_BIG_ENDIAN,     uint8>(str,         hgl::strlen(str));} ///<按utf16-be格式写入字符串(前置1字节字符串长度,再写入字符阵列)
    }//namespace io
}//namespace hgl
