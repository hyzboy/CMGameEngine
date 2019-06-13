#ifndef HGL_GRAPH_FONT_SOURCE_AGG_INCLUDE
#define HGL_GRAPH_FONT_SOURCE_AGG_INCLUDE

#include<hgl/graph/FontSource.h>

#include"agg_rendering_buffer.h"
#include"agg_renderer_base.h"
//#include"agg_pixfmt_gray.h"
#include"agg_pixfmt_rgba.h"
#include"agg_scanline_u.h"
#include"agg_renderer_scanline.h"
#include"agg_rasterizer_scanline_aa.h"
#include"agg_font_win32_tt.h"

namespace hgl
{
    namespace graph
    {
        using namespace agg;

        typedef font_engine_win32_tt_int32 font_engine_type;
        typedef font_cache_manager<font_engine_type> font_manager_type;

        //typedef pixfmt_gray8 font_pixfmt;
        typedef pixfmt_bgra32 font_pixfmt;

        #define font_pixel_bytes    4

        class AGGBitmapFont:public FontSource
        {
            HDC fnt_hdc;

            int fnt_size;

            int8u *data;

            rendering_buffer *buf;

            font_pixfmt *fmt;
            renderer_base<font_pixfmt> *base;

            font_engine_type *  fnt_engine;     //字符引擎
            font_manager_type * fnt_manage;     //字符管理缓冲

        protected:

            int LineHeight;
            int LineDistance;

        public:

            AGGBitmapFont(const FontInfo &fnt);
            ~AGGBitmapFont();

            bool MakeCharBitmap(wchar_t ch);                ///<产生字体数据
            int GetLineHeight()const{return LineHeight;}    ///<取得行高
        };//class AGGBitmapFont
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_FONT_SOURCE_AGG_INCLUDE
