#include<hgl/CodePage.h>
#include<hgl/Other.h>

#if HGL_OS == HGL_OS_Windows
#include<windows.h>

namespace hgl
{
	int to_utf16(const CharSet &cs,u16char **dst,const char *src,const int src_size)
	{
		const int src_str_size=(src_size==-1)?strlen(src):src_size;

		const int len=MultiByteToWideChar(cs.codepage,0,src,src_str_size,0,0);

		if(len<=0)return(len);

		*dst=new u16char[len];

		return MultiByteToWideChar(cs.codepage,0,src,src_str_size,*dst,len);
	}

	int to_utf8(const CharSet &cs,u8char **dst,const char *src,const int src_size)
    {
        u16char *u16str;
        int u16size=to_utf16(cs,&u16str,src,src_size);

        if(u16size<=0)return(u16size);

        int u8_size;
        dst=u16_to_u8(u16str,u16size,u8_size)

        delete[] u16str;
        return u8_size;
    }

	int utf16_to(const CharSet &cs,char **dst,const u16char *src,const int src_size)
	{
		const int src_str_size=(src_size==-1)?strlen(src):src_size;

		const int len=WideCharToMultiByte(cs.codepage,0,src,src_str_size,0,0,0,0);

		if(len<=0)return(len);

		*dst=new char[len];

		return WideCharToMultiByte(cs.codepage,0,src,src_str_size,*dst,len,0,0);
	}

	int utf8_to(const CharSet &cs,char **dst,const char *src,const int src_size)
    {
        int u16str_size;
        u16char *u16str=u8_to_u16(src,src_size,&u16str_size);

        if(!u16str)return(0);

        int result=utf16_to(cs,dst,u16str,u16str_size);

        delete[] u16str;
        return(result);
    }
}//namespace hgl
#else
/**
 * linux iconv 函数细节说明
 * by hyz in Friday,March 30,2012
 *
 * iconv(icont_t *cd,
 * 		char **inbuf,size_t *inbytesleft,
 * 		char **outbuf,size_t *outbytesleft);
 *
 * ps:	1.转换结果并不在outbuf中，而是在最早传入的指针中，所以inbuf,outbuf一定要备份一份出来用
 * 		2.inbytesleft中得到的是剩下还没有转完的总字节数
 * 		3.outbytesleft中得到的是剩下还没有用完的输出区总字节数
 */

#include<iconv.h>
#include<errno.h>
namespace hgl
{
	template<typename T,typename S>
	int CharSetConv(T **out_buf,const CharSetName &out_charset,
					const S *in_str,int in_str_size,const CharSetName &in_charset)
	{
		if(!out_buf||!out_charset||!*out_charset
		 ||!in_str||!*in_str||in_str_size==0||!in_charset||!*in_charset)
			return(-1);

		iconv_t cd=iconv_open(out_charset,in_charset);

		if(cd==(iconv_t)-1)
			return(-1);

		if(in_str_size<0)
			in_str_size=strlen(in_str);

		size_t result;

		iconv_str in;

		char *out;
		T *out_str;
		T *out_new;

		size_t in_left;
		size_t out_left;

		int out_malloc_size=in_str_size;
		int last_left=-1;

		out_str=new T[out_malloc_size+1];

		in=(char *)in_str;
		out=(char *)out_str;
		in_left=in_str_size;
		out_left=out_malloc_size*sizeof(T);

		while(true)
		{
			size_t res=iconv(cd,&in,&in_left,&out,&out_left);

			if(res==(size_t)(-1))
			{
				if(errno==E2BIG)
				{
					out_new=new T[out_malloc_size*2];

					if(!out_new)
						return(false);

					result=out-(char *)out_str;

					memcpy(out_new,out_str,result);

					out_malloc_size*=2;

					delete[] out_str;
					out_str=out_new;

					out=(char *)out_str;
					out+=result;

					//in_left已经被减了，所以不用处理
					out_left=(out_malloc_size*sizeof(T))-result;
					continue;
				}
				else if(errno==EILSEQ)		//有非法字符，跳过
				{
					in++;
					in_left--;
					continue;
				}
			}

			break;
		}

		//result=(out_malloc_size-(out_left/sizeof(T));
		result=((T *)out)-out_str;							// 同上一行

		out_str[result]=0;
		iconv_close(cd);

		*out_buf=out_str;
		return(result);
	}

	int to_utf16(const CharSet &cs,u16char **dst,const char *src,const int src_size)
	{
		return CharSetConv<u16char,char>(dst,endian::GetCharSet<u16char>(),src,src_size,cs.charset);
	}

	int utf16_to(const CharSet &cs,char **dst,const u16char *src,const int src_size)
	{
		return CharSetConv<char,u16char>(dst,cs.charset,src,src_size,endian::GetCharSet<u16char>());
	}

	int to_utf8(const CharSet &cs,char **dst,const char *src,const int src_size)
    {
        return CharSetConv<char,char>(dst,utf8_charset,src,src_size,cs.charset);
    }

    int utf8_to(const CharSet &cs,char **dst,const char *src,const int src_size)
    {
        return CharSetConv<char,char>(dst,cs.charset,src,src_size,utf8_charset);
    }
}//namespace hgl
#endif//HGL_OS == HGL_OS_Windows

namespace hgl
{
    CharSet DefaultCharSet();

    CharSet UTF8CharSet     (ccpUTF8,   utf8_charset    );
    CharSet UTF16LECharSet  (ccpUTF16LE,utf16le_charset );
    CharSet UTF16BECharSet  (ccpUTF16BE,utf16be_charset );

