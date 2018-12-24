
#include"HTMLParse.h"
#include"NewsStorage.h"
#include"NewsCreater.h"

#include<hgl/Console.h>
#include<hgl/webapi/UserAgentString.h>

using namespace hgl::webapi;

namespace
{
    constexpr char SPAR_CHAR[]=u8"：";
    constexpr uint SPAR_CHAR_SIZE=sizeof(SPAR_CHAR)-1;

    constexpr char TEXT_SOURCE[]=u8"来源 : ";
    constexpr uint TEXT_SOURCE_SIZE=sizeof(TEXT_SOURCE)-1;

    constexpr char URL_HTTP_CHAOZHOUDAILY_COM[]="http://chaozhoudaily.com";

    class NewsPageParse:public HTMLParse
    {
        NewsStorage *storage;
        OSString save_path;

        NewsInfo *ni;

        NewsCreater *creater;

    public:

        NewsPageParse(NewsStorage *ns,const OSString &sp,const UTF8String &title,const UTF8String &src_link,const UTF8String &post_time_text)
        {
            storage=ns;

            ni=new NewsInfo;
            ni->title=title;
            ni->index=storage->GetMaxIndex();
            ni->post_time=post_time_text;
            ni->tags.Add(U8_TEXT("广东"));
            ni->tags.Add(U8_TEXT("潮汕"));
            ni->tags.Add(U8_TEXT("潮州"));
            ni->src_link=src_link;

            save_path=MergeFilename(sp,OSString(ni->index));
            MakePath(save_path);

            creater=new NewsCreater(save_path);
        }

        ~NewsPageParse()
        {
            ni->first_image =creater->first_image;
            ni->first_line  =creater->first_line;
            ni->img_count   =creater->img_count;

            creater->Save();

            storage->Add(ni);

            delete creater;
            std::cout<<ni->index<<" : "<<ni->title.c_str()<<std::endl;
        }

        void ParseSourceNode(const GumboNode *node)
        {
            if(node->v.element.tag==GUMBO_TAG_SPAN)
            {
                UTF8String text=GetSubText(node);

                text.Trim();

                if(!text.IsEmpty())
                {
                    int pos=text.FindString(TEXT_SOURCE);

                    if(pos!=-1
                     &&text.Length()>pos+TEXT_SOURCE_SIZE)
                    {
                        ni->source=text.SubString(pos+TEXT_SOURCE_SIZE);
                        ni->tags.Add(ni->source);

                        std::cout<<"source: "<<ni->source.c_str()<<std::endl;
                        return;
                    }
                }
            }

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            for(int i=0;i<node->v.element.children.length;i++)
                ParseSourceNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseInnerNode(const GumboNode *node)
        {
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            if(node->v.element.tag==GUMBO_TAG_P
             ||node->v.element.tag==GUMBO_TAG_SPAN)
            {
                UTF8String text=GetSubText(node,false);

                creater->WriteText(text);
            }
            else
            if(node->v.element.tag==GUMBO_TAG_IMG)
            {
                const GumboAttribute *src=GetAttr(node,"src");

                if(src)
                {
                    if(strncmp(src->value,"http",4)==0)
                        creater->WriteImage(src->value);
                    else
                        creater->WriteImage(UTF8String(URL_HTTP_CHAOZHOUDAILY_COM)+src->value);
                }
            }

            for(int i=0;i<node->v.element.children.length;i++)
                ParseInnerNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->v.element.tag!=GUMBO_TAG_DIV)
                return;

            const GumboAttribute *div_class=GetAttr(node,"class");

            if(!div_class)
                return;

            if(strcmp(div_class->value,"detail_message")==0)
            {
                for(int i=0;i<node->v.element.children.length;i++)
                    ParseSourceNode((const GumboNode *)(node->v.element.children.data[i]));
            }
            else
            if(strcmp(div_class->value,"detail_main")==0)
            {
                for(int i=0;i<node->v.element.children.length;i++)
                    ParseInnerNode((const GumboNode *)(node->v.element.children.data[i]));
            }
        }
    };//class NewsPageParse:public HTMLParse

