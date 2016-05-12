#ifndef HGL_ENDIAN_INCLUDE
#define HGL_ENDIAN_INCLUDE

#include<hgl/platform/Platform.h>				// 平台定义
namespace hgl
{
	namespace endian
	{
		/**
		 * 字节序类型枚举
		 */
		enum ByteOrderMask
		{
			bomAnsi=0,
			bomUTF8,
			bomUTF16LE,
			bomUTF16BE,
			bomUTF32LE,
			bomUTF32BE,
			bomEnd
		};

		/**
		 * 字节序文件头数据结构
		 */
		struct BOMFileHeader
		{
			int size;					//字节序文件头长度
			unsigned char data[4];		//字节序数据
		};

		/**
		 * 字节序文件头定义
		 */
		const BOMFileHeader BOMData[bomEnd]=
		{
			{0,{}},
			{3,{0xEF,0xBB,0xBF}},
			{2,{0xFF,0xFE}},
			{2,{0xFE,0xFF}},
			{4,{0xFF,0xFE,0x00,0x00}},
			{4,{0x00,0x00,0xFE,0xFF}}
		};

        #define CharSetNameLength   32
        using CharSetName=char[CharSetNameLength];

		template<int,char> const CharSetName &GetCurCharSet();											///<取得当前程序编码字符集

        const CharSetName utf8_charset="utf8";
        const CharSetName utf16le_charset="utf-16le";
        const CharSetName utf16be_charset="utf-16be";
        const CharSetName utf32le_charset="utf-32le";
        const CharSetName utf32be_charset="utf-32be";

		template<> inline const CharSetName &GetCurCharSet<2,HGL_LITTLE_ENDIAN	>(){return utf16le_charset;}
		template<> inline const CharSetName &GetCurCharSet<2,HGL_BIG_ENDIAN	>(){return utf16be_charset;}
		template<> inline const CharSetName &GetCurCharSet<4,HGL_LITTLE_ENDIAN	>(){return utf32le_charset;}
		template<> inline const CharSetName &GetCurCharSet<4,HGL_BIG_ENDIAN	>(){return utf32be_charset;}

		template<typename T> inline const CharSetName &GetCharSet()
		{
			return GetCurCharSet<sizeof(T),HGL_ENDIAN>();
		}

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
				*dst=EndianSwap(D(*src));		//必须在ENDIAN SWAP前转换类型，否则在32转16位时，会将0000转出来。
				++dst;
				++src;
			}
		}

		#if HGL_ENDIAN == HGL_BIG_ENDIAN

			#define HGL_BOM_UTF16LE	0xfffe
			#define HGL_BOM_UTF16BE	0xfeff
			#define HGL_BOM_UTF32LE	0xfffe0000
			#define HGL_BOM_UTF32BE	0x0000feff

			#define LittleToCurrentEndian	EndianSwap
			#define BigToCurrentEndian		ToBigEndian

			template<typename T> T ToBigEndian(T value){return value;}

			template<typename T> inline void ToBigEndian(T *value,const int64 count){}
			template<typename D,typename S> inline void ToBigEndian(D *dst,const S *src,const int64 count){hgl_cpy(dst,src,count);}

			template<typename T> T ToLittleEndian(T value){return EndianSwap(value);}

			template<typename T> inline void ToLittleEndian(T *value,const int64 count){EndianSwap<T>(value,count);}
			template<typename D,typename S> inline void ToLittleEndian(D *dst,const S *src,const int64 count){EndianSwap<D,S>(dst,src,count);}

		#else

			#define HGL_BOM_UTF16LE	0xfeff
			#define HGL_BOM_UTF16BE	0xfffe
			#define HGL_BOM_UTF32LE	0x0000feff
			#define HGL_BOM_UTF32BE	0xfffe0000

			#define LittleToCurrentEndian	ToLittleEndian
			#define BigToCurrentEndian		EndianSwap

			template<typename T> T ToBigEndian(T value){return EndianSwap(value);}

			template<typename T> inline void ToBigEndian(T *value,const int64 count){EndianSwap<T>(value,count);}
			template<typename D,typename S> inline void ToBigEndian(D *dst,const S *src,const int64 count){EndianSwap<D,S>(dst,src,count);}

			template<typename T> T ToLittleEndian(T value){return value;}

			template<typename T> inline void ToLittleEndian(T *value,const int64 count){}
			template<typename D,typename S> inline void ToLittleEndian(D *dst,const S *src,const int64 count){hgl_cpy(dst,src,count);}
		#endif//HGL_BIG_ENDIAN

		template<typename T>
		T *swap_strcpy(const T *in_str,const int length)
		{
			if(length<=0||!in_str)return(nullptr);

			T *out_str=new T[length+1];

			EndianSwap<T>(out_str,in_str,length);

			out_str[length]=0;

			return out_str;
		}

		template<char> struct UTF16CharConvert;

		template<> struct UTF16CharConvert<HGL_LITTLE_ENDIAN>
		{
			static void convert(const u16char *str,const int length)
			{
				#if HGL_ENDIAN == HGL_BIG_ENDIAN
					swap_strcpy<u16char>(str,length);
				#endif//HGL_ENDIAN == HGL_LITTLE_ENDIAN
			}

			static void convert(u16char *out_str,const u16char *in_str,const int length)
			{
				#if HGL_ENDIAN == HGL_LITTLE_ENDIAN
					memcpy(out_str,in_str,length*sizeof(u16char));
				#else
					swap_strcpy<u16char>(in_str,length);
				#endif//HGL_ENDIAN == HGL_LITTLE_ENDIAN
			}
		};//template<> struct UTF16CharConvert<HGL_LITTLE_ENDIAN>

		template<> struct UTF16CharConvert<HGL_BIG_ENDIAN>
		{
			static void convert(const u16char *str,const int length)
			{
				#if HGL_ENDIAN == HGL_LITTLE_ENDIAN
					swap_strcpy<u16char>(str,length);
				#endif//HGL_ENDIAN == HGL_LITTLE_ENDIAN
			}

			static void convert(u16char *out_str,const u16char *in_str,const int length)
			{
				#if HGL_ENDIAN == HGL_LITTLE_ENDIAN
					memcpy(out_str,in_str,length*sizeof(u16char));
				#else
					swap_strcpy<u16char>(in_str,length);
				#endif//HGL_ENDIAN == HGL_LITTLE_ENDIAN
			}
		};//template<> struct UTF16ToWideChar<HGL_BIG_ENDIAN>
	}//namespace endian

	using namespace endian;
}//namespace hgl
#endif//HGL_ENDIAN_INCLUDE
