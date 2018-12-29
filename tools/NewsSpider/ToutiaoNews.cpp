#include<hgl/Console.h>
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

UTF8String user_agent;

void InitUserAgent()
{
    FirefoxUserAgentConfig cfg;

    cfg.os              =OS_iPhone;

    cfg.os_ver.major    =12;
    cfg.os_ver.minor    =1;

    cfg.ff_ver.major    =64;
    cfg.ff_ver.minor    =0;

    user_agent=FirefoxUserAgent(cfg);
}

class HTMLParse
{
private:

    UTF8String web_url;
    MemoryOutputStream mos;

    CharSet page_charset;

protected:

    const UTF8String GetSubText(const GumboNode *node,int index)
    {
        if(index<0||index>=node->v.element.children.length)
            return(nullptr);

        const GumboNode *sub_node=(const GumboNode *)(node->v.element.children.data[index]);

        if(sub_node->type!=GUMBO_NODE_TEXT)
            return(nullptr);

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

        ParseNode(node);

        for(int i=0;i<node->v.element.children.length;i++)
            Parse((const GumboNode *)(node->v.element.children.data[i]));
    }

public:

    void Parse(const UTF8String &url)
    {
        web_url=url;
        network::http::get(&mos,url.c_str(),user_agent);

        SaveMemoryToFile("FirstPage.html",mos.GetData(),mos.Tell());

        GumboOutput *output=gumbo_parse_with_options(&kGumboDefaultOptions,(const char *)mos.GetData(),mos.Tell());

        Parse(output->root);

        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
};//class HTMLParse

class FirstPageParse:public HTMLParse
{
public:

    using HTMLParse::HTMLParse;
    ~FirstPageParse()=default;

    void ParseNode(const GumboNode *node) override
    {
    }
};//class FirstPageParse:public HTMLParse

HGL_CONSOLE_MAIN_FUNC()
{
    if(args.GetCount()<2)
    {
        std::cerr<<"use: ToutianNews name"<<std::endl;
        return 0;
    }

    InitUserAgent();

    FirstPageParse fpp;

    const UTF8String url="http://newssearch.chinadaily.com.cn/cn/search?query="+args[1];

    fpp.Parse(url);

    return 0;
}

