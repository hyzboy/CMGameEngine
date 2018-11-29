#include<hgl/Console.h>
#include<hgl/network/HTTPTools.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/LogInfo.h>
#include<hgl/type/StringList.h>

#include<gumbo-parser/gumbo.h>      //google html5 解晰库

#include<iostream>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::filesystem;

UTF8String save_doc_path;
UTF8String save_img_path;

class ActiveURLList
{
    UTF8StringList fin_url_list;        //完成转换的新闻网页
    UTF8StringList url_list;            //待转换的新闻网页

public:

    void Add(const UTF8String &url)
    {
        if(fin_url_list.Find(url)!=-1)return;
        if(url_list.Find(url)!=-1)return;

        url_list.Add(url);
    }

    bool Get(UTF8String &url)
    {
        const int count=url_list.GetCount();

        if(count<=0)return(false);

        url=url_list.GetString(count-1);

        url_list.Delete(count-1);

        fin_url_list.Add(url);

        return(true);
    }

    const uint GetCount()const
    {
        return url_list.GetCount();
    }
};//

ActiveURLList list_page;
ActiveURLList news_page;

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

bool CheckAttr(const GumboNode *node,const char *name,const char *value)
{
    for(int i=0;i<node->v.element.attributes.length;i++)
    {
        const GumboAttribute *attr=(const GumboAttribute *)(node->v.element.attributes.data[i]);

        if(strcmp(attr->name,name)==0)
            return(!strcmp(attr->value,value));
    }

    return false;
}

constexpr char HTTP_NEWS_SINA_COM_CN_HEADER[]="http://news.sina.com.cn/";
constexpr uint HTTP_NEWS_SINA_COM_CN_HEADER_SIZE=sizeof(HTTP_NEWS_SINA_COM_CN_HEADER)-1;
constexpr char HTTPS_NEWS_SINA_COM_CN_HEADER[]="https://news.sina.com.cn/";
constexpr uint HTTPS_NEWS_SINA_COM_CN_HEADER_SIZE=sizeof(HTTPS_NEWS_SINA_COM_CN_HEADER)-1;
constexpr char NEWS_LINK_END[]=".shtml";
constexpr uint NEWS_LINK_END_SIZE=sizeof(NEWS_LINK_END)-1;

constexpr char TAG_NEWS_SINA_COM_CN_HEADER[]="http://tags.news.sina.com.cn/";
constexpr uint TAG_NEWS_SINA_COM_CN_HEADER_SIZE=sizeof(TAG_NEWS_SINA_COM_CN_HEADER)-1;

constexpr char CHARSET_ATTR[]="charset=";
constexpr uint CHARSET_ATTR_SIZE=sizeof(CHARSET_ATTR)-1;

class HTMLParse
{
private:

    UTF8String web_url;
    MemoryOutputStream mos;

    CharSet page_charset;

protected:

    const UTF8String GetSubText(const GumboNode *node,int index)
    {
        if(index<0||index>=node->v.element.children.length)
            return(nullptr);

        const GumboNode *sub_node=(const GumboNode *)(node->v.element.children.data[index]);

        if(sub_node->type!=GUMBO_NODE_TEXT)
            return(nullptr);

         if(page_charset.codepage==ccpUTF8)
            return UTF8String(sub_node->v.text.text);
        else
        {
            char *u8str;

            int len=to_utf8(page_charset,
                            &u8str,
                            sub_node->v.text.original_text.data,
                            sub_node->v.text.original_text.length);

            return UTF8String(u8str,len,true);
        }
    }

    bool CheckNewsLinkNode(const GumboNode *node)
    {
        if(node->type!=GUMBO_NODE_ELEMENT)return(false);       //没有子节点
        if(node->v.element.tag!=GUMBO_TAG_A)return(false);     //不是A节点
        if(node->v.element.attributes.length<2)return(false);  //属性太少
        if(node->v.element.children.length!=1)return(false);   //子节点数量不对

        if(!CheckAttr(node,"target","_blank"))return(false);

        const GumboAttribute *href=GetAttr(node,"href");

        if(!href)return(false);

        if(strncmp(href->value,HTTP_NEWS_SINA_COM_CN_HEADER,HTTP_NEWS_SINA_COM_CN_HEADER_SIZE)
            &&strncmp(href->value,HTTPS_NEWS_SINA_COM_CN_HEADER,HTTPS_NEWS_SINA_COM_CN_HEADER_SIZE))return(false);

        const char *end_href=href->value+strlen(href->value)-NEWS_LINK_END_SIZE;

        if(strcmp(end_href,NEWS_LINK_END))
        {
            std::cout<<"list page: "<<href->value<<std::endl;
            list_page.Add(href->value);
            return(false);
        }

        const char *text=GetSubText(node,0);

        if(!text)return(false);

        std::cout<<"news: "<<text<<std::endl;
        std::cout<<"link: "<<href->value<<std::endl;

        news_page.Add(href->value);
        return(true);
    }

public:

