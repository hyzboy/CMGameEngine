#include<hgl/CodePage.h>

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
