#include<hgl/CodePage.h>

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

    int to_utf8(const CharSet &cs,char **dst,const char *src,const int src_size)
    {
        u16char *u16str;
        int u16size=to_utf16(cs,&u16str,src,src_size);

        if(u16size<=0)return(u16size);

        int u8_size;
        *dst = u16_to_u8(u16str, u16size, u8_size);

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
        u16char *u16str=u8_to_u16(src,src_size,u16str_size);

        if(!u16str)return(0);

        int result=utf16_to(cs,dst,u16str,u16str_size);

        delete[] u16str;
        return(result);
    }
}//namespace hgl