    HTMLParse()
    {
        page_charset=CharSet("utf-8");
    }

    virtual ~HTMLParse()=default;

    virtual void ParseNode(const GumboNode *node)=0;

    virtual void Parse(const GumboNode *node)
    {
        if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

        if(node->v.element.tag==GUMBO_TAG_META)
        {
            const GumboAttribute *content=GetAttr(node,"content");

            if(!content)return;
            const char *pos=strstr(content->value,CHARSET_ATTR);

            if(pos)
                page_charset=CharSet(pos+CHARSET_ATTR_SIZE);

            return;
        }

        ParseNode(node);

        for(int i=0;i<node->v.element.children.length;i++)
            Parse((const GumboNode *)(node->v.element.children.data[i]));
    }
public:

    void Parse(const char *url)
    {
        web_url=url;
        network::http::get(&mos,url);

        GumboOutput *output=gumbo_parse_with_options(&kGumboDefaultOptions,(const char *)mos.GetData(),mos.Tell());

        Parse(output->root);

        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
};//class HTMLParse


constexpr os_char IMAGE_SUBPATH[]=OS_TEXT("img");

/**
 * 正文解晰
 */
class SinaNewsArticleParse:public HTMLParse
{
    TextOutputStream *tos;

    int art_count;
    int img_count;

public:

    SinaNewsArticleParse(TextOutputStream *os,int index):HTMLParse()
    {
        tos=os;

        art_count=index;
        img_count=0;
    }

    void ParseNode(const GumboNode *node) override
    {
        if(CheckNewsLinkNode(node))return;              //是一个新的新闻节点

        if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

        if(node->v.element.tag==GUMBO_TAG_P)
        {
            if(CheckAttr(node,"class","show_author"))
            {
                const char *author=GetSubText(node,0);

                if(!author)return;

                tos->WriteLine(UTF8String("<p><b>Author:</b>")+author+UTF8String("</p>"));
            }
            else    //普通正文
            {
                const char *text=GetSubText(node,0);

                if(!text)return;

                if(isSameCharString(text,' '))
                {
                    //tos->WriteLine(UTF8String("<p/>"));
                }
                else
                {
                    UTF16String u16_text=to_u16(text);

                    if(isSameCharString(u16_text.c_str(),u'　'))
                    {
                        //tos->WriteLine(UTF8String("<p/>"));
                    }
                    else
                    {
                        tos->WriteLine(UTF8String("<p>")+text+UTF8String("</p>"));
                    }
                }
            }
        }
        else
        if(node->v.element.tag==GUMBO_TAG_STRONG)
        {
            const char *text=GetSubText(node,0);

            if(!text)return;

            tos->WriteLine(UTF8String("<strong>")+text+UTF8String("</strong>"));
        }
        else
        if(node->v.element.tag==GUMBO_TAG_IMG)
        {
            const GumboAttribute *src=GetAttr(node,"src");

            if(!src)return;

            FileOutputStream fos;

            const UTF8String ext_name=UTF8String(".")+ClipFileExtName(UTF8String(src->value));

            const UTF8String img_filename=UTF8String(art_count)+U8_TEXT("_")+UTF8String(img_count++)+ext_name;

            if(fos.CreateTrunc(MergeFilename(save_img_path,ToOSString(img_filename))))
            {
                std::cout<<"download image: "<<src->value<<std::endl;

                network::http::get(&fos,U8_TEXT("https:")+UTF8String(src->value));

                fos.Close();
            }

            tos->WriteLine(UTF8String("<p><img src=\"")+MergeFilename(IMAGE_SUBPATH,img_filename)+UTF8String("\"></p>"));
        }
    }
};//

/**
 * 新闻页解晰
 */
class SinaNewsPageParse:public HTMLParse
{
    MemoryOutputStream mos;
    TextOutputStream *tos;

    OSString filename;
    int news_index;

    UTF8String main_title;

public:

