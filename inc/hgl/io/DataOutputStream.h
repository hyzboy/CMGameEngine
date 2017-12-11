#ifndef HGL_IO_DATA_OUTPUT_STREAM_INCLUDE
#define HGL_IO_DATA_OUTPUT_STREAM_INCLUDE

#include<hgl/io/OutputStream.h>
#include<hgl/type/BaseString.h>
#include<hgl/CodePage.h>
#include<hgl/algorithm/VectorMath.h>
namespace hgl
{
    namespace io
    {
        class OutputStream;

        /**
        * 格式数据输出流基类
        */
        class DataOutputStream                                                                      ///格式数据输出流基类
        {
        protected:

            OutputStream *out;

        public:

            DataOutputStream(OutputStream *os)
            {
                out=os;
            }

            virtual ~DataOutputStream()
            {
            }

            virtual void Use(OutputStream *os)
            {
                out=os;
            }

            virtual int64 Write(const void *buf,int64 size)
            {
                if(!buf||size<0)return(-1);

                return(out?out->Write(buf,size):-1);
            }

            virtual int64 WriteFully(const void *buf,int64 size)
            {
                if(!buf||size<0)return(-1);

                return(out?out->WriteFully(buf,size):-1);
            }

            virtual int64 Seek(int64 offset,SeekOrigin so=soBegin)
            {
                return(out?out->Seek(offset,so):-1);
            }

            virtual bool    CanRestart  ()const{return out?out->CanRestart  ():false;}              ///<是否可以复位
            virtual bool    CanSeek     ()const{return out?out->CanSeek     ():false;}              ///<是否可以定位
            virtual bool    CanSize     ()const{return out?out->CanSize     ():false;}              ///<是否可以取得尺寸

            virtual bool    Restart     ()const{return out?out->Restart     ():false;}              ///<复位访问指针
            virtual int64   Tell        ()const{return out?out->Tell        ():-1;}                 ///<返回当前访问位置
            virtual int64   GetSize     ()const{return out?out->GetSize     ():-1;}                 ///<取得流长度
            virtual int64   Available   ()const{return out?out->Available   ():-1;}                 ///<剩下的可以不受阻塞写入的字节数
            
            template<typename T> bool WriteNumber(const T &value);

            #define STREAM_WRITE(type,name) virtual bool Write##name(const type &value) \
                                            {   \
                                                return Write(value);    \
                                            }   \
                                            \
                                            virtual int64 Write##name(const type *data,const int64 count)   \
                                            {   \
                                                if(count<=0)return(count);  \
                                                if(!data)return(-1);    \
                                                \
                                                return WriteArrays(data,count); \
                                            }

            STREAM_WRITE(char,      Char    );
            STREAM_WRITE(bool,      Bool    );
            STREAM_WRITE(int8,      Int8    );
            STREAM_WRITE(uint8,     Uint8   );

            #undef STREAM_WRITE

            template<> bool WriteNumber<int8>(const int8 &value){return WriteInt8(value);}
            template<> bool WriteNumber<uint8>(const uint8 &value){return WriteUint8(value);}

