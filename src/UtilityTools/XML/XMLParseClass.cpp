//取名为XMLParseClass是为了避免与expat的xmlparse.c编译造成obj冲突

#include<hgl/ut/XMLParse.h>
#include<hgl/io/FileInputStream.h>
#include<hgl/FileSystem.h>
#include<expat.h>

namespace hgl
{
    namespace
    {        
        constexpr uint HGL_XML_PARSE_MAX_SIZE=HGL_SIZE_1KB*128;         //最大一次解晰长度

        void XMLStartElement(XMLParse *xml,const XML_Char *name,const XML_Char **atts)
        {
            xml->StartElement(name,atts);
        }

        void XMLCharData(XMLParse *xml,const XML_Char *str,int len)
        {
            xml->CharData(str,len);
        }

        void XMLEndElement(XMLParse *xml,const XML_Char *name)
        {
            xml->EndElement(name);
        }
    }

    XMLParse::XMLParse()
    {
        xml=nullptr;
    }

    XMLParse::~XMLParse()
    {
        if(!xml)return;

        XML_ParserFree(xml);
    }

    void XMLParse::StartParse()
    {
        XML_SetElementHandler(xml,(XML_StartElementHandler)XMLStartElement,(XML_EndElementHandler)XMLEndElement);
        XML_SetCharacterDataHandler(xml,(XML_CharacterDataHandler)XMLCharData);
    }

    /**
    * 重新开始一次解晰
    */
    void XMLParse::Start()
    {
        if(xml)
        {
            XML_ParserReset(xml,"utf-8");
        }
        else
        {
            xml=XML_ParserCreate("utf-8");

            XML_SetUserData(xml,this);

            StartParse();
        }
    }

    /**
    * 解晰一段xml文本
    * @param buf 文本缓冲区
    * @param len 文本长度
    * @param isFin 是否结束
    * @return 此次解晰是否成功
    */
    bool XMLParse::Parse(const char *buf,int len,bool isFin)
    {
        if(!xml)return(false);

        return XML_Parse(xml,buf,len,isFin);
    }

    bool hgl::XMLParse::Parse(io::InputStream *is, bool isFin)
    {
        if(!is)return(false);
        
        if(is->CanSize()&&is->GetSize()<=HGL_XML_PARSE_MAX_SIZE)        //可以取长度的，并且<=指定长度的一次读完
        {
            int full_size=is->Available();

            char *data=new char[full_size];

            int pos=0;
            int size;

            bool result;

            while(pos<full_size)
            {
                size=is->ReadFully(data,full_size);

                if(size<0)
                    return(false);

                result=Parse(data,size,pos+size>=full_size);

                if(!result)
                    return(false);

                pos+=size;
            }

            return(true);
        }
        else                    //不能取长度或是大于指定长度的
        {
            char data[HGL_XML_PARSE_MAX_SIZE];

            int size;
            bool result;

            for(;;)
            {
                size=is->Available();

                if(size<=0)break;

                if(size>HGL_XML_PARSE_MAX_SIZE)
                {
                    size=is->Read(data,HGL_XML_PARSE_MAX_SIZE);
                    result=Parse(data,HGL_XML_PARSE_MAX_SIZE,false);
                }
                else
                {
                    size=is->Read(data,size);
                    result=Parse(data,size,true);
                }

                if(!result)return(false);
            }

            return(true);
        }
    }

    /**
     * 解晰一个XML文件
     */
    bool XMLParseFile(XMLParse *xml,const OSString &filename)
    {
        if(!xml)return(false);
        if(filename.IsEmpty())return(false);
        if(!filesystem::FileCanRead(filename))return(false);

        io::OpenFileInputStream fis(filename);

        return xml->Parse(&fis);
    }

    namespace
    {
        void StartElementCB(XMLParseCB *xml,const XML_Char *name,const XML_Char **atts)
        {
            SafeCallEvent(xml->OnStartElement,(name,atts));
        }

        void CharDataCB(XMLParseCB *xml,const XML_Char *str,int len)
        {
            SafeCallEvent(xml->OnCharData,(str,len));
        }

        void EndElementCB(XMLParseCB *xml,const XML_Char *name)
        {
            SafeCallEvent(xml->OnEndElement,(name));
        }
    }

    XMLParseCB::XMLParseCB()
    {
        OnStartElement=nullptr;
        OnCharData=nullptr;
        OnEndElement=nullptr;
    }

    void XMLParseCB::StartParse()
    {
        XML_SetElementHandler(xml,(XML_StartElementHandler)StartElementCB,(XML_EndElementHandler)EndElementCB);
        XML_SetCharacterDataHandler(xml,(XML_CharacterDataHandler)CharDataCB);
    }
}//namespace hgl
