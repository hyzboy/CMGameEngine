#include<hgl/CodePage.h>

namespace hgl
{
    CharSet DefaultCharSet();

    CharSet UTF8CharSet     (ccpUTF8,   utf8_charset    );
    CharSet UTF16LECharSet  (ccpUTF16LE,utf16le_charset );
    CharSet UTF16BECharSet  (ccpUTF16BE,utf16be_charset );

    int    u16_to_u8(char *dst,int dst_size,const u16char *src,const int src_size)
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
            if(*sp<=0x7F)                       // U-00000000 - U-0000007F: 0xxx
            {
                *tp++=(*sp)&0xFF;
            }
            else if(*sp<=0x7FF)                 // U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
            {
                *tp++=0xC0|((*sp>> 6)&0x1F);
                *tp++=0x80|((*sp    )&0x3F);
            }
            else                                // U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
            {
                *tp++=0xE0|((*sp>>12)&0x0F);
                *tp++=0x80|((*sp>> 6)&0x3F);
                *tp++=0x80|((*sp    )&0x3F);
            }

            sp++;
        }

//        *tp=0;

        return int(tp-(uint8 *)dst);
    }

    int    u8_to_u16(u16char *dst,int dst_size,const char *src,const int src_size)
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
            if(*sp<0x80)                        //    U-00000000 - U-0000007F: 0xxxxxxx
            {
                *tp=*sp++;
            }
            else if((*sp>=0xC0)&&(*sp<0xE0))    //    U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
            {
                *tp =((*sp++)&0x1F)<<6;
                *tp|=((*sp++)&0x3F);
            }
            else if((*sp>=0xE0)&&(*sp<0xF0))    //    U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
            {
                *tp =((*sp++)&0x0F)<<12;
                *tp|=((*sp++)&0x3F)<<6;
                *tp|=((*sp++)&0x3F);
            }
            else if((*sp>=0xF0)&&(*sp<0xF8))    //    U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            {
                sp+=4;    //不解析
            }
            else if((*sp>=0xF8)&&(*sp<0xFC))    //    U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            {
                sp+=5;    //不解析
            }
            else if((*sp>=0xFC))                //    U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            {
                sp+=6;    //不解析
            }
            else
            {
                *tp=0;
                break;
            }

            tp++;
        }

//        *tp=0;

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

        dst[dst_size]=0;        //为防止内存检测工具报错，所以提前赋0

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
            if(*sp<0x80)                        //    U-00000000 - U-0000007F: 0xxxxxxx
            {
                sp++;
                dst_size++;
            }
            else if((*sp>=0xC0)&&(*sp<0xE0))    //    U-00000080 - U-000007FF: 110xxxxx 10xxxxxx
            {
                sp+=2;
                dst_size++;
            }
            else if((*sp>=0xE0)&&(*sp<0xF0))    //    U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
            {
                sp+=3;
                dst_size++;
            }
            else if((*sp>=0xF0)&&(*sp<0xF8))    //    U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            {
                sp+=4;    //不解析
            }
            else if((*sp>=0xF8)&&(*sp<0xFC))    //    U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            {
                sp+=5;    //不解析
            }
            else if((*sp>=0xFC))                //    U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            {
                sp+=6;    //不解析
            }
            else
            {
                break;
            }
        }

        if(dst_size<=0)
            return(nullptr);

        u16char *dst=new u16char[dst_size+1];

        dst[dst_size]=0;        //为防止内存检测工具报错，所以提前赋0

        u8_to_u16(dst,dst_size,src,src_size);

        return dst;
    }

    /**
     * 分析这个unicode文本文件的字节序
     * @param input 输入数据
     * @return 字节序
     */
    const BOMFileHeader *ParseBOM(const void *input)
    {
        if(!input)return(nullptr);

        const BOMFileHeader *bfh=BOMData+bomUTF8;

        for(uint i=bomUTF8;i<bomEnd;i++)
        {
            if(memcmp(input,bfh->data,bfh->size)==0)
                return bfh;

            ++bfh;
        }

        return nullptr;
    }

    /**
     * 转换BOM数据到CharSet结构
     * @param bom BOM数据
     * @param cs 字符集数据结构
     * @return 是否转换成功
     */
    bool BOM2CharSet(CharSet *cs,const BOMFileHeader *bom)
    {
        if(!cs)return(false);
        if(!bom)return(false);

        if(bom->bom<=bomNone||bom->bom>=bomEnd)return(false);

        cs->codepage=bom->code_page;
        memcpy(cs->charset,bom->char_set,sizeof(CharSetName));
        return(true);
    }
}//namespace hgl
