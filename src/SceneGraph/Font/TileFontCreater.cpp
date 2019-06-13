#include<hgl/graph/TileFont.h>
#include<hgl/graph/FontInfo.h>
#include<hgl/graph/Render.h>
#include<hgl/type/IndexData.h>
#include"FontSourceWin.h"

namespace hgl
{
    namespace graph
    {
        class FontSourceStorage:public IndexObject<FontInfo,FontSource>
        {
            FontSource *CreateObject(const FontInfo &fnt)
            {
                return(new WinBitmapFont(fnt));
            }

            void DeleteObject(const FontInfo &,FontSource *source)
            {
                if(source)
                    delete source;
            }

        public:
        };//class FontSourceStorage

        static FontSourceStorage FontStorage;

        void InitFontStorage()
        {
        }

        void ClearFontStorage()
        {
            FontStorage.Clear();
        }

        FontSource *GetFontSource(const FontInfo &fnt)
        {
            FontSource *source;

            if(FontStorage.Get(fnt,source))
                return(source);

            source=new WinBitmapFont(fnt);

            FontStorage.Add(fnt,source);
            return(source);
        }

        /**
        * 通过系统字体创建一个Tile字体,字体大小以较大的为准
        * @param chs_fnt 英文字体
        * @param eng_fnt 中文字体
        * @param count 缓冲区内保存的字符个数,-1表示自动取(会根据屏幕大小取最大值)
        */
        TileFont *CreateTileFont(const FontInfo &chs_fnt,const FontInfo &eng_fnt,int count)
        {
            int font_height;
            int height;

            if(chs_fnt.height>eng_fnt.height)
                height=chs_fnt.height;
            else
                height=eng_fnt.height;

            font_height=height;

            if(chs_fnt.bold     ||eng_fnt.bold
             ||chs_fnt.italic   ||eng_fnt.italic)height+=height/4;

            height+=3;
            height-=height&3;                       //大小必须能被4整除

//          if(height<16)
//              height=16;

            if(count==-1)                           //自动,按屏幕填满字计算
            {
                int sw,sh;

                GetScreenSize(sw,sh);

                count=((sw+height-1)/height)
                     *((sh+height-1)/height);
            }

            TileData *td=new TileData(height,height,count,HGL_R8);

            FontSource *cfs=GetFontSource(chs_fnt);
            FontSource *efs=GetFontSource(eng_fnt);

            return(new TileFont(font_height,td,cfs,efs));
        }

        /**
        * 通过系统字体创建一个Tile字体
        * @param chs_fontname 中文字体名称
        * @param eng_fontname 英文字体名称
        * @param width 宽,可以为0,表示默认。
        * @param height 高
        * @param bold 加粗,默认false
        * @param italic 斜体,默认false
        * @param count 缓冲区内保存的字符个数
        */
        TileFont *CreateTileFont(const wchar_t *chs_fontname,const wchar_t *eng_fontname,int width,int height,bool bold,bool italic,int count)
        {
            FontInfo chs_fnt(chs_fontname,width,height,bold,italic);
            FontInfo eng_fnt(eng_fontname,width,height,bold,italic);

            return CreateTileFont(chs_fnt,eng_fnt,count);
        }
    }//namespace graph
}//namespace hgl
