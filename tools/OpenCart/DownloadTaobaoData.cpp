#include<hgl/Console.h>
#include<hgl/network/HTTPTools.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/LogInfo.h>

#include<gumbo-parser/gumbo.h>      //google html5 解晰库
#include<json/json.h>               //JSON 解晰库

using namespace hgl;

const char g_page_config[]="\n        \n    g_page_config = ";
const uint g_page_config_size=sizeof(g_page_config)-1;

const char tb_subtitle[]="<p class=\"tb-subtitle\">";
const uint tb_subtitle_size=sizeof(tb_subtitle)-1;

class TaobaoParse
{
    int product_id;             ///<产品ID
    int manufacturer_id;        ///<品牌ID
    int category_id;            ///<分类ID

    UTF8String manufacturer_str;
    UTF8String category_str;

    io::FileOutputStream product_file;
    //io::FileOutputStream category_file;

    io::UTF8TextOutputStream *product_sql;
    //io::TextOutputStream *category_sql;

    int page_count=1;

public:

    void Init(int pid,int mid,int cid)
    {
        product_id      =pid;
        manufacturer_id =mid;
        category_id     =cid;

        manufacturer_str=UTF8String(manufacturer_id);
        category_str=UTF8String(category_id);

        product_file.CreateTrunc("product.sql");
        //category_file.CreateTrunc("category.sql");

        product_sql=new io::UTF8TextOutputStream(&product_file);
        //category_sql=new io::LEDataOutputStream(&category_file);
    }

    void Close()
    {
        //delete category_sql;
        delete product_sql;
    }

    void ParseItemList(const char *str,const int size)
    {
        Json::Reader reader;
        Json::Value root;

        if(!reader.parse(std::string(str,size),root,false))
        {
            LOG_ERROR("解晰JSON串失败");
            return;
        }

        Json::Value itemlist=root["mods"]["itemlist"]["data"]["auctions"];

        LOG_INFO("商品数量： "+UTF8String(itemlist.size()));

        for(int i=0;i<itemlist.size();i++)
        {
            Json::Value item=itemlist[i];

            UTF8String nid=UTF8String(item["nid"].asCString());

            UTF8String pic_url=UTF8String("https:")+UTF8String(item["pic_url"].asCString());
            UTF8String pic_filename=UTF8String("catalog/TaobaoImage/")+nid+UTF8String(pic_url.strrchr('.'));

            UTF8String price=UTF8String(item["view_price"].asCString());
            UTF8String title=UTF8String(item["raw_title"].asCString());
            UTF8String detail;

            //下载详细信息
            {
                io::MemoryOutputStream mos;

                network::http::get(&mos,UTF8String("https:")+UTF8String(item["detail_url"].asCString()));

                const char *detail_text=(const char *)mos.GetData();

                if(!detail_text)
                    continue;

                const char *start=hgl::strstr(detail_text,tb_subtitle);

                if(!start)
                    continue;

                start+=tb_subtitle_size;
                const char *end=hgl::strstr(start,"</p>");

                if(end<=start)
                    continue;

                {
                    char *u8str;

                    int u8_len=ansi_to_utf8(ccpGBK,&u8str,start,end-start);

                    detail.Set(u8str,u8_len,true);
                }
            }

            //下载图片
            {
                io::FileOutputStream fos;
                fos.CreateTrunc(pic_filename);
                network::http::get(&fos,pic_url);
                fos.Close();
            }

            //产生SQL语句
            {
                UTF8String product_id_str=UTF8String(product_id);

                //插入产品
                UTF8String sql="INSERT INTO `oc_product` VALUES ("+product_id_str
                                +",'SN"+nid+"','','','','','','','',1000,100,'"
                                +pic_filename+"',"+manufacturer_str+",1,"+price
                                +",0,0,'2000-01-01',0,1,0.00000000,0.00000000,0.00000000,1,1,1,0,1,0,'2000-01-01 0:0:0','2000-01-01 00:00:00');";
                product_sql->WriteLine(sql);

                sql="INSERT INTO `oc_product_description` (`product_id`, `language_id`, `name`, `description`, `tag`, `meta_title`, `meta_description`, `meta_keyword`) VALUES ('"
                    +product_id_str+"', '2', '"+title+"', '"+detail+"', '"+title+"', '"+title+"', '', '');";
                product_sql->WriteLine(sql);

                sql="INSERT INTO `oc_product_to_category` (`product_id`, `category_id`) VALUES ('"+product_id_str+"', '"+category_str+"');";
                product_sql->WriteLine(sql);

                sql="INSERT INTO `oc_product_to_layout` (`product_id`, `store_id`, `layout_id`) VALUES ('"+product_id_str+"', '0', '0');";
                product_sql->WriteLine(sql);

                sql="INSERT INTO `oc_product_to_store` (`product_id`, `store_id`) VALUES ('"+product_id_str+"', '0');";
                product_sql->WriteLine(sql);

                product_sql->WriteLineEnd();

                ++product_id;
            }

//             LOG_INFO(UTF8String(i)+") 卖家： "+UTF8String(item["nick"].asCString())
//                         +UTF8String(" 价格： ")+price
//                         +UTF8String(" 名称： ")+title
//                         +UTF8String(" 图片： ")+pic_url
//                         +UTF8String(" 简介: ")+detail);
        }
    }

