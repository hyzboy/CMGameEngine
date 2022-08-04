﻿#ifndef HTML_PARSE_INCLUDE
#define HTML_PARSE_INCLUDE

#include<hgl/filesystem/FileSystem.h>
#include<hgl/network/HTTPTools.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/LogInfo.h>
#include<hgl/type/StringList.h>
#include<hgl/webapi/UserAgentString.h>

#if HGL_OS == HGL_OS_Windows
#include<gumbo.h>
#else
#include<gumbo-parser/gumbo.h>
#endif//

#include<iostream>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::filesystem;
using namespace hgl::webapi;

constexpr char CHARSET_ATTR[]="charset=";
constexpr uint CHARSET_ATTR_SIZE=sizeof(CHARSET_ATTR)-1;

inline const GumboAttribute *GetAttr(const GumboNode *node,const char *name)
{
    if(node->type!=GUMBO_NODE_ELEMENT)return(nullptr);

    for(int i=0;i<node->v.element.attributes.length;i++)
    {
        const GumboAttribute *attr=(const GumboAttribute *)(node->v.element.attributes.data[i]);

        if(strcmp(attr->name,name)==0)
            return attr;
    }

    return nullptr;
}

inline bool CheckAttr(const GumboNode *node,const char *name,const char *value)
{
    if(node->type!=GUMBO_NODE_ELEMENT)return(false);

    for(int i=0;i<node->v.element.attributes.length;i++)
    {
        const GumboAttribute *attr=(const GumboAttribute *)(node->v.element.attributes.data[i]);

        if(strcmp(attr->name,name)==0)
            return(!strcmp(attr->value,value));
    }

    return false;
}

class HTMLParse
{
private:

    UTF8String web_url;
    MemoryOutputStream mos;

protected:

    CharSet page_charset;
    UTF8String user_agent;

protected:

    const UTF8String GetSubText(const GumboNode *node,bool sub=true) const
    {
        if(node->type!=GUMBO_NODE_ELEMENT)return UTF8String("");       //没有子节点

        for(int i=0;i<node->v.element.children.length;i++)
        {
            const GumboNode *sub_node=(const GumboNode *)(node->v.element.children.data[i]);

            if(sub_node->type!=GUMBO_NODE_TEXT)
            {
                if(!sub)
                    return UTF8String("");

                UTF8String str=GetSubText(sub_node);

                if(str.Length()>0)
                    return str;

                continue;
            }

            if(page_charset.codepage==ccpUTF8)
                return UTF8String(sub_node->v.text.text);
            else
            {
                char *u8str;

                int len=to_utf8(page_charset,
                                &u8str,
                                sub_node->v.text.original_text.data,
                                sub_node->v.text.original_text.length);

                return UTF8String(u8str,len,true);
            }
        }

        return UTF8String("");
    }

    void GetSubText(const GumboNode *node,UTF8String &text) const
    {
        if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

        for(int i=0;i<node->v.element.children.length;i++)
        {
            const GumboNode *sub_node=(const GumboNode *)(node->v.element.children.data[i]);

            if(sub_node->type!=GUMBO_NODE_TEXT)
            {
                UTF8String str=GetSubText(sub_node);

                if(str.Length()>0)
                    text+=str;

                continue;
            }

            if(page_charset.codepage==ccpUTF8)
            {
                text+=UTF8String(sub_node->v.text.text);
            }
            else
            {
                char *u8str;

                int len=to_utf8(page_charset,
                                &u8str,
                                sub_node->v.text.original_text.data,
                                sub_node->v.text.original_text.length);

                text+=UTF8String(u8str,len,true);
            }
        }
    }

    const GumboNode *GetSubNode(const GumboNode *node,const uint tag) const
    {
        if(node->type!=GUMBO_NODE_ELEMENT)return nullptr;

        for(int i=0;i<node->v.element.children.length;i++)
        {
            const GumboNode *sub_node=(const GumboNode *)(node->v.element.children.data[i]);

            if(sub_node->v.element.tag==tag)
                return sub_node;
            else
            {
                sub_node=GetSubNode(sub_node,tag);

                if(sub_node)
                    return sub_node;
            }
        }

        return nullptr;
    }

    const GumboNode *GetSubNode(const GumboNode *node,const uint tag,const UTF8String &key,const UTF8String &value)
    {
        if(node->type!=GUMBO_NODE_ELEMENT)return nullptr;

        for(int i=0;i<node->v.element.children.length;i++)
        {
            const GumboNode *sub_node=(const GumboNode *)(node->v.element.children.data[i]);

            if(sub_node->v.element.tag==tag
             &&CheckAttr(sub_node,key,value))
                return sub_node;

            sub_node=GetSubNode(sub_node,tag,key,value);

            if(sub_node)
                return sub_node;
        }

        return nullptr;
    }

public:

    HTMLParse()
    {
        page_charset=CharSet("utf-8");
    }

    virtual ~HTMLParse()=default;

    virtual void ParseNode(const GumboNode *node)=0;

    virtual void Parse(const GumboNode *node)
    {
        if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

        if(node->v.element.tag==GUMBO_TAG_META)
        {
            const GumboAttribute *content=GetAttr(node,"content");

            if(!content)return;
            const char *pos=strstr(content->value,CHARSET_ATTR);

            if(pos)
                page_charset=CharSet(pos+CHARSET_ATTR_SIZE);

            return;
        }

        ParseNode(node);

        for(int i=0;i<node->v.element.children.length;i++)
            Parse((const GumboNode *)(node->v.element.children.data[i]));
    }

public:

    void Parse(const UTF8String &url,const UTF8String &ua)
    {
        web_url=url;
        user_agent=ua;

        mos.Restart();

        network::http::get(&mos,url.c_str(),user_agent);

//         SaveMemoryToFile("FirstPage.html",mos.GetData(),mos.Tell());

        GumboOutput *output=gumbo_parse_with_options(&kGumboDefaultOptions,(const char *)mos.GetData(),mos.Tell());

        Parse(output->root);

        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
};//class HTMLParse
#endif//HTML_PARSE_INCLUDE