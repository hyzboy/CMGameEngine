#pragma once

#include<string>
#include<cstring>

extern "C"
{
	struct XML_ParserStruct;

	using XML_Parser=struct XML_ParserStruct *;
}

/**
* XML解析器(虚拟函数版)<br>
*/
class XMLParse
{
protected:

    XML_Parser xml;

    virtual void StartParse();

public:

    XMLParse();
    ~XMLParse();
    
    virtual void StartElement(const char *element_name,const char **atts)=0;
    virtual void CharData(const char *str,int str_length){};
    virtual void EndElement(const char *element_name){};

public:

    virtual void Start();
    virtual bool Parse(const char *buf,int len,bool isFin);
};//class XMLParse

bool XMLParseFile(XMLParse *xml,const std::wstring &filename);

#define XML_START_PARSE(name)	while(*name)	\
								{	\
									const char *flag=*name;++name;	\
									const char *info=*name;++name;

#define XML_END_PARSE				std::cout<<__FILE__<<":"<<__LINE__<<" can't parse atts \""<<flag<<"\" , info \""<<info<<"\"."<<std::endl;  \
								}

#define XML_END_PARSE_SKIP()    ;}

#define xml_parse_skip(name)		        if(std::strcmp(flag,#name)==0)continue;else
#define xml_parse_string_u16(name)	        if(std::strcmp(flag,#name)==0)name=to_u16(info);else
#define xml_parse_string_u8(name)	        if(std::strcmp(flag,#name)==0)name=info;else
#define xml_parse_int(name)                 if(std::strcmp(flag,#name)==0)name=atoi(info);else
#define xml_parse_md5(name)                 if(std::strcmp(flag,#name)==0)ToMD5(name,info);else

#define xml_parse_to_string_u8(name,value)   if(std::strcmp(flag,name)==0)value=info;else

/** 使用范例:

    <update>
        <file name="Hello.exe" 
                md5="BFBE152A7F2CE8ACFE69A8A3624CB0D6" size="83456"
                compress_md5="173226395464F435B67DFE2BD7251F1C" compress_size="41458"/>
    </update>

	void StartElement(const char *element_name,const char **atts) override
    {
        if(strcmp(element_name,"file")==0)
        {
            std::string name;
            MD5Code md5;
            int size;
            MD5Code compress_md5;
            int compress_size;

            XML_START_PARSE(atts)

                xml_parse_string_u8(name)
                xml_parse_md5(md5)
                xml_parse_int(size)
                xml_parse_md5(compress_md5)
                xml_parse_int(compress_size)

            XML_END_PARSE

            fc_list.push_back(FileCheck(name,md5,size,compress_md5,compress_size));
        }
    }
*/
