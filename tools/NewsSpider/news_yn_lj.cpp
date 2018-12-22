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

    constexpr char TEXT_AUTHOR[]=u8"作者：";
    constexpr uint TEXT_AUTHOR_SIZE=sizeof(TEXT_AUTHOR)-1;
    constexpr char TEXT_EDITOR[]=u8"编辑：";
    constexpr uint TEXT_EDITOR_SIZE=sizeof(TEXT_EDITOR)-1;

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
            ni->tags.Add(U8_TEXT("江西"));
            ni->tags.Add(U8_TEXT("抚州"));
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
                    if(text.GetBeginChar()=='2')        //日期
                    {
                        ni->post_time=text;
                        std::cout<<"post_time: "<<text.c_str()<<std::endl;
                        return;
                    }
                    else
                    if(text.FindString(TEXT_AUTHOR)!=-1
                     &&text.Length()>TEXT_AUTHOR_SIZE)
                    {
                        ni->author=text;
                        ni->tags.Add(text);

                        std::cout<<"author: "<<text.c_str()<<std::endl;
                        return;
                    }
                    else
                    if(text.FindString(TEXT_EDITOR)!=-1
                     &&text.Length()>TEXT_EDITOR_SIZE)
                    {
                        ni->tags.Add(text);
                        std::cout<<"editor: "<<text.c_str()<<std::endl;
                        return;
                    }
                }
            }
            else
            if(node->v.element.tag==GUMBO_TAG_A)
            {
                UTF8String text=GetSubText(node);

                text.Trim();

                if(!text.IsEmpty())
                {
                    ni->source=text;
                    ni->tags.Add(text);
                    std::cout<<"source: "<<text.c_str()<<std::endl;
                }

                return;
            }

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            for(int i=0;i<node->v.element.children.length;i++)
                ParseSourceNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseInnerNode(const GumboNode *node)
        {
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            if(node->v.element.tag==GUMBO_TAG_P)
            {
                UTF8String text;

                GetSubText(node,text);

                creater->WriteText(text);
            }
            else
            if(node->v.element.tag==GUMBO_TAG_IMG)
            {
                const GumboAttribute *src=GetAttr(node,"src");

                if(src)
                    creater->WriteImage(src->value);
            }

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            for(int i=0;i<node->v.element.children.length;i++)
                ParseInnerNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->v.element.tag==GUMBO_TAG_P
             &&CheckAttr(node,"class","info pr30"))
            {
                 for(int i=0;i<node->v.element.children.length;i++)
                     ParseSourceNode((const GumboNode *)(node->v.element.children.data[i]));
            }
            else
            if(node->v.element.tag==GUMBO_TAG_DIV
            &&CheckAttr(node,"class","newsMainBox"))
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
        UTF8String next_page_url;

    public:

        ListPageParse(NewsStorage *ns,const OSString &sp)
        {
            storage=ns;
            save_path=sp;
        }

        void ParseNewsNode(const GumboNode *node)
        {
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            if(node->v.element.tag==GUMBO_TAG_H3)
            {
                const GumboNode *link_node=GetSubNode(node,GUMBO_TAG_A);

                if(link_node)
                {
                    const GumboAttribute *href=GetAttr(link_node,"href");

                    if(!href)return;

                    if(storage->CheckSourceLink(href->value))
                        return;

                    const UTF8String title=GetSubText(link_node);

                    std::cout<<"link: "<<href->value<<std::endl;
                    std::cout<<"title: "<<title.c_str()<<std::endl;

                    NewsPageParse npp(storage,save_path,title,href->value);

                    npp.Parse(href->value,user_agent);

                    return;
                }
            }

            for(int i=0;i<node->v.element.children.length;i++)
                ParseNewsNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->v.element.tag==GUMBO_TAG_A)
            {
                if(CheckAttr(node,"class","next"))
                {
                    const GumboAttribute *href=GetAttr(node,"href");

                    next_page_url=href->value;
                }
            }

            if(node->v.element.tag!=GUMBO_TAG_UL)return;
            if(!CheckAttr(node,"class","imgTxtBar clearfix imgTxtBar-b"))return;

            for(int i=0;i<node->v.element.children.length;i++)
                ParseNewsNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        bool GetNextPageURL(UTF8String &url)
        {
            url=next_page_url;
        }
    };//class ListPageParse:public HTMLParse
}//namespace

void news_yn_lj(const UTF8String &user_agent,const OSString &save_path)
{
    MakePath(save_path);

    const OSString json_filename=MergeFilename(save_path,"news.json");
    NewsStorage storage;

    storage.Load(json_filename);

    UTF8String url="http://www.lijiang.cn/hjxw/social/";

    do
    {
        std::cout<<"--------parse: "<<url.c_str()<<std::endl;

        ListPageParse fpp(&storage,save_path);

        fpp.Parse(url,user_agent);

        fpp.GetNextPageURL(url);
    }while(!url.IsEmpty());

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

    std::cout<<std::endl<<"云南丽江"<<std::endl;
    news_yn_lj(user_agent,MergeFilename(save_doc_path,"云南丽江"));

    return 0;
}

