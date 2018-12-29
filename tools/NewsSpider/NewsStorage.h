#pragma once

#include<hgl/type/StringList.h>
#include<hgl/type/List.h>
#include<json/json.h>

using namespace hgl;

struct NewsInfo
{
    int index=0;

    UTF8String title;
    UTF8String post_time;
    UTF8String source;
    UTF8String author;
    UTF8StringList tags;
    UTF8String first_image;
    UTF8String first_line;

    int img_count=0;

    UTF8String src_link;

public:

    void Make(Json::Value &);
    void Parse(const Json::Value &);
};

class NewsStorage
{
    ObjectList<NewsInfo> news_list;

    int max_index=0;

public:

    void Load(const OSString &);
    void Save(const OSString &);

    bool CheckSourceLink(const UTF8String &);

    bool Add(NewsInfo *);

    const int GetMaxIndex(){return ++max_index;}
};//class NewsStorage
