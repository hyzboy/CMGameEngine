#ifndef HGL_ENDIAN_INCLUDE
#define HGL_ENDIAN_INCLUDE

#include<hgl/platform/Platform.h>               // 平台定义
namespace hgl
{
    namespace endian
    {
        /**
        * Windows代码页枚举
        * 全部Windows所支持代码页请参见 http://msdn.microsoft.com/en-us/library/dd317756
        */
        enum CharCodePage                                       ///代码页枚举
        {
            ccpNone=0,                                          ///<起始定义，无意义

            //中文
            ccpGBK                          =936,               ///<中国GBK标准中文
            ccpBig5                         =950,               ///<中国台湾Big5标准繁体中文
            ccpGB2312                       =20936,             ///<中国GB2312标准简体中文
            ccpGB18030                      =54936,             ///<中国GB18030-2000标准中文

            //日文
            ccpShiftJIS                     =932,               ///<日文ShiftJIS
            ccpJISX                         =50222,             ///<日文JIS X/ISO 2022

            //韩文
            ccpKorean                       =949,               ///<韩文

            //苹果编码
            ccpMacJanpan                    =10001,             ///<日文
            ccpMacTraditionalChinese        =10002,             ///<繁体中文
            ccpMacSimplifiedChinese         =10008,             ///<简体中文

            //unicode
            ccpUTF7                         =65000,             ///<utf-7
            ccpUTF8                         =65001,             ///<utf-8

            ccpUTF16LE                      =1200,
            ccpUTF16BE                      =1201,
            ccpUTF32LE                      =12000,
            ccpUTF32BE                      =12001,

            ccpEnd                                              ///<结束定义，无意义
        };//enum CharCodePage

        /**
         * 字节序类型枚举
         */
        enum ByteOrderMask
        {
            bomNone=0,
            bomUTF8,
            bomUTF16LE,
            bomUTF16BE,
            bomUTF32LE,
            bomUTF32BE,
            bomEnd
        };

        constexpr uint CharSetNameLength=32;                                                            ///<字符集名称长度
        using CharSetName=char[CharSetNameLength];                                                      ///<字符集名称类型定义

        template<int,char> const CharSetName &GetCurCharSet();                                          ///<取得当前程序编码字符集

        constexpr CharSetName utf8_charset="utf8";
        constexpr CharSetName utf16le_charset="utf-16le";
        constexpr CharSetName utf16be_charset="utf-16be";
        constexpr CharSetName utf32le_charset="utf-32le";
        constexpr CharSetName utf32be_charset="utf-32be";

        template<> inline const CharSetName &GetCurCharSet<2,HGL_LITTLE_ENDIAN    >(){return utf16le_charset;}
        template<> inline const CharSetName &GetCurCharSet<2,HGL_BIG_ENDIAN       >(){return utf16be_charset;}
        template<> inline const CharSetName &GetCurCharSet<4,HGL_LITTLE_ENDIAN    >(){return utf32le_charset;}
        template<> inline const CharSetName &GetCurCharSet<4,HGL_BIG_ENDIAN       >(){return utf32be_charset;}

        template<typename T> inline const CharSetName &GetCharSet()
        {
            return GetCurCharSet<sizeof(T),HGL_ENDIAN>();
        }

        /**
         * 字节序文件头数据结构
         */
        struct BOMFileHeader
        {
            int size;                   ///<字节序文件头长度
            unsigned char data[4];      ///<字节序数据

            ByteOrderMask bom;          ///<字节序枚举
            const CharSetName *char_set;///<字符集名称
            CharCodePage code_page;     ///<代码页
        };

        /**
         * 字节序文件头定义
         */
        constexpr BOMFileHeader BOMData[bomEnd]=
        {
            {0,{}                   ,bomNone,   nullptr            ,ccpNone     },
            {3,{0xEF,0xBB,0xBF}     ,bomUTF8,   &utf8_charset      ,ccpUTF8     },
            {2,{0xFF,0xFE}          ,bomUTF16LE,&utf16le_charset   ,ccpUTF16LE  },
            {2,{0xFE,0xFF}          ,bomUTF16BE,&utf16be_charset   ,ccpUTF16BE  },
            {4,{0xFF,0xFE,0x00,0x00},bomUTF32LE,&utf32le_charset   ,ccpUTF32LE  },
            {4,{0x00,0x00,0xFE,0xFF},bomUTF32BE,&utf32be_charset   ,ccpUTF32BE  }
        };

        template<typename T>
        inline T EndianSwap(const T value)
        {
            union
            {
                T v;
                uint8 bytes[sizeof(T)];
            }a,b;

            a.v=value;

            for(uint i=0;i<sizeof(T);i++)
                b.bytes[i]=a.bytes[sizeof(T)-1-i];

            return b.v;
        }

        template<> inline  int8 EndianSwap< int8>(const  int8 value){return value;}
        template<> inline uint8 EndianSwap<uint8>(const uint8 value){return value;}

        template<> inline uint16 EndianSwap(const uint16 value)
        {
            return ((value&0xFF)<<8)
                |((value&0xFF00)>>8);
        }

        template<> inline uint32 EndianSwap(const uint32 value)
        {
            return ((value&0xFF)<<24)
                |((value&0xFF00)<<8)
                |((value&0xFF0000)>>8)
                |((value&0xFF000000)>>24);
        }

