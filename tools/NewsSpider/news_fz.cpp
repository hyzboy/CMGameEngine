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
            ni->tags.Add(U8_TEXT("抚州"));
            ni->src_link=src_link;

            save_path=MergeFilename(sp,OSString(ni->index));
            MakePath(save_path);

            creater=new NewsCreater(save_path);
        }

        ~NewsPageParse()
        {
            if(creater)
            {
                ni->first_image =creater->first_image;
                ni->first_line  =creater->first_line;
                ni->img_count   =creater->img_count;

                ni->index=storage->GetMaxIndex();

                creater->Save();

                storage->Add(ni);
            }
            else
            {
                delete ni;
            }

            delete creater;
            std::cout<<ni->index<<" : "<<ni->title.c_str()<<std::endl;
        }

        void ParseSourceNode(const GumboNode *node)
        {
            if(node->v.element.tag!=GUMBO_TAG_DIV)return;

            const UTF8String text=GetSubText(node);

            if(!text.IsEmpty())
            {
                int pos=text.FindString(SPAR_CHAR);

                if(pos<=0)
                    return;

                ni->post_time=text.SubString(pos+SPAR_CHAR_SIZE,16);

                pos=text.FindString(SPAR_CHAR,pos+SPAR_CHAR_SIZE+16);

                if(pos<=0)
                    return;

                ni->source=text.SubString(pos+SPAR_CHAR_SIZE);
                ni->source.Trim();
            }
        }

        void ParseInnerNode(const GumboNode *node)
        {
            if(!creater)return;

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            if(node->v.element.tag==GUMBO_TAG_P)
            {
                const UTF8String text=GetSubText(node);

                creater->WriteText(text);
            }

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
            if(node->v.element.tag==GUMBO_TAG_DIV)
            {
                const GumboAttribute *id=GetAttr(node,"id");

                if(!id)
                    return;

                if(strcmp(id->value,"w-text-aabh")==0)
                {
                    for(int i=0;i<node->v.element.children.length;i++)
                        ParseSourceNode((const GumboNode *)(node->v.element.children.data[i]));
                }

                if(strcmp(id->value,"w-text-aabi")==0)
                {
                    for(int i=0;i<node->v.element.children.length;i++)
                        ParseInnerNode((const GumboNode *)(node->v.element.children.data[i]));
                }
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
            if(node->v.element.tag!=GUMBO_TAG_A)return;
            if(node->v.element.attributes.length<=1)return;

            const GumboAttribute *href=GetAttr(node,"href");

            if(!href)return;

            if(storage->CheckSourceLink(href->value))
                return;

            const UTF8String title=GetSubText(node);

            std::cout<<"link: "<<href->value<<std::endl;
            std::cout<<"title: "<<title.c_str()<<std::endl;

            NewsPageParse npp(storage,save_path,title,href->value);

            npp.Parse(href->value,user_agent);
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

            if(node->v.element.tag!=GUMBO_TAG_DIV)return;
            if(node->v.element.children.length<=0)return;
            if(!CheckAttr(node,"id","w-text-aabf"))return;

            for(int i=0;i<node->v.element.children.length;i++)
                ParseNewsNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        bool GetNextPageURL(UTF8String &url)
        {
            url=next_page_url;
        }
    };//class ListPageParse:public HTMLParse
}//namespace

void news_fz(const UTF8String &user_agent,const OSString &save_path)
{
    MakePath(save_path);

    const OSString json_filename=MergeFilename(save_path,"news.json");
    NewsStorage storage;

    storage.Load(json_filename);

    UTF8String url="http://www.zgfznews.com/shehui/";

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

    std::cout<<std::endl<<"抚州"<<std::endl;
    news_fz(user_agent,MergeFilename(save_doc_path,"fz"));

    return 0;
}

