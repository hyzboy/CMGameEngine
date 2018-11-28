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

void ParseNewsPage(const char *url)
{
}

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

constexpr char NEWS_SINA_COM_CN_HEADER[]="https://news.sina.com.cn/";
constexpr uint NEWS_SINA_COM_CN_HEADER_SIZE=sizeof(NEWS_SINA_COM_CN_HEADER)-1;
constexpr char NEWS_LINK_END[]=".shtml";
constexpr uint NEWS_LINK_END_SIZE=sizeof(NEWS_LINK_END)-1;

/**
 * 节点查找
 * 范例: <a target="_blank" href="https://news.sina.com.cn/c/xl/2018-11-28/doc-ihpevhcm2060930.shtml">习近平出席西班牙国王举行的盛大欢迎仪式</a>
 * 判断依据: 为A点节，至少有两属性target,href，只有一个TEXT类型子节点
 *          target为"_blank"
 *          href前缀为"https://news.sina.com.cn"
 *          href结尾为".shtml"
 */
void ParseNode(const GumboNode *node)
{
    if(node->type!=GUMBO_NODE_ELEMENT)return;

    if(node->v.element.tag==GUMBO_TAG_A        //寻找A节点
     &&node->v.element.attributes.length>=2    //至少2属性
     &&node->v.element.children.length==1)     //只有一个子节点
    {
        const GumboAttribute *target=GetAttr(node,"target");
        const GumboAttribute *href=GetAttr(node,"href");

        if(target&&href)
        {
            if(strcmp(target->value,"_blank")==0
             &&strncmp(href->value,NEWS_SINA_COM_CN_HEADER,NEWS_SINA_COM_CN_HEADER_SIZE)==0)
            {
                const char *end_href=href->value+strlen(href->value)-NEWS_LINK_END_SIZE;

                if(strcmp(end_href,NEWS_LINK_END)==0)
                {
                    const GumboNode *sub_node=(const GumboNode *)(node->v.element.children.data[0]);

                    if(sub_node->type==GUMBO_NODE_TEXT)     //是文本，确定它是新闻链接
                    {
                        std::cout<<"news: "<<sub_node->v.text.text<<std::endl;
                        std::cout<<"link: "<<href->value<<std::endl;
                    }
                }
            }
        }
    }

    for(int i=0;i<node->v.element.children.length;i++)
        ParseNode((const GumboNode *)(node->v.element.children.data[i]));
}

void ParseFirstPage(const void *data,const int size)
{
    GumboOutput *output=gumbo_parse_with_options(&kGumboDefaultOptions,(const char *)data,size);

    ParseNode(output->root);

    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

HGL_CONSOLE_MAIN_FUNC()
{
    sii.info.ProjectName=U8_TEXT("新闻爬虫");
    sii.info.ProjectCode=OS_TEXT("NewsSpider");

    if(!app.Init(&sii))
        return(-1);

    UTF8String url="https://news.sina.com.cn";

    io::MemoryOutputStream mos;

    network::http::get(&mos,url.c_str());

    filesystem::SaveMemoryToFile(OS_TEXT("sina.html"),mos.GetData(),mos.Tell());

    ParseFirstPage(mos.GetData(),mos.Tell());
    return 0;
}
