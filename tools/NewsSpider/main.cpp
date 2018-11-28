#include<hgl/Console.h>
#include<hgl/network/HTTPTools.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/LogInfo.h>

#include<gumbo-parser/gumbo.h>      //google html5 解晰库
#include<json/json.h>               //JSON 解晰库

#include<iostream>

using namespace hgl;

const GumboAttribute *GetAttr(const GumboNode *node,const char *name)
{
    for(int i=0;i<node->v.element.attributes.length;i++)
    {
        const GumboAttribute *attr=(const GumboAttribute *)(node->v.element.attributes.data[i]);

        if(strcmp(attr->name,name)==0)
            return attr;
    }

    return nullptr;
}

class HTMLParse
{
public:

    HTMLParse()=default;
    virtual ~HTMLParse()=default;

    virtual void ParseNode(const GumboNode *node)=0;

    virtual void Parse(const GumboNode *node)
    {
        ParseNode(node);

        if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

        for(int i=0;i<node->v.element.children.length;i++)
            Parse((const GumboNode *)(node->v.element.children.data[i]));
    }
public:

    void Parse(const char *url)
    {
        io::MemoryOutputStream mos;

        network::http::get(&mos,url);

        GumboOutput *output=gumbo_parse_with_options(&kGumboDefaultOptions,(const char *)mos.GetData(),mos.Tell());

        Parse(output->root);

        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
};//class HTMLParse

class SinaNewsPageParse
{
public:

    void ParseNewsPage(const GumboNode *node)
    {
    }

};//

constexpr char NEWS_SINA_COM_CN_HEADER[]="https://news.sina.com.cn/";
constexpr uint NEWS_SINA_COM_CN_HEADER_SIZE=sizeof(NEWS_SINA_COM_CN_HEADER)-1;
constexpr char NEWS_LINK_END[]=".shtml";
constexpr uint NEWS_LINK_END_SIZE=sizeof(NEWS_LINK_END)-1;

/**
 * 页内新闻链接查找
 * 范例: <a target="_blank" href="https://news.sina.com.cn/c/xl/2018-11-28/doc-ihpevhcm2060930.shtml">习近平出席西班牙国王举行的盛大欢迎仪式</a>
 * 判断依据: 为A点节，至少有两属性target,href，只有一个TEXT类型子节点
 *          target为"_blank"
 *          href前缀为"https://news.sina.com.cn"
 *          href结尾为".shtml"
 */
class SinaNewsFirstPageParse:public HTMLParse
{
public:

    void ParseNode(const GumboNode *node) override
    {
        if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点
        if(node->v.element.tag!=GUMBO_TAG_A)return;     //不是A节点
        if(node->v.element.attributes.length<2)return;  //属性太少
        if(node->v.element.children.length!=1)return;   //子节点数量不对

        const GumboAttribute *target=GetAttr(node,"target");
        const GumboAttribute *href=GetAttr(node,"href");

        if(!target)return;
        if(!href)return;

        if(strcmp(target->value,"_blank"))return;
        if(strncmp(href->value,NEWS_SINA_COM_CN_HEADER,NEWS_SINA_COM_CN_HEADER_SIZE))return;

        const char *end_href=href->value+strlen(href->value)-NEWS_LINK_END_SIZE;

        if(strcmp(end_href,NEWS_LINK_END))return;

        const GumboNode *sub_node=(const GumboNode *)(node->v.element.children.data[0]);

        if(sub_node->type!=GUMBO_NODE_TEXT)return;

        std::cout<<"news: "<<sub_node->v.text.text<<std::endl;
        std::cout<<"link: "<<href->value<<std::endl;

        //ParseNewsPage(href->value,sub_node->v.text.text);
    }
};//

HGL_CONSOLE_MAIN_FUNC()
{
    sii.info.ProjectName=U8_TEXT("新闻爬虫");
    sii.info.ProjectCode=OS_TEXT("NewsSpider");

    if(!app.Init(&sii))
        return(-1);

    SinaNewsFirstPageParse sina;

    sina.Parse("https://news.sina.com.cn");
    return 0;
}
