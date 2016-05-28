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
    io::FileOutputStream product_file;
    io::UTF8TextOutputStream *product_sql;

    int product_id;             ///<产品ID
    int manufacturer_id;        ///<品牌ID
    int category_id;            ///<分类ID

    int product_image_id;       ///<图片ID

    UTF8String manufacturer_str;
    UTF8String category_str;

    int page_count;

public:

    TaobaoParse()
    {
        product_file.CreateTrunc("product.sql");
        product_sql=new io::UTF8TextOutputStream(&product_file);
    }

    ~TaobaoParse()
    {
        delete product_sql;
    }

    void Init(int pid,int piid,int mid,int cid)
    {
        product_id      =pid;
        product_image_id=piid;
        manufacturer_id =mid;
        category_id     =cid;

        manufacturer_str=UTF8String(manufacturer_id);
        category_str=UTF8String(category_id);

        page_count=1;
    }

    void ParseItemHTML(const char *str,const int size)
    {
    }

    void ParseItemListJson(const char *str,const int size)
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

            int ai_count=0;

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
                    io::FileOutputStream fos;
                    fos.CreateTrunc(nid+".html");
                    fos.Write(mos.GetData(),mos.GetSize());
                    fos.Close();
                    break;
                }

                {
                    char *u8str;



                    int u8_len=ansi_to_utf8(ccpGBK,&u8str,start,end-start);

                    detail.Set(u8str,u8_len,true);

                    detail.Replace(' ','\'');   //干掉引号
                    detail.Replace(' ','\n');   //干掉换行
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

                //如果有多张图片
                {
                    for(int ai=0;ai<ai_count;ai++)
                    {
                        UTF8String ai_url=item["auctionImages"][ai].asCString();

                        UTF8String ai_pic_filename=UTF8String("catalog/TaobaoImage/")+nid+UTF8String("_")+UTF8String(ai)+UTF8String(ai_url.strrchr('.'));

                        UTF8String ai_pic_url=UTF8String("https:")+ai_url;

                        //下载图片
                        {
                            io::FileOutputStream fos;
                            fos.CreateTrunc(ai_pic_filename);
                            network::http::get(&fos,ai_pic_url);
                            fos.Close();
                        }

                        sql="INSERT INTO 'oc_product_image' ('product_image_id','product_id','image','sort_order') VALUES ('"
                            +UTF8String(product_image_id)+"','"
                            +UTF8String(product_id)+"','"
                            +ai_pic_filename+"','0');";
                        product_sql->WriteLine(sql);

                        product_image_id++;
                    }
                }

                product_sql->WriteLineEnd();

                ++product_id;
            }

//             LOG_INFO(UTF8String(i)+") 卖家： "+UTF8String(item["nick"].asCString())
//                         +UTF8String(",价格： ")+price
//                         +UTF8String(",名称： ")+title
//                         +UTF8String(",图片： ")+pic_url
//                         +UTF8String(",简介: ")+detail);
        }
    }

    void ParseItemListHTML(const GumboNode *root)
    {
        if(root->type==GUMBO_NODE_TEXT)
        {
            if(strcmp(root->v.text.text,g_page_config,g_page_config_size)==0)      //找到商品数据项json串
            {
                const char *start   =root->v.text.text+g_page_config_size;
                const char *end     =strchr(start,'\n');

                ParseItemListJson(start,end-start-1);

                ++page_count;
            }

            return;
        }

        if(root->type!=GUMBO_NODE_ELEMENT)return;

        for(int i=0;i<root->v.element.children.length;i++)
            ParseItemListHTML((const GumboNode *)(root->v.element.children.data[i]));
    }

    void ParseItemListHTML(const char *data,const int size)
    {
        GumboOutput *output=gumbo_parse_with_options(&kGumboDefaultOptions,data,size);

        ParseItemListHTML(output->root);

        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
};//class TaobaoParse

