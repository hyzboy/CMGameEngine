#include"HTMLParse.h"
#include"NewsStorage.h"

namespace
{
    class NewsPageParse:public HTMLParse
    {
        NewsStorage *storage;
        OSString save_path;

        NewsInfo *ni;

        MemoryOutputStream mos;
        TextOutputStream *tos;
        OSString filename;

    public:

        NewsPageParse(NewsStorage *ns,const OSString &sp,const UTF8String &src_link)
        {
            storage=ns;

            ni=new NewsInfo;
            ni->index=storage->GetMaxIndex();
            ni->tags.Add(U8_TEXT("哈尔滨"));
            ni->src_link=src_link;

            save_path=MergeFilename(sp,OSString(ni->index));
            MakePath(save_path);

            storage->Add(ni);

            tos=new UTF8TextOutputStream(&mos);

            filename=MergeFilename(save_path,OS_TEXT("index.html"));

            tos->WriteLine(UTF8String("<html><head><meta charset=\"utf-8\"/></head>"));
        }

        ~NewsPageParse()
        {
            tos->WriteLine(UTF8String("</html>"));
            SaveMemoryToFile(filename,mos.GetData(),mos.Tell());
            delete tos;

            std::cout<<ni->index<<" : "<<ni->title.c_str()<<std::endl;
        }

        void ParseTitle(const GumboNode *node)
        {
            if(!CheckAttr(node,"class","post-title"))return;

            ni->title=GetSubText(node);
        }

        void ParseHeaderNode(const GumboNode *node)
        {
            if(node->v.element.tag==GUMBO_TAG_H1)
            {
                ParseTitle(node);
                return;
            }

            if(node->v.element.tag==GUMBO_TAG_SPAN)
            {
                if(!CheckAttr(node,"class","time"))return;

                ni->post_time=GetSubText(node);
            }

            if(node->v.element.tag==GUMBO_TAG_A)
            {
                if(!CheckAttr(node,"rel","category tag"))return;

                ni->source=GetSubText(node);

                return;
            }

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            for(int i=0;i<node->v.element.children.length;i++)
                ParseHeaderNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseInnerNode(const GumboNode *node)
        {
            if(node->v.element.tag==GUMBO_TAG_P)
            {
                const UTF8String text=GetSubText(node);

                if(text.Length()>0)
                {
                    tos->WriteLine(U8_TEXT("<p>")+text+U8_TEXT("</p>"));

                    if(ni->first_line.IsEmpty())
                        ni->first_line=text;
                }
            }

            if(node->v.element.tag==GUMBO_TAG_IMG)
            {
                const GumboAttribute *src=GetAttr(node,"src");

                if(src)
                {
                    FileOutputStream fos;

                    const UTF8String ext_name=UTF8String(".")+ClipFileExtName(UTF8String(src->value));

                    const UTF8String img_filename=UTF8String(ni->img_count)+ext_name;

                    if(fos.CreateTrunc(MergeFilename(save_path,ToOSString(img_filename))))
                    {
                        std::cout<<"download image: "<<src->value<<std::endl;

                        network::http::get(&fos,UTF8String(src->value));

                        fos.Close();
                    }

                    tos->WriteLine(UTF8String("<p><img src=\"")+img_filename+UTF8String("\"></p>"));

                    if(ni->first_image.IsEmpty())
                    {
                        ni->first_image=img_filename;
                    }

                    ++ni->img_count;
                }
            }

            if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

            for(int i=0;i<node->v.element.children.length;i++)
                ParseInnerNode((const GumboNode *)(node->v.element.children.data[i]));
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->v.element.tag==GUMBO_TAG_HEADER
             &&CheckAttr(node,"class","entry-header"))
            {
                for(int i=0;i<node->v.element.children.length;i++)
                    ParseHeaderNode((const GumboNode *)(node->v.element.children.data[i]));
            }
            else
            if(node->v.element.tag==GUMBO_TAG_DIV
             &&CheckAttr(node,"class","entry-inner"))
            {
                for(int i=0;i<node->v.element.children.length;i++)
                    ParseInnerNode((const GumboNode *)(node->v.element.children.data[i]));
            }
        }
    };//

    class ListPageParse:public HTMLParse
    {
        NewsStorage *storage;
        OSString save_path;

    public:

        ListPageParse(NewsStorage *ns,const OSString &sp)
        {
            storage=ns;
            save_path=sp;
        }

        void ParseNewsNode(const GumboNode *node)
        {
            if(node->v.element.tag!=GUMBO_TAG_A)return;
            if(node->v.element.attributes.length<=2)return;

            const GumboAttribute *href=GetAttr(node,"href");
            const GumboAttribute *title=GetAttr(node,"title");

            if(storage->CheckSourceLink(href->value))
                return;

            std::cout<<"link: "<<href->value<<std::endl;
            std::cout<<"title: "<<title->value<<std::endl;

            NewsPageParse npp(storage,save_path,href->value);

            npp.Parse(href->value,user_agent);
        }

        void ParseNode(const GumboNode *node) override
        {
            if(node->v.element.tag!=GUMBO_TAG_H3)return;
            if(node->v.element.children.length<=0)return;
            if(!CheckAttr(node,"class","post-title"))return;

            for(int i=0;i<node->v.element.children.length;i++)
                ParseNewsNode((const GumboNode *)(node->v.element.children.data[i]));
        }
    };//class ListPageParse:public HTMLParse
}//namespace

void news_hrb(const UTF8String &user_agent,const OSString &save_path)
{
    MakePath(save_path);

    const OSString json_filename=MergeFilename(save_path,"news.json");
    NewsStorage storage;

    storage.Load(json_filename);

    ListPageParse fpp(&storage,save_path);

    UTF8String url="http://www.xjchuju.com/list-1.html";

    fpp.Parse(url,user_agent);

    for(int i=2;i<=15;i++)
    {
        url="http://www.xjchuju.com/list-1-400-"+UTF8String(i)+".html";

        fpp.Parse(url,user_agent);
    }

    storage.Save(json_filename);
}