            #define STREAM_WRITE(type,name) virtual bool Write##name(const type &)=0;   \
                                            virtual int64 Write##name(const type *,const int64)=0;  \
                                            template<> bool WriteNumber<type>(const type &value){return Write##name(value);}

            STREAM_WRITE(int16,     Int16   );
            STREAM_WRITE(int32,     Int32   );
            STREAM_WRITE(int64,     Int64   );

            STREAM_WRITE(uint16,    Uint16  );
            STREAM_WRITE(uint32,    Uint32  );
            STREAM_WRITE(uint64,    Uint64  );

            STREAM_WRITE(u16char,   UTF16Char);
            STREAM_WRITE(float,     Float   );
            STREAM_WRITE(double,    Double  );

            #undef STREAM_WRITE

            bool Write(const Vector2f &vec){return WriteFloat((float *)&vec,2)==2;}
            bool Write(const Vector3f &vec){return WriteFloat((float *)&vec,3)==3;}
            bool Write(const Vector4f &vec){return WriteFloat((float *)&vec,4)==4;}

            /**
            * 自适应类型数据写入</p>
            * 请在多平台混用的情况下不要使用此函数，以免造成不同平台数据实质结构不一致的情况
            */
            template<typename T> bool Write(const T &data)
            {
                return WriteFully(&data,sizeof(T))==sizeof(T);
            }

            /**
            * 自适应类型数据阵列写入</p>
            * 请在多平台混用的情况下不要使用此函数，以免造成不同平台数据实质结构不一致的情况
            * @param data 数据
            * @param count 数据个数
            * @return 实质写入的数据个数
            * @return -1 出错
            */
            template<typename T> int64 WriteArrays(const T *data,int64 count)
            {
                if(count<=0)return(count);
                if(!data)return(-1);

                return WriteFully(data,count*sizeof(T))/sizeof(T);
            }

            template<typename T> bool WriteUTF8Chars        (const T *str,uint count);                          ///<按utf8格式写入字符阵列
            template<typename T> bool WriteUTF16LEChars     (const T *str,uint count);                          ///<按utf16-le格式写入字符阵列
            template<typename T> bool WriteUTF16BEChars     (const T *str,uint count);                          ///<按utf16-be格式写入字符阵列

            template<typename T> bool WriteUTF8Chars        (const BaseString<T> &str){return WriteUTF8Chars    (str.c_str(),str.Length());}
            template<typename T> bool WriteUTF16LEChars     (const BaseString<T> &str){return WriteUTF16LEChars (str.c_str(),str.Length());}
            template<typename T> bool WriteUTF16BEChars     (const BaseString<T> &str){return WriteUTF16BEChars (str.c_str(),str.Length());}

            template<typename T> bool WriteUTF8StringWithLength (const char *str,const uint length);
            template<typename T> bool WriteUTF8StringWithLength (const UTF16String &str);            
            
            template<uchar C,typename T> bool WriteUTF16StringWithLength(const u16char *str,const uint len);

        public:

            //32 bit length
            bool WriteUTF8String        (const char *str,uint length);                                              ///<按utf16-le格式写入字符串(前置4字节字符串长度,再写入字符阵列)
            bool WriteUTF8String        (const char *str            );                                              ///<按utf8格式写入字符串(前置4字节字符串长度,再写入字符阵列)
            bool WriteUTF8String        (const UTF8String &str      );                                              ///<按utf8格式写入字符串(前置4字节字符串长度,再写入字符阵列)
            bool WriteUTF8String        (const UTF16String &str     );                                              ///<按utf8格式写入字符串(前置4字节字符串长度,再写入字符阵列)

            bool WriteUTF16LEString     (const u16char *str,uint len);                                              ///<按utf16-le格式写入字符串(前置4字节字符串长度,再写入字符阵列)
            bool WriteUTF16BEString     (const u16char *str,uint len);                                              ///<按utf16-be格式写入字符串(前置4字节字符串长度,再写入字符阵列)

            bool WriteUTF16LEString     (const UTF16String &str     );                                              ///<按utf16-le格式写入字符串(前置4字节字符串长度,再写入字符阵列)
            bool WriteUTF16BEString     (const UTF16String &str     );                                              ///<按utf16-be格式写入字符串(前置4字节字符串长度,再写入字符阵列)

            bool WriteUTF16LEString     (const u16char *str         );                                              ///<按utf16-le格式写入字符串(前置4字节字符串长度,再写入字符阵列)
            bool WriteUTF16BEString     (const u16char *str         );                                              ///<按utf16-be格式写入字符串(前置4字节字符串长度,再写入字符阵列)

            bool WriteUTF16LEString     (const UTF8String &str      ){return WriteUTF16LEString(to_u16(str));}      ///<按utf16-le格式写入字符串(前置4字节字符串长度,再写入字符阵列)
            bool WriteUTF16BEString     (const UTF8String &str      ){return WriteUTF16BEString(to_u16(str));}      ///<按utf16-be格式写入字符串(前置4字节字符串长度,再写入字符阵列)

            //16 bit length
            bool WriteUTF8ShortString   (const char *str,uint length);                                              ///<按utf16-le格式写入字符串(前置2字节字符串长度,再写入字符阵列)
            bool WriteUTF8ShortString   (const char *str            );                                              ///<按utf8格式写入字符串(前置2字节字符串长度,再写入字符阵列)
            bool WriteUTF8ShortString   (const UTF8String &str      );                                              ///<按utf8格式写入字符串(前置2字节字符串长度,再写入字符阵列)
            bool WriteUTF8ShortString   (const UTF16String &str     );                                              ///<按utf8格式写入字符串(前置2字节字符串长度,再写入字符阵列)

            bool WriteUTF16LEShortString(const u16char *str,uint len);                                              ///<按utf16-le格式写入字符串(前置2字节字符串长度,再写入字符阵列)
            bool WriteUTF16BEShortString(const u16char *str,uint len);                                              ///<按utf16-be格式写入字符串(前置2字节字符串长度,再写入字符阵列)

            bool WriteUTF16LEShortString(const UTF16String &str     );                                              ///<按utf16-le格式写入字符串(前置2字节字符串长度,再写入字符阵列)
            bool WriteUTF16BEShortString(const UTF16String &str     );                                              ///<按utf16-be格式写入字符串(前置2字节字符串长度,再写入字符阵列)

            bool WriteUTF16LEShortString(const u16char *str         );                                              ///<按utf16-le格式写入字符串(前置2字节字符串长度,再写入字符阵列)
            bool WriteUTF16BEShortString(const u16char *str         );                                              ///<按utf16-be格式写入字符串(前置2字节字符串长度,再写入字符阵列)

            bool WriteUTF16LEShortString(const UTF8String &str      ){return WriteUTF16LEShortString(to_u16(str));} ///<按utf16-le格式写入字符串(前置2字节字符串长度,再写入字符阵列)
            bool WriteUTF16BEShortString(const UTF8String &str      ){return WriteUTF16BEShortString(to_u16(str));} ///<按utf16-be格式写入字符串(前置2字节字符串长度,再写入字符阵列)

            //8 bit length
            bool WriteUTF8TinyString    (const char *str,uint length);                                              ///<按utf16-le格式写入字符串(前置1字节字符串长度,再写入字符阵列)
            bool WriteUTF8TinyString    (const char *str            );                                              ///<按utf8格式写入字符串(前置1字节字符串长度,再写入字符阵列)
            bool WriteUTF8TinyString    (const UTF8String &str      );                                              ///<按utf8格式写入字符串(前置1字节字符串长度,再写入字符阵列)
            bool WriteUTF8TinyString    (const UTF16String &str     );                                              ///<按utf8格式写入字符串(前置1字节字符串长度,再写入字符阵列)

            bool WriteUTF16LETinyString (const u16char *str,uint len);                                              ///<按utf16-le格式写入字符串(前置1字节字符串长度,再写入字符阵列)
            bool WriteUTF16BETinyString (const u16char *str,uint len);                                              ///<按utf16-be格式写入字符串(前置1字节字符串长度,再写入字符阵列)

            bool WriteUTF16LETinyString (const UTF16String &str     );                                              ///<按utf16-le格式写入字符串(前置1字节字符串长度,再写入字符阵列)
            bool WriteUTF16BETinyString (const UTF16String &str     );                                              ///<按utf16-be格式写入字符串(前置1字节字符串长度,再写入字符阵列)

            bool WriteUTF16LETinyString (const u16char *str         );                                              ///<按utf16-le格式写入字符串(前置1字节字符串长度,再写入字符阵列)
            bool WriteUTF16BETinyString (const u16char *str         );                                              ///<按utf16-be格式写入字符串(前置1字节字符串长度,再写入字符阵列)

            bool WriteUTF16LETinyString (const UTF8String &str      ){return WriteUTF16LETinyString(to_u16(str));}  ///<按utf16-le格式写入字符串(前置1字节字符串长度,再写入字符阵列)
            bool WriteUTF16BETinyString (const UTF8String &str      ){return WriteUTF16BETinyString(to_u16(str));}  ///<按utf16-be格式写入字符串(前置1字节字符串长度,再写入字符阵列)
        };//class DataOutputStream
    }//namespace io
}//namespace hgl
#include<hgl/io/EndianDataOutputStream.h>
#endif//HGL_IO_DATA_OUTPUT_STREAM_INCLUDEs
