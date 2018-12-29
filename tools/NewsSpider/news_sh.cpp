#include"HTMLParse.h"
#include"NewsStorage.h"
#include"NewsCreater.h"

#include<hgl/Console.h>
#include<hgl/webapi/UserAgentString.h>

using namespace hgl::webapi;

namespace
{
    constexpr char HTTP_NEWS_ONLINE_SH_CN[]="https://news.online.sh.cn/news/gb/node/";
    constexpr char SPAR_CHAR[]=u8"：";
    constexpr uint SPAR_CHAR_SIZE=sizeof(SPAR_CHAR)-1;

    class NewsPageParse:public HTMLParse
    {
        NewsStorage *storage;
        OSString save_path;

        NewsInfo *ni;

        NewsCreater *creater;

    public:

        NewsPageParse(NewsStorage *ns,const OSString &sp,const UTF8String &title,const UTF8String &src_link)
        {
            storage=ns;

            ni=new NewsInfo;
            ni->title=title;
            ni->index=storage->GetMaxIndex();
            ni->tags.Add(U8_TEXT("上海"));
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

        void ParseTimeNode(const GumboNode *node)
        {
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            const GumboNode *year       =GetSubNode(node,GUMBO_TAG_DIV,"class","text_box");
            const GumboNode *month_day  =GetSubNode(node,GUMBO_TAG_H3,"class","fb_time_1");
            const GumboNode *hour_min   =GetSubNode(node,GUMBO_TAG_H5,"class","fb_time_2");

            if(year&&month_day&&hour_min)
            {
                UTF8String y=GetSubText(year);
                UTF8String md=GetSubText(month_day);
                UTF8String hm=GetSubText(hour_min);

                y.Trim();
                md.Trim();
                hm.Trim();

                ni->post_time=y+"/"+md+" "+hm;
                return;
            }
        }

        void ParseInnerNode(const GumboNode *node)
        {
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            if(node->v.element.tag==GUMBO_TAG_TABLE)return;

            if(node->v.element.tag==GUMBO_TAG_P)
            {
                const UTF8String text=GetSubText(node);

                creater->WriteText(text);
            }

            if(node->v.element.tag==GUMBO_TAG_IMG)
            {
                const GumboAttribute *src=GetAttr(node,"src");

                if(src)
                {
                    if(strncmp(src->value,"http",4)==0)
                        creater->WriteImage(src->value);
                    else
                        creater->WriteImage(UTF8String(HTTP_NEWS_ONLINE_SH_CN)+UTF8String(src->value));
                }
            }

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            for(int i=0;i<node->v.element.children.length;i++)
                ParseInnerNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->v.element.tag!=GUMBO_TAG_DIV)return;

            const GumboAttribute *div_class=GetAttr(node,"class");

            if(!div_class)
                return;

            if(strcmp(div_class->value,"share_box")==0)
            {
                ParseTimeNode(node);
            }
            else
            if(strcmp(div_class->value,"post_text")==0)
            {
                for(int i=0;i<node->v.element.children.length;i++)
                    ParseInnerNode((const GumboNode *)(node->v.element.children.data[i]));
            }
        }
    };

    class ListPageParse:public HTMLParse
    {
        NewsStorage *storage;
        OSString save_path;
        UTF8String next_page_url;

    public:

        ListPageParse(NewsStorage *ns,const OSString &sp)
        {
            storage=ns;
            save_path=sp;
        }

        void ParseNewsNode(const GumboNode *node)
        {
            if(node->v.element.tag==GUMBO_TAG_LI)
            {
                const GumboNode *a_node=GetSubNode(node,GUMBO_TAG_A);

                if(a_node)
                {
                    const GumboAttribute *href=GetAttr(a_node,"href");

                    if(!href)return;

                    const UTF8String title=GetSubText(a_node);

                    UTF8String url;

                    if(strncmp(href->value,"http",4)==0)
                    {
                        if(strncmp(href->value,HTTP_NEWS_ONLINE_SH_CN,sizeof(HTTP_NEWS_ONLINE_SH_CN)-1))
                            return;

                        url=href->value;
                    }
                    else
                        url=UTF8String(HTTP_NEWS_ONLINE_SH_CN)+href->value;

                    if(storage->CheckSourceLink(url))
                        return;

                    std::cout<<"link: "<<url.c_str()<<std::endl;
                    std::cout<<"title: "<<title.c_str()<<std::endl;

                    NewsPageParse npp(storage,save_path,title,url);

                    npp.Parse(url,user_agent);
                }
            }

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            for(int i=0;i<node->v.element.children.length;i++)
                ParseNewsNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParsePageNum(const GumboNode *node)
        {
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            if(node->v.element.tag==GUMBO_TAG_A)
            {
                const UTF8String txt=GetSubText(node,false);

                if(txt==u8"下一页")
                {
                    const GumboAttribute *href=GetAttr(node,"href");

                    next_page_url=href->value;
                    return;
                }
            }

            for(int i=0;i<node->v.element.children.length;i++)
                ParsePageNum((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点
            if(node->v.element.tag!=GUMBO_TAG_DIV)return;

            const GumboAttribute *div_id=GetAttr(node,"id");

            if(!div_id)return;

            if(strcmp(div_id->value,"displaypagenum")==0)
            {
                ParsePageNum(node);
            }
            else
            if(strcmp(div_id->value,"list")==0)
            {
                for(int i=0;i<node->v.element.children.length;i++)
                    ParseNewsNode((const GumboNode *)(node->v.element.children.data[i]));
            }
        }

        bool GetNextPageURL(UTF8String &url)
        {
            url=next_page_url;
        }
    };//class ListPageParse:public HTMLParse
}//namespace

void news_sh(const UTF8String &user_agent,const OSString &save_path)
{
    MakePath(save_path);

    const OSString json_filename=MergeFilename(save_path,"news.json");
    NewsStorage storage;

    storage.Load(json_filename);

    UTF8String url;
    UTF8String np_url="shanghai.htm";

    do
    {
        url="https://news.online.sh.cn/news/gb/node/"+np_url;
        std::cout<<"--------parse: "<<url.c_str()<<std::endl;

        ListPageParse fpp(&storage,save_path);

        fpp.Parse(url,user_agent);

        fpp.GetNextPageURL(np_url);

    }while(!np_url.IsEmpty());

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

    std::cout<<std::endl<<"上海"<<std::endl;
    news_sh(user_agent,MergeFilename(save_doc_path,u8"上海"));

    return 0;
}