	int	u16_to_u8(char *dst,int dst_size,const u16char *src,const int src_size)
	{
		if(src_size<=0||!src||!*src)
		{
			if(dst&&dst_size>0)
				*dst=0;

			return(0);
		}

		if(!dst||dst_size<=0)
			return(-1);

		const u16char *sp=src;
		uint8 *tp=(uint8 *)dst;

		while(*sp&&(int(tp-(uint8 *)dst)<dst_size))
		{
			if(*sp<=0x7F)						// U-00000000 - U-0000007F: 0xxx
			{
				*tp++=(*sp)&0xFF;
			}
			else if(*sp<=0x7FF)					// U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
			{
				*tp++=0xC0|((*sp>> 6)&0x1F);
				*tp++=0x80|((*sp    )&0x3F);
			}
			else								// U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
			{
				*tp++=0xE0|((*sp>>12)&0x0F);
				*tp++=0x80|((*sp>> 6)&0x3F);
				*tp++=0x80|((*sp    )&0x3F);
			}

			sp++;
		}

//		*tp=0;

		return int(tp-(uint8 *)dst);
	}

	int	u8_to_u16(u16char *dst,int dst_size,const char *src,const int src_size)
	{
		if(src_size<=0||!src||!*src)
		{
			if(dst&&dst_size>0)
				*dst=0;

			return(0);
		}

		if(!dst||dst_size<=0)
			return(-1);

		const uint8 *sp=(uint8 *)src;
		u16char *tp=dst;

		while(*sp&&(int(tp-dst)<dst_size))
		{
			if(*sp<0x80)						//	U-00000000 - U-0000007F: 0xxxxxxx
			{
				*tp=*sp++;
			}
			else if((*sp>=0xC0)&&(*sp<0xE0))	//	U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
			{
				*tp =((*sp++)&0x1F)<<6;
				*tp|=((*sp++)&0x3F);
			}
			else if((*sp>=0xE0)&&(*sp<0xF0))	//	U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
			{
				*tp =((*sp++)&0x0F)<<12;
				*tp|=((*sp++)&0x3F)<<6;
				*tp|=((*sp++)&0x3F);
			}
			else if((*sp>=0xF0)&&(*sp<0xF8))	//	U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			{
				sp+=4;	//不解析
			}
			else if((*sp>=0xF8)&&(*sp<0xFC))	//	U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			{
				sp+=5;	//不解析
			}
			else if((*sp>=0xFC))				//	U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			{
				sp+=6;	//不解析
			}
			else
			{
				*tp=0;
				break;
			}

			tp++;
		}

//		*tp=0;

		return int(tp-dst);
	}

	/**
	 * 转换u16char *字符串到utf8格式char *字符串
	 * @param src 源字符串
	 * @param src_size 源字符串字符数
	 * @param dst_size 结果字符串字符数
	 * @return 转换结果，需自行delete[]
	 */
	char *u16_to_u8(const u16char *src,const int src_size,int &dst_size)
	{
		if(src_size<=0||!src||!*src)
		{
			dst_size=0;
			return(nullptr);
		}

		const u16char *sp=src;
		dst_size=0;

		while(*sp&&(sp-src<src_size))
		{
			if(*sp<=0x7F)
			{
				sp++;
				dst_size++;
			}
			else
			if(*sp<=0x7FF)
			{
				sp++;
				dst_size+=2;
			}
			else
			{
				sp++;
				dst_size+=3;
			}
		}

		if(dst_size<=0)
            return(nullptr);

		char *dst=new char[dst_size+1];

		dst[dst_size]=0;		//为防止内存检测工具报错，所以提前赋0

		u16_to_u8(dst,dst_size,src,src_size);

		return dst;
	}

	/**
	 * 转换utf8格式char *字符串到u16char *字符串
	 * @param src 源字符串
	 * @param src_size 源字符串字符数
	 * @param dst_size 结果字符串字符数
	 * @return 转换结果，需自行delete[]
	 */
	u16char *u8_to_u16(const char *src,const int src_size,int &dst_size)
	{
		if(src_size<=0||!src||!*src)
		{
			dst_size=0;
            return(nullptr);
		}

		uint8 *sp=(uint8 *)src;

		dst_size=0;

		while(*sp&&(sp-(uint8 *)src)<src_size)
		{
			if(*sp<0x80)						//	U-00000000 - U-0000007F: 0xxxxxxx
			{
				sp++;
				dst_size++;
			}
			else if((*sp>=0xC0)&&(*sp<0xE0))	//	U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
			{
				sp+=2;
				dst_size++;
			}
			else if((*sp>=0xE0)&&(*sp<0xF0))	//	U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
			{
				sp+=3;
				dst_size++;
			}
			else if((*sp>=0xF0)&&(*sp<0xF8))	//	U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			{
				sp+=4;	//不解析
			}
			else if((*sp>=0xF8)&&(*sp<0xFC))	//	U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			{
				sp+=5;	//不解析
			}
			else if((*sp>=0xFC))				//	U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
			{
				sp+=6;	//不解析
			}
			else
			{
				break;
			}
		}

		if(dst_size<=0)
            return(nullptr);

		u16char *dst=new u16char[dst_size+1];

		dst[dst_size]=0;		//为防止内存检测工具报错，所以提前赋0

		u8_to_u16(dst,dst_size,src,src_size);

		return dst;
	}
}//namespace hgl