        template<> inline uint64 EndianSwap(const uint64 value)
        {
            return ((value&0xFF)<<56)
                |((value&0xFF00)<<40)
                |((value&0xFF0000)<<24)
                |((value&0xFF000000)<<8)
                |((value&0xFF00000000)>>8)
                |((value&0xFF0000000000)>>24)
                |((value&0xFF000000000000)>>40)
                |((value&0xFF00000000000000)>>56);
        }

        template<> inline u16char EndianSwap(const u16char value){return EndianSwap(uint16(value));}

        template<typename T>
        inline void EndianSwap(T *value,const int64 count)
        {
            for(int64 i=0;i<count;i++)
            {
                *value=EndianSwap(*value);
                ++value;
            }
        }

        template<typename T>
        inline void EndianSwap(T *dst,const T *src,const int64 count)
        {
            for(uint i=0;i<count;i++)
            {
                *dst=EndianSwap(*src);
                ++dst;
                ++src;
            }
        }

        template<typename D,typename S>
        inline void EndianSwap(D *dst,const S *src,const int64 count)
        {
            for(uint i=0;i<count;i++)
            {
                *dst=EndianSwap(D(*src));       //必须在ENDIAN SWAP前转换类型，否则在32转16位时，会将0000转出来。
                ++dst;
                ++src;
            }
        }

        #if HGL_ENDIAN == HGL_BIG_ENDIAN

            constexpr uint HGL_BOM_UTF16LE  =0xfffe;
            constexpr uint HGL_BOM_UTF16BE  =0xfeff;
            constexpr uint HGL_BOM_UTF32LE  =0xfffe0000;
            constexpr uint HGL_BOM_UTF32BE  =0x0000feff;

            #define LittleToCurrentEndian   EndianSwap
            #define BigToCurrentEndian      ToBigEndian

            template<typename T> T ToBigEndian(T value){return value;}

            template<typename T> inline void ToBigEndian(T *value,const int64 count){}
            template<typename D,typename S> inline void ToBigEndian(D *dst,const S *src,const int64 count){hgl_cpy(dst,src,count);}

            template<typename T> T ToLittleEndian(T value){return EndianSwap(value);}

            template<typename T> inline void ToLittleEndian(T *value,const int64 count){EndianSwap<T>(value,count);}
            template<typename D,typename S> inline void ToLittleEndian(D *dst,const S *src,const int64 count){EndianSwap<D,S>(dst,src,count);}

        #else

            constexpr uint HGL_BOM_UTF16LE  =0xfeff;
            constexpr uint HGL_BOM_UTF16BE  =0xfffe;
            constexpr uint HGL_BOM_UTF32LE  =0x0000feff;
            constexpr uint HGL_BOM_UTF32BE  =0xfffe0000;

            #define LittleToCurrentEndian   ToLittleEndian
            #define BigToCurrentEndian      EndianSwap

            template<typename T> T ToBigEndian(T value){return EndianSwap(value);}

            template<typename T> inline void ToBigEndian(T *value,const int64 count){EndianSwap<T>(value,count);}
            template<typename D,typename S> inline void ToBigEndian(D *dst,const S *src,const int64 count){EndianSwap<D,S>(dst,src,count);}

            template<typename T> T ToLittleEndian(T value){return value;}

            template<typename T> inline void ToLittleEndian(T *,const int64){}
            template<typename D,typename S> inline void ToLittleEndian(D *dst,const S *src,const int64 count){hgl_cpy(dst,src,count);}

        #endif//HGL_BIG_ENDIAN

        template<char> struct UTF16CharConvert;

        template<> struct UTF16CharConvert<HGL_LITTLE_ENDIAN>
        {
        #if HGL_ENDIAN == HGL_BIG_ENDIAN
            static void convert(u16char *str,const int length)
            {
                    EndianSwap<u16char>(str,length);
            }
        #else
            static void convert(const u16char *,const int){}
        #endif//HGL_ENDIAN == HGL_LITTLE_ENDIAN

            static void convert(u16char *out_str,const u16char *in_str,const int length)
            {
                #if HGL_ENDIAN == HGL_LITTLE_ENDIAN
                    memcpy(out_str,in_str,length*sizeof(u16char));
                #else
                    EndianSwap<u16char>(in_str,length);
                #endif//HGL_ENDIAN == HGL_LITTLE_ENDIAN
            }
        };//template<> struct UTF16CharConvert<HGL_LITTLE_ENDIAN>

        template<> struct UTF16CharConvert<HGL_BIG_ENDIAN>
        {
            static void convert(u16char *str,const int length)
            {
                #if HGL_ENDIAN == HGL_LITTLE_ENDIAN
                    EndianSwap<u16char>(str,length);
                #endif//HGL_ENDIAN == HGL_LITTLE_ENDIAN
            }

            static void convert(u16char *out_str,const u16char *in_str,const int length)
            {
                #if HGL_ENDIAN == HGL_LITTLE_ENDIAN
                    memcpy(out_str,in_str,length*sizeof(u16char));
                #else
                    EndianSwap<u16char>(in_str,length);
                #endif//HGL_ENDIAN == HGL_LITTLE_ENDIAN
            }
        };//template<> struct UTF16ToWideChar<HGL_BIG_ENDIAN>
    }//namespace endian

    using namespace endian;
}//namespace hgl
#endif//HGL_ENDIAN_INCLUDE