    void ParseHTML(const GumboNode *root)
    {
        if(root->type==GUMBO_NODE_TEXT)
        {
            if(strcmp(root->v.text.text,g_page_config,g_page_config_size)==0)      //找到商品数据项json串
            {
                const char *start   =root->v.text.text+g_page_config_size;
                const char *end     =strchr(start,'\n');

                ParseItemList(start,end-start-1);

                ++page_count;
            }

            return;
        }

        if(root->type!=GUMBO_NODE_ELEMENT)return;

        for(int i=0;i<root->v.element.children.length;i++)
            ParseHTML((const GumboNode *)(root->v.element.children.data[i]));
    }

    void ParseHTML(const char *data,const int size)
    {
        GumboOutput *output=gumbo_parse_with_options(&kGumboDefaultOptions,data,size);

        ParseHTML(output->root);

        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
};//class TaobaoParse

HGL_CONSOLE_MAIN_FUNC()
{
    sii.info.ProjectName=U8_TEXT("下载淘宝数据");
    sii.info.ProjectCode=OS_TEXT("DownloadTaobaoData");

    if(!app.Init(&sii))
        return(-1);

    if(args.GetCount()<8)
    {
        LOG_ERROR("Example: DownloadTaobaoData type low-price high-price page_count product_id manufacturer_id category_id");
        return(-1);
    }

    LOG_INFO(OS_TEXT("类型: ")+args[1]);
    LOG_INFO(OS_TEXT("最低限价: ")+args[2]);
    LOG_INFO(OS_TEXT("最高限价: ")+args[3]);
    LOG_INFO(OS_TEXT("获取页数: ")+args[4]);
    LOG_INFO(OS_TEXT("起始产品ID: ")+args[5]);
    LOG_INFO(OS_TEXT("品牌ID: ")+args[6]);
    LOG_INFO(OS_TEXT("分类ID: ")+args[7]);

    int page_count;
    int product_id;
    int manufacturer_id;
    int category_id;

    args[4].ToInt(page_count);
    args[5].ToInt(product_id);
    args[6].ToInt(manufacturer_id);
    args[7].ToInt(category_id);

    TaobaoParse tp;

    tp.Init(product_id,manufacturer_id,category_id);

    for(int i=0;i<page_count;i++)
    {
        UTF8String url=UTF8String("https://s.taobao.com/list?q=")+args[1];

        if(args[2]!=args[3])        //如果价格不相等
            url+=UTF8String("&sort=price-asc&filter=reserve_price[")+args[2]+UTF8String(',')+args[3]+UTF8String("]");

        if(i>0)
        {
            url+=UTF8String("&bcoffset=0&s=")+UTF8String(i*60);
        }

        LOG_INFO(OS_TEXT("URL: ")+url);

        io::MemoryOutputStream mos;

        network::http::get(&mos,url.c_str());

        tp.ParseHTML((const char *)mos.GetData(),mos.GetSize());
    }

    tp.Close();
    return 0;
}
