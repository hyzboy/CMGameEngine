#include"HTMLParse.h"

namespace
{
    class NewsPageParse:public HTMLParse
    {
        UTF8String title;
        UTF8String post_time;
        UTF8String source;

    public:

        using HTMLParse::HTMLParse;

        void ParseTitle(const GumboNode *node)
        {
            if(!CheckAttr(node,"class","post-title"))return;

            title=GetSubText(node);

            std::cout<<"title: "<<title.c_str()<<std::endl;
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

                post_time=GetSubText(node);

                std::cout<<"post time: "<<post_time.c_str()<<std::endl;
            }

            if(node->v.element.tag==GUMBO_TAG_A)
            {
                if(!CheckAttr(node,"rel","category tag"))return;

                source=GetSubText(node);

                std::cout<<"source: "<<source.c_str()<<std::endl;
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
                    std::cout<<text.c_str()<<std::endl;
            }

            if(node->v.element.tag==GUMBO_TAG_IMG)
            {
                const GumboAttribute *img=GetAttr(node,"src");

                if(img)
                    std::cout<<"img: "<<img->value<<std::endl;
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
    public:

        using HTMLParse::HTMLParse;

        void ParseNewsNode(const GumboNode *node)
        {
            if(node->v.element.tag!=GUMBO_TAG_A)return;
            if(node->v.element.attributes.length<=2)return;

            const GumboAttribute *href=GetAttr(node,"href");
            const GumboAttribute *title=GetAttr(node,"title");

            std::cout<<"link: "<<href->value<<std::endl;
            std::cout<<"title: "<<title->value<<std::endl;

            NewsPageParse npp;

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

void news_hrb(const UTF8String &user_agent)
{
    ListPageParse fpp;

    const UTF8String url="http://www.xjchuju.com/list-1.html";

    fpp.Parse(url,user_agent);
}
