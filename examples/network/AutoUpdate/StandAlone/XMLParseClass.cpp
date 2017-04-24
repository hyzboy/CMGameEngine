#include"XMLParse.h"
#include<expat.h>

constexpr uint XML_PARSE_MAX_SIZE=1024;             //最大一次解晰长度

namespace
{
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
