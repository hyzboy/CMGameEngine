#include"NewsStorage.h"
#include<hgl/ut/JsonTool.h>

void NewsInfo::Make(Json::Value &node)
{
    node["index"]=index;
    node["title"]=title.c_str();

    if(!post_time.IsEmpty())
        node["post_time"]=post_time.c_str();

    if(!source.IsEmpty())
        node["source"]=source.c_str();

    if(!author.IsEmpty())
        node["author"]=author.c_str();

    for(int i=0;i<tags.GetCount();i++)
        node["tags"][i]=tags[i].c_str();

    if(!first_image.IsEmpty())
        node["first_image"]=first_image.c_str();

    node["link"]=link.c_str();
    node["img_count"]=img_count;
}

void NewsStorage::Save(const OSString &filename)
{
    Json::Value root;

    const int count=news_list.GetCount();

    if(count<=0)return;

    NewsInfo **ni=news_list.GetData();

    for(int i=0;i<count;i++)
    {
        (*ni)->Make(root[i]);
        ++ni;
    }

    SaveJson(root,filename);
}

int NewsStorage::Add(NewsInfo *ni)
{
    if(!ni)
        return(-1);

    ++max_index;
    ni->index=max_index;

    news_list.Add(ni);
    return max_index;
}
