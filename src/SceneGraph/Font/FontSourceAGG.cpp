#include"FontSourceAGG.h"
#include"agg_conv_curve.h"
#include"agg_conv_contour.h"
#include"agg_renderer_primitives.h"
#include"agg_conv_transform.h"
#include"agg_gamma_lut.h"

namespace hgl
{
    namespace graph
    {
        //template<class VertexSource> class faux_weight
        //{
        //public:

        //  faux_weight(VertexSource& vs) :
        //      m_mtx_zoom_in_y(agg::trans_affine_scaling(1, 100)),
        //      m_mtx_zoom_out_y(agg::trans_affine_scaling(1, 1.0/100.0)),
        //      m_source(&vs),
        //      m_trans_zoom_in_y(*m_source, m_mtx_zoom_in_y),
        //      m_contour(m_trans_zoom_in_y),
        //      m_trans_zoom_out(m_contour, m_mtx_zoom_out_y)
        //  {
        //      m_contour.auto_detect_orientation(false);
        //  }

        //  void weight(double v) { m_contour.width(v); }

        //  void rewind(unsigned path_id=0) {m_trans_zoom_out.rewind(path_id); }
        //  unsigned vertex(double* x, double* y) { return m_trans_zoom_out.vertex(x, y); }

        //private:

        //  agg::trans_affine m_mtx_zoom_in_y;
        //  agg::trans_affine m_mtx_zoom_out_y;
        //  VertexSource* m_source;
        //  agg::conv_transform<VertexSource> m_trans_zoom_in_y;
        //  agg::conv_contour<agg::conv_transform<VertexSource> > m_contour;
        //  agg::conv_transform<agg::conv_contour<agg::conv_transform<VertexSource> > > m_trans_zoom_out;
        //};

        AGGBitmapFont::AGGBitmapFont(const FontInfo &fnt):FontSource(fnt)
        {
            fnt_size=fnt.height;

            data=new int8u[fnt_size*fnt_size*font_pixel_bytes];

            memset(data,0,fnt_size*fnt_size*font_pixel_bytes);

            buf=new rendering_buffer(data,fnt_size,fnt_size,fnt_size*font_pixel_bytes);
            fmt=new font_pixfmt(*buf);
            base=new renderer_base<font_pixfmt>(*fmt);

            fnt_hdc=CreateCompatibleDC(0);

            fnt_engine=new font_engine_type(fnt_hdc);
            fnt_manage=new font_manager_type(*fnt_engine);

            fnt_engine->hinting(true);
            fnt_engine->flip_y(true);

            fnt_engine->height(fnt.height);
            fnt_engine->width(fnt.width);
            fnt_engine->italic(fnt.italic);
            fnt_engine->weight(fnt.bold?FW_BOLD:FW_NORMAL);

            //fnt_engine->create_font(fnt.name,fnt.anti?glyph_ren_agg_gray8:glyph_ren_native_mono);
            fnt_engine->create_font(fnt.name,fnt.anti?glyph_ren_outline:glyph_ren_native_mono);
        }

        AGGBitmapFont::~AGGBitmapFont()
        {
            delete fnt_manage;
            delete fnt_engine;
            delete base;
            delete fmt;
            delete buf;
            delete[] data;
            DeleteDC(fnt_hdc);
        }

        bool AGGBitmapFont::MakeCharBitmap(wchar_t ch)                                      ///<产生字体数据
        {
            const agg::glyph_cache *glyph = fnt_manage->glyph(ch);

            if(!glyph)
                return(false);

            FontSource::Bitmap *bmp=char_bitmap+ch;

            bmp->w      =fnt_size;//glyph->bounds.x2-glyph->bounds.x1;
            bmp->h      =fnt_size;//glyph->bounds.y2-glyph->bounds.y1;
            bmp->adv_x  =glyph->advance_x+0.9;
            bmp->adv_y  =glyph->advance_y;
            bmp->x      =glyph->bounds.x1;
            bmp->y      =fnt_size+glyph->bounds.y1;

            double cur_x=-glyph->bounds.x1;
            double cur_y=-glyph->bounds.y1;

            bmp->data=new unsigned char[fnt_size*fnt_size*font_pixel_bytes];
            memset(bmp->data,0,fnt_size*fnt_size*font_pixel_bytes);

            buf->attach(bmp->data,fnt_size,fnt_size,fnt_size*font_pixel_bytes);

            //fnt_manage->add_kerning(&cur_x, &cur_y);      //排版缩进用，因为我们是自己排版，所以这个用不到
            fnt_manage->init_embedded_adaptors(glyph,cur_x,cur_y);

            if(glyph->data_type==glyph_data_outline)
            {
                rasterizer_scanline_aa<> ras;
                renderer_scanline_aa_solid<renderer_base<font_pixfmt>>  ras_aa_solid(*base);
                scanline32_u8 sl;

                agg::trans_affine m_mtx;
                agg::conv_curve<font_manager_type::path_adaptor_type> m_curves(fnt_manage->path_adaptor());
                agg::conv_transform<agg::conv_curve<font_manager_type::path_adaptor_type> > m_trans(m_curves,m_mtx);
                //faux_weight<agg::conv_transform<agg::conv_curve<font_manager_type::path_adaptor_type> > > m_weight(m_trans);

                agg::gamma_lut<> gamma(1.9f);
                fmt->apply_gamma_inv(gamma);

                ras_aa_solid.color(rgba(1,1,1,1));

                ras.reset();
                ras.add_path(m_trans);

                render_scanlines(   ras,
                                    sl,
                                    ras_aa_solid);
            }
            //else
            //if(glyph->data_type==glyph_data_gray8)
            //{
            //  renderer_scanline_aa_solid<renderer_base<font_pixfmt>>  ras_aa_solid(*base);
            //  ras_aa_solid.color(rgba(1,1,1,1));
            //
            //  render_scanlines(   fnt_manage->gray8_adaptor(),
            //                      fnt_manage->gray8_scanline(),
            //                      ras_aa_solid);
            //}
            else
            if(glyph->data_type==glyph_data_mono)
            {
                renderer_scanline_bin_solid<renderer_base<font_pixfmt>> ras_bin_solid(*base);
                ras_bin_solid.color(rgba(1,1,1,1));

                render_scanlines(   fnt_manage->mono_adaptor(),
                                    fnt_manage->mono_scanline(),
                                    ras_bin_solid);
            }


            return(true);
        }
    }//namespace graph
}//namespace hgl
