#include"HTMLParse.h"
#include"NewsStorage.h"
#include"NewsCreater.h"

#include<hgl/Console.h>
#include<hgl/webapi/UserAgentString.h>

using namespace hgl::webapi;

namespace
{
    constexpr char HTTPS_CD_SCOL_COM_CN[]="https://cd.scol.com.cn";
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
            ni->tags.Add(U8_TEXT("四州"));
            ni->tags.Add(U8_TEXT("成都"));
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
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点
            if(node->v.element.tag==GUMBO_TAG_A)
            {
                const UTF8String text=GetSubText(node);

                ni->source=text;
                ni->source.Trim();

                std::cout<<"source: "<<text.c_str()<<std::endl;

                if(ni->source.Length()>0)
                    ni->tags.Add(ni->source);
            }

            for(int i=0;i<node->v.element.children.length;i++)
                ParseSourceNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseTimeSourceNode(const GumboNode *node)
        {
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点
            if(node->v.element.tag==GUMBO_TAG_SPAN)
            {
                const GumboAttribute *id=GetAttr(node,"id");

                if(!id)
                    return;

                if(strcmp(id->value,"pubtime_baidu")==0)
                {
                    const UTF8String text=GetSubText(node);

                    ni->post_time=text;
                    std::cout<<"post time: "<<text.c_str()<<std::endl;
                }
                else
                if(strcmp(id->value,"source_baidu")==0)
                {
                    ParseSourceNode(node);
                    return;
                }
            }

            for(int i=0;i<node->v.element.children.length;i++)
                ParseTimeSourceNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseInnerNode(const GumboNode *node)
        {
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
                {
                    if(strncmp(src->value,"http",4)==0)
                        creater->WriteImage(src->value);
                    else
                        creater->WriteImage(UTF8String(HTTPS_CD_SCOL_COM_CN)+UTF8String(src->value));
                }
            }

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            for(int i=0;i<node->v.element.children.length;i++)
                ParseInnerNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->v.element.tag==GUMBO_TAG_SPAN)
            {
                if(CheckAttr(node,"id","editor_baidu"))
                {
                    const UTF8String txt=GetSubText(node);

                    if(txt.Length()>0)
                    {
                        ni->author=txt;

                        std::cout<<"author: "<<txt.c_str()<<std::endl;

                        ni->tags.Add(txt);
                    }
                }
            }

            if(node->v.element.tag==GUMBO_TAG_DIV)
            {
                const GumboAttribute *id=GetAttr(node,"id");

                if(!id)
                    return;

                if(strcmp(id->value,"scol_time")==0)
                {
                    for(int i=0;i<node->v.element.children.length;i++)
                        ParseTimeSourceNode((const GumboNode *)(node->v.element.children.data[i]));
                }

                if(strcmp(id->value,"scol_txt")==0)
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
            if(node->v.element.tag==GUMBO_TAG_A)
            {
                if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

                const GumboAttribute *href=GetAttr(node,"href");

                if(!href)return;

                const UTF8String title=GetSubText(node);

                UTF8String url=UTF8String(HTTPS_CD_SCOL_COM_CN)+href->value;

                if(storage->CheckSourceLink(url))
                    return;

                std::cout<<"link: "<<url.c_str()<<std::endl;
                std::cout<<"title: "<<title.c_str()<<std::endl;

                NewsPageParse npp(storage,save_path,title,url);

                npp.Parse(url,user_agent);
            }

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            for(int i=0;i<node->v.element.children.length;i++)
                ParseNewsNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->v.element.tag==GUMBO_TAG_A)
            {
                const UTF8String txt=GetSubText(node);

                if(txt==u8"下一页")
                {
                    const GumboAttribute *href=GetAttr(node,"href");

                    next_page_url=UTF8String(href->value);
                }
            }

            if(node->v.element.tag!=GUMBO_TAG_DIV)return;
            if(node->v.element.children.length<=0)return;
            if(!CheckAttr(node,"id","txtlist"))return;

            for(int i=0;i<node->v.element.children.length;i++)
                ParseNewsNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        bool GetNextPageURL(UTF8String &url)
        {
            url=next_page_url;
        }
    };//class ListPageParse:public HTMLParse
}//namespace

void news_sc_cd(const UTF8String &user_agent,const OSString &save_path,const UTF8String &sub_page)
{
    MakePath(save_path);

    const OSString json_filename=MergeFilename(save_path,"news.json");
    NewsStorage storage;

    storage.Load(json_filename);

    UTF8String main_url="https://cd.scol.com.cn/"+sub_page+"/";
    UTF8String url=main_url;
    UTF8String np_url;

    do
    {
        std::cout<<"--------parse: "<<url.c_str()<<std::endl;

        ListPageParse fpp(&storage,save_path);

        fpp.Parse(url,user_agent);

        fpp.GetNextPageURL(np_url);

        url=main_url+np_url;
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

    std::cout<<std::endl<<"四川成都-民生"<<std::endl;
    news_sc_cd(user_agent,MergeFilename(save_doc_path,u8"四川成都-民生"),"ms");

    std::cout<<std::endl<<"四川成都-要闻"<<std::endl;
    news_sc_cd(user_agent,MergeFilename(save_doc_path,u8"四川成都-要闻"),"cdyw");

    std::cout<<std::endl<<"四川成都-城事"<<std::endl;
    news_sc_cd(user_agent,MergeFilename(save_doc_path,u8"四川成都-民生"),"cs1");

    return 0;
}


