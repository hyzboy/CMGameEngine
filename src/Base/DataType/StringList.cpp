#include<hgl/type/StringList.h>
#include<hgl/filesystem/FileSystem.h>
namespace hgl
{
    /**
     * 加载一个原始文本块到UTF8StringList
     */
    int LoadStringListFromText(UTF8StringList &sl,uchar *data,const int size,const CharSet &cs)
    {
        char *str=nullptr;

        int line_count;
        int char_count;

        if(size>=3&&data[0]==0xEF&&data[1]==0xBB&&data[2]==0xBF)            //utf8
            line_count=SplitToStringListByEnter<char>(sl,(char *)(data+3),size-3);
        else
        if(cs==UTF8CharSet)
            line_count=SplitToStringListByEnter<char>(sl,(char *)data,size);
        else
        {
            if(size>=2)
            {
                int u16_count;

                if(data[0]==0xFF&&data[1]==0xFE)                                //LE
                {
                    if(size>=4&&data[2]==0&&data[3]==0)                         //utf32-le
                    {
                        u16_count=(size-4)>>2;
                        u16char *u16str=new u16char[u16_count];

                        LittleToCurrentEndian(u16str,(uint32 *)(data+4),u16_count);

                        str=u16_to_u8(u16str,u16_count,char_count);
                        delete[] u16str;
                    }
                    else                                                        //utf16-le
                    {
                        u16_count=(size-2)>>1;
                        LittleToCurrentEndian((u16char *)(data+2),u16_count);
                        str=u16_to_u8((u16char *)(data+2),u16_count,char_count);
                    }
                }
                else
                if(data[0]==0&&data[1]==0&&data[2]==0xFE&&data[3]==0xFF)        //utf32-be
                {
                    u16_count=(size-4)>>2;
                    u16char *u16str=new u16char[u16_count];

                    BigToCurrentEndian(u16str,(uint32 *)(data+4),u16_count);

                    str=u16_to_u8(u16str,u16_count,char_count);
                    delete[] u16str;
                }
                else
                if(data[0]==0xFE&&data[1]==0xFF)                                //utf16-be
                {
                    u16_count=(size-2)>>1;
                    BigToCurrentEndian((u16char *)(data+2),u16_count);
                    str=u16_to_u8((u16char *)(data+2),u16_count,char_count);
                }
            }

            if(!str)
#ifdef __ANDROID__
                return 0;
#else
                char_count=to_utf8(cs,&str,(char *)data,size);
#endif//

            line_count=SplitToStringListByEnter<char>(sl,str,char_count);

            delete[] str;
        }

        delete[] data;
        return line_count;
    }

    /**
     * 加载一个原始文本块到UTF16StringList
     */
    int LoadStringListFromText(UTF16StringList &sl,uchar *data,const int size,const CharSet &cs)
    {
        u16char *str=nullptr;

        int char_count=0;
        int line_count;

        if(size>=2)
        {
            if(data[0]==0xFF&&data[1]==0xFE)                                //LE
            {
                if(size>=4&&data[2]==0&&data[3]==0)                         //utf32-le
                {
                    str=(u16char *)data;        //32->16使用原缓冲区覆盖
                    char_count=(size-4)>>2;
                    LittleToCurrentEndian(str,(uint32 *)(data+4),char_count);
                }
                else                                                        //utf16-le
                {
                    str=(u16char *)(data+2);
                    char_count=(size-2)>>1;
                    LittleToCurrentEndian(str,char_count);
                }
            }
            else
            if(data[0]==0&&data[1]==0&&data[2]==0xFE&&data[3]==0xFF)        //utf32-be
            {
                str=(u16char *)data;            ////32->16使用原缓冲区覆盖
                char_count=(size-4)>>2;
                BigToCurrentEndian(str,(uint32 *)(data+4),char_count);
            }
            else
            if(data[0]==0xFE&&data[1]==0xFF)                                //utf16-be
            {
                str=(u16char *)(data+2);
                char_count=(size-2)>>1;
                BigToCurrentEndian(str,char_count);
            }
        }

        if((uchar *)str>=data&&(uchar *)str<=data+size)                      //如果str的地址在data的范围内
        {
            line_count=SplitToStringListByEnter<u16char>(sl,str,char_count);
        }
        else
        {
            if(size>=3&&data[0]==0xEF&&data[1]==0xBB&&data[2]==0xBF)        //utf8
                str=u8_to_u16((char *)(data+3),size-3,char_count);
            else
            if(cs==UTF8CharSet)
                str=u8_to_u16((char *)data,size,char_count);
            else
            {
#ifdef __ANDROID__
                return 0;
#else
                char_count=to_utf16(cs,&str,(char *)data,size);
#endif//
            }

            line_count=SplitToStringListByEnter<u16char>(sl,str,char_count);

            delete[] str;
        }

        delete[] data;
        return line_count;
    }

    /**
     * 加载一个原始文本文件到UTF8StringList
     */
    int LoadStringListFromTextFile(UTF8StringList &sl,const OSString &filename,const CharSet &cs)
    {
        uchar *data;

        const int size=filesystem::LoadFileToMemory(filename,(void **)&data);

        if(size<=0)
            return size;

        return LoadStringListFromText(sl,data,size,cs);
    }

      /**
     * 加载一个原始文本文件到UTF16StringList
     */
    int LoadStringListFromTextFile(UTF16StringList &sl,const OSString &filename,const CharSet &cs)
    {
        uchar *data;

        const int size=filesystem::LoadFileToMemory(filename,(void **)&data);

        if(size<=0)
            return size;

        return LoadStringListFromText(sl,data,size,cs);
    }
}//namespace hgl
