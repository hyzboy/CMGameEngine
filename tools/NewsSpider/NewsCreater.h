#pragma once

#include<hgl/io/MemoryOutputStream.h>
#include<hgl/io/TextOutputStream.h>
#include<hgl/filesystem/FileSystem.h>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::filesystem;

class NewsCreater
{
    OSString save_path;

    MemoryOutputStream mos;
    TextOutputStream *tos;

public:

    int img_count;
    UTF8String first_image;
    UTF8String first_line;

public:

    NewsCreater(const OSString &sp)
    {
        save_path=sp;

        img_count=0;

        tos=new UTF8TextOutputStream(&mos);

        tos->WriteLine(UTF8String(R"(
<html>
    <head>
        <meta charset="utf-8" name=viewport content="width=device-width,initial-scale=1,maximum-scale=1,minimum-scale=1,user-scalable=no"/>
        <style>
            *{
                margin: 0px;
                padding: 0px;
            }

            p{
                margin: 10px 0px;
            }

            body{
                padding: 10px;
            }

            .ImageLayout{
                display: block;
                width: 100%;
            }
        </style>
    </head>
    <body>
)"));
    }

    ~NewsCreater()
    {
        delete tos;
    }

    void Save()
    {
        const OSString filename=MergeFilename(save_path,OS_TEXT("index.html"));

        tos->WriteLine(UTF8String("</body></html>"));
        SaveMemoryToFile(filename,mos.GetData(),mos.Tell());
    }

    /**
     * 写入一段正文
     */
    void WriteText(const UTF8String &text)
    {
        if(text.Length()<=0)return;

        tos->WriteLine(U8_TEXT("<p>")+text+U8_TEXT("</p>"));

        if(first_line.IsEmpty())
            first_line=text;
    }

    void WriteImage(const UTF8String &url)
    {
        FileOutputStream fos;

        const UTF8String ext_name=UTF8String(".")+ClipFileExtName(url);

        const UTF8String img_filename=UTF8String(img_count)+ext_name;

        const OSString local_filename=MergeFilename(save_path,ToOSString(img_filename));

        if(fos.CreateTrunc(local_filename))
        {
            std::cout<<"download image: "<<url.c_str()<<std::endl;

            network::http::get(&fos,url);

            fos.Close();
        }

        tos->WriteLine(UTF8String("<img src=\"")+img_filename+UTF8String("\" class=\"ImageLayout\">"));

        if(first_image.IsEmpty())
            first_image=img_filename;

        ++img_count;
    }
};//class NewsCreater
