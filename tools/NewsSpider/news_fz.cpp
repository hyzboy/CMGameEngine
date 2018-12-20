#include"HTMLParse.h"
#include"NewsStorage.h"
#include"NewsCreater.h"

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
            ni->index=storage->GetMaxIndex();
            ni->tags.Add(U8_TEXT("抚州"));
            ni->src_link=src_link;

            save_path=MergeFilename(sp,OSString(ni->index));
            MakePath(save_path);

            storage->Add(ni);

            creater=new NewsCreater(save_path);
        }

        ~NewsPageParse()
        {
            ni->first_image =creater->first_image;
            ni->first_line  =creater->first_line;
            ni->img_count   =creater->img_count;

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

                ni->post_time=text.SubString(pos+SPAR_CHAR_SIZE,16);

                std::cout<<"post_time: "<<ni->post_time.c_str()<<std::endl;

                pos=text.FindString(SPAR_CHAR,pos+SPAR_CHAR_SIZE+16);

                ni->source=text.SubString(pos+SPAR_CHAR_SIZE);

                ni->source.Trim();

                std::cout<<"post source: "<<ni->source.c_str()<<std::endl;
            }
        }

        void ParseInnerNode(const GumboNode *node)
        {
            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            if(node->v.element.tag==GUMBO_TAG_P)
            {
                const UTF8String text=GetSubText(node);

                if(text.Length()<=0)
                {
                }

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