    SinaNewsPageParse(const int index):HTMLParse()
    {
        news_index=index;

        tos=new UTF8TextOutputStream(&mos);

        filename=MergeFilename(save_doc_path,OSString(index)+OS_TEXT(".html"));

        tos->WriteLine(UTF8String("<html><head><meta charset=\"utf-8\"/></head>"));
    }

    ~SinaNewsPageParse()
    {
        tos->WriteLine(UTF8String("</html>"));
        SaveMemoryToFile(filename,mos.GetData(),mos.Tell());
        delete tos;
    }

    const UTF8String &GetMainTitle()const{return main_title;}

    void ParseNode(const GumboNode *node) override
    {
        if(node->type!=GUMBO_NODE_ELEMENT)return;       //没有子节点

        if(node->v.element.tag==GUMBO_TAG_H1)
        {
            if(!CheckAttr(node,"class","main-title"))return;

            const char *text=GetSubText(node,0);

            if(!text)return;

            main_title=text;

            tos->WriteLine(UTF8String("<p><h1>")+main_title+UTF8String("</h1></p>"));
        }
        else
        if(node->v.element.tag==GUMBO_TAG_DIV)
        {
            if(CheckAttr(node,"id","article")           //正文
             &&CheckAttr(node,"class","article"))
            {
                SinaNewsArticleParse ap(tos,news_index);

                ap.Parse(node);
            }
        }
        else
        if(node->v.element.tag==GUMBO_TAG_A)
        {
            const GumboAttribute *href=GetAttr(node,"href");

            if(!href)return;
            if(strncmp(href->value,TAG_NEWS_SINA_COM_CN_HEADER,TAG_NEWS_SINA_COM_CN_HEADER_SIZE))return;

            const char *text=GetSubText(node,0);

            tos->WriteLine(UTF8String("<p><b>Tag:</b>")+text+UTF8String("</p>"));
        }
    }
};//class SinaNewsPageParse:public HTMLParse

/**
 * 页内新闻链接查找
 * 范例: <a target="_blank" href="https://news.sina.com.cn/c/xl/2018-11-28/doc-ihpevhcm2060930.shtml">习近平出席西班牙国王举行的盛大欢迎仪式</a>
 * 判断依据: 为A点节，至少有两属性target,href，只有一个TEXT类型子节点
 *          target为"_blank"
 *          href前缀为"https://news.sina.com.cn"
 *          href结尾为".shtml"
 */
class SinaNewsFirstPageParse:public HTMLParse
{
public:

    void ParseNode(const GumboNode *node) override
    {
        CheckNewsLinkNode(node);
    }
};//

void MakeSavePath()
{
    {
        OSString cur_path;

        GetCurrentPath(cur_path);

        save_doc_path=MergeFilename(cur_path,OS_TEXT("sina_news"));
    }

    MakePath(save_doc_path);

    save_img_path=MergeFilename(save_doc_path,IMAGE_SUBPATH);

    MakePath(save_img_path);
}

HGL_CONSOLE_MAIN_FUNC()
{
    sii.info.ProjectName=U8_TEXT("新闻爬虫");
    sii.info.ProjectCode=OS_TEXT("NewsSpider");

    if(!app.Init(&sii))
        return(-1);

    MakeSavePath();

    MemoryOutputStream mos;
    TextOutputStream *tos;

    tos=new UTF8TextOutputStream(&mos);

    tos->WriteLine(UTF8String("<html><head><meta charset=\"utf-8\"/></head><ul>"));

    list_page.Add("https://news.sina.com.cn");

    int news_count=0;

    do
    {
        UTF8String url;

        while(list_page.Get(url))
        {
            SinaNewsFirstPageParse sina;

            std::cout<<"parse page: "<<url.c_str()<<std::endl;

            sina.Parse(url);
        }

        while(news_page.Get(url))
        {
            std::cout<<"parse news: "<<url.c_str()<<std::endl;

            SinaNewsPageParse news_page(news_count);

            news_page.Parse(url);

            tos->WriteLine(UTF8String("<li><a href=\"")+UTF8String(news_count)+UTF8String(".html\">")+news_page.GetMainTitle()+UTF8String("</a></li>"));

            ++news_count;
        }
    }while(list_page.GetCount()+news_page.GetCount());

    tos->WriteLine(UTF8String("</ul></html>"));
    SaveMemoryToFile(MergeFilename(save_doc_path,OS_TEXT("index.html")),mos.GetData(),mos.Tell());
    delete tos;
    return 0;
}