HGL_CONSOLE_MAIN_FUNC()
{
    sii.info.ProjectName=U8_TEXT("下载淘宝数据");
    sii.info.ProjectCode=OS_TEXT("DownloadTaobaoData");

    if(!app.Init(&sii))
        return(-1);

//     if(args.GetCount()<8)
//     {
//         LOG_ERROR("Example: DownloadTaobaoData type low-price high-price page_count product_id manufacturer_id category_id");
//         return(-1);
//     }
//
//     LOG_INFO(OS_TEXT("类型: ")+args[1]);
//     LOG_INFO(OS_TEXT("最低限价: ")+args[2]);
//     LOG_INFO(OS_TEXT("最高限价: ")+args[3]);
//     LOG_INFO(OS_TEXT("获取页数: ")+args[4]);
//     LOG_INFO(OS_TEXT("起始产品ID: ")+args[5]);
//     LOG_INFO(OS_TEXT("品牌ID: ")+args[6]);
//     LOG_INFO(OS_TEXT("分类ID: ")+args[7]);

    int page_count;
    int product_id      =107000;
    int product_image_id=10000;
    int manufacturer_id =100;
    int category_id;

//     args[4].ToInt(page_count);
//     args[5].ToInt(product_id);
//     args[6].ToInt(manufacturer_id);
//     args[7].ToInt(category_id);

    struct CategorySearch
    {
        int category_id;

        char search_str[64];

        int page;

//         int min_price;
//         int max_price;
    };

    const CategorySearch cs[]=
    {
//        {18,"家用电器",     },
//        {20,"电子数码",     },
//         {24,"家纺布艺",     },
//        {25,"家居生活",     },
//         {33,"箱包服饰",     },
//         {34,"个护美妆",     },
//        {57,"建材洁具",     },
//         {64,"儿童用品",     },
        {107,"办公用品",    50},

        {26,"台式电脑",     5},
        {28,"卧室家具",     20},
        {29,"日常生活",     30},
        {30,"生活装饰",     30},
        {31,"儿童家居",     20},
        {41,"身体护理",     20},
        {42,"彩妆",        40},
        {43,"头发护理",     20},
        {45,"家用电器",     60},
        {46,"厨房电器",     20},
        {65,"童装童鞋",     30},
        {67,"婴儿服饰",     30},
        {68,"孕妇用品",     10},
        {69,"母婴用品",     10},
        {70,"玩具+童车",    30},
        {71,"生活电器",      40},
        {72,"个人护理",     40},
        {74,"男装女装",     30},
        {81,"床上用品",     10},
        {82,"儿童床品",     10},
        {83,"窗帘布艺",     20},
        {85,"羽绒被",      5},
        {86,"服饰配件",     15},
        {87,"睡衣内衣",     40},
        {88,"拉杆箱",      5},
        {89,"电脑包",      5},
        {90,"女式包",      40},
        {91,"男式包",      5},
        {92,"书包",        15},
        {93,"笔记本",   20},       //笔记本电脑
        {94,"电脑配件",     40},
        {95,"电脑平板",     20},
        {96,"大屏智能",     50},   //手机
        {97,"摄像机摄影机",  20},
        {98,"MP3播放器",    20},
        {99,"家用五金",     50},
        {100,"开关插座",    5},
        {101,"灯具照明",    15},
        {102,"厨卫洁具",    10},
        {103,"厨卫装饰",    10},
        {104,"香水",      40},
        {105,"面部护理",    30},
        {108,"鞋靴",      50},
        {109,"厨具",      5},

        {110,"口红+唇膏",              20},
        {111,"底妆+粉饼+隔离霜+遮瑕",    20},
        {112,"腮红+胭脂",              20},
        {113,"眼影+眼霜",              20},
        {114,"美甲+洗甲+卸甲",          10},
        {115,"眼线+眼线笔",             20},
        {116,"眉笔+眉粉",              20},
        {117,"睫毛膏",                                  20},
        {118,"卸妆水+卸妆液+卸妆乳+卸妆油+卸妆啫喱+洁面巾",    20},
        {119,"防晒+隔离",                               20},

        {-1,"",0},
    };

    TaobaoParse tp;

    const CategorySearch *p=cs;

    for(;;)
    {
        if(p->category_id==-1)break;

        tp.Init(product_id,product_image_id,manufacturer_id,p->category_id);

        for(int i=0;i<p->page;i++)
        {
            UTF8String url=UTF8String("https://s.taobao.com/list?q=")+UTF8String(p->search_str);

//             if(p->min_price!=p->max_price)        //如果价格不相等
//                 url+=UTF8String("&sort=price-asc&filter=reserve_price[")+UTF8String(p->min_price)+UTF8String(',')+UTF8String(p->max_price)+UTF8String("]");

            if(i>0)
            {
                url+=UTF8String("&bcoffset=0&s=")+UTF8String(i*60);
            }

            LOG_INFO(OS_TEXT("URL: ")+url);

            io::MemoryOutputStream mos;

            network::http::get(&mos,url.c_str());

            tp.ParseItemListHTML((const char *)mos.GetData(),mos.GetSize());
        }

        product_id+=p->page*100;

        p++;
    }

    return 0;
}