    class ListPageParse:public HTMLParse
    {
        NewsStorage *storage;
        OSString save_path;
        int next_page_num;

    public:

        ListPageParse(NewsStorage *ns,const OSString &sp)
        {
            storage=ns;
            save_path=sp;
            next_page_num=-1;
        }

        void ParseNewsNode(const GumboNode *node)
        {
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            if(node->v.element.tag==GUMBO_TAG_LI)
            {
                const GumboNode *post_time_node=GetSubNode(node,GUMBO_TAG_SPAN);

                if(!post_time_node)return;
                if(!CheckAttr(post_time_node,"class","date"))return;

                const UTF8String post_time_text=GetSubText(post_time_node);

                const GumboNode *link_node=GetSubNode(node,GUMBO_TAG_A);

                if(!link_node)
                    return;

                const GumboAttribute *href=GetAttr(link_node,"href");

                if(!href)return;

                if(storage->CheckSourceLink(href->value))
                    return;

                UTF8String title=GetSubText(link_node);

                title.Trim();

                const UTF8String url=UTF8String(URL_HTTP_CHAOZHOUDAILY_COM)+href->value;

                std::cout<<"date: "<<post_time_text.c_str()<<std::endl;
                std::cout<<"link: "<<url.c_str()<<std::endl;
                std::cout<<"title: "<<title.c_str()<<std::endl;

                NewsPageParse npp(storage,save_path,title,url,post_time_text);

                npp.Parse(url,user_agent);

                return;
            }

            for(int i=0;i<node->v.element.children.length;i++)
                ParseNewsNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->v.element.tag!=GUMBO_TAG_DIV)
                return;

            const GumboAttribute *div_class=GetAttr(node,"class");

            if(!div_class)
                return;

            if(strcmp(div_class->value,"page_box")==0)
            {
                const GumboNode *cur_page=GetSubNode(node,GUMBO_TAG_SPAN);

                if(cur_page
                &&CheckAttr(cur_page,"class","current"))
                {
                    const UTF8String text=GetSubText(cur_page);

                    if(text==u8"首页")
                        next_page_num=2;
                    else
                    if(text==u8"尾页")
                        next_page_num=-1;
                    else
                    {
                        if(text.ToInt(next_page_num))
                            ++next_page_num;
                    }
                }
            }
            else
            if(strcmp(div_class->value,"con_left_main_box")==0)
            {
                for(int i=0;i<node->v.element.children.length;i++)
                    ParseNewsNode((const GumboNode *)(node->v.element.children.data[i]));
            }
        }

        int GetNextPageURL()
        {
            return next_page_num;
        }
    };//class ListPageParse:public HTMLParse
}//namespace

void news_gd_cc(const UTF8String &user_agent,const OSString &save_path)
{
    MakePath(save_path);

    const OSString json_filename=MergeFilename(save_path,"news.json");
    NewsStorage storage;

    storage.Load(json_filename);

    UTF8String main_url="http://chaozhoudaily.com/list/3/";
    UTF8String url;

    int page_num=1;

    do
    {
        url=main_url+UTF8String(page_num);

        std::cout<<"--------parse: "<<url.c_str()<<std::endl;

        ListPageParse fpp(&storage,save_path);

        fpp.Parse(url,user_agent);

        page_num=fpp.GetNextPageURL();
    }while(page_num>1);

    storage.Save(json_filename);
}

const UTF8String InitUserAgent()
{
    FirefoxUserAgentConfig cfg;

    cfg.os              =OS_WindowsAMD64;

    cfg.os_ver.major    =10;
    cfg.os_ver.minor    =0;

    cfg.ff_ver.major    =64;
    cfg.ff_ver.minor    =0;

    return FirefoxUserAgent(cfg);
}

HGL_CONSOLE_MAIN_FUNC()
{
    const UTF8String user_agent=InitUserAgent();

    OSString cur_path;

    GetCurrentPath(cur_path);
    OSString save_doc_path=MergeFilename(cur_path,OS_TEXT("news"));

    std::cout<<std::endl<<"广东潮州"<<std::endl;
    news_gd_cc(user_agent,MergeFilename(save_doc_path,"广东潮州"));

    return 0;
}

