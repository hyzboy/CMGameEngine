#include"HTMLParse.h"
#include"NewsStorage.h"
#include"NewsCreater.h"

#include<hgl/Console.h>
#include<hgl/webapi/UserAgentString.h>

using namespace hgl::webapi;

namespace
{
    constexpr char HTTP_HB_NEWS_COM[]="http://www.hbneww.com";
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
            ni->tags.Add(U8_TEXT("湖北"));
            ni->tags.Add(U8_TEXT("宜昌"));
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
            if(node->v.element.tag==GUMBO_TAG_SPAN)
            {
                const UTF8String text=GetSubText(node,false);

                if(text.Length()==10
                 &&strncmp(text.c_str(),u8"20",2)==0)      //日期都是20开头
                {
                    ni->post_time=text;

                    std::cout<<"post time: "<<text.c_str()<<std::endl;

                    if(ni->post_time.Length()>0)
                        ni->tags.Add(ni->post_time);
                }

                return;
            }

            for(int i=0;i<node->v.element.children.length;i++)
                ParseSourceNode((const GumboNode *)(node->v.element.children.data[i]));
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
                        creater->WriteImage(UTF8String(HTTP_HB_NEWS_COM)+UTF8String(src->value));
                }
            }

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            for(int i=0;i<node->v.element.children.length;i++)
                ParseInnerNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->v.element.tag!=GUMBO_TAG_DIV)return;

            const GumboAttribute *id=GetAttr(node,"class");

            if(!id)
                return;

            if(strcmp(id->value,"edit")==0)
            {
                for(int i=0;i<node->v.element.children.length;i++)
                    ParseSourceNode((const GumboNode *)(node->v.element.children.data[i]));
            }

            if(strcmp(id->value,"detials")==0)
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
            if(node->v.element.tag==GUMBO_TAG_A)
            {
                if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点
                if(!CheckAttr(node,"target","_blank"))return;

                const GumboAttribute *href=GetAttr(node,"href");

                if(!href)return;

                const UTF8String title=GetSubText(node);

                UTF8String url=UTF8String(HTTP_HB_NEWS_COM)+href->value;

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
                const UTF8String txt=GetSubText(node,false);

                if(txt==u8"下一页")
                {
                    const GumboAttribute *href=GetAttr(node,"href");

                    next_page_url=UTF8String(href->value);
                }
            }

            if(node->v.element.tag!=GUMBO_TAG_DIV)return;
            if(node->v.element.children.length<=0)return;
            if(!CheckAttr(node,"class","sonL newest"))return;

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

    UTF8String main_url="http://www.hbneww.com/hbnews/"+sub_page+"/";
    UTF8String url=main_url;
    UTF8String np_url;

    do
    {
        url=main_url+np_url;

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

    std::cout<<std::endl<<"湖北-宜昌"<<std::endl;
    news_sc_cd(user_agent,MergeFilename(save_doc_path,u8"湖北-宜昌"),"ych");

    return 0;
}



