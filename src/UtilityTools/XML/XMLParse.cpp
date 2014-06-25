#include<hgl/ut/XMLParse.h>
#include<hgl/File.h>
#include<expat.h>

namespace hgl
{
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

	bool XMLParse::ParseFile(const OSString &filename)
	{
		if(filename.Length()<=0)return(false);

		LOG_INFO(OS_TEXT("XMLParse::ParseFile(filename=")+filename+OS_TEXT(")"));

		char *data;
		int size;

		size=LoadFileToMemory(filename,(void **)&data);

		if(size<=0)
		{
			LOG_INFO(OS_TEXT("XMLParse::ParseFile(filename=")+filename+OS_TEXT(") Load File Failed."));
			return(false);
		}

		const bool result=Parse(data,size,true);

		delete[] data;
		return(result);
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
