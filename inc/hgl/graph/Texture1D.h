#ifndef HGL_GRAPH_TEXTURE_1D_INCLUDE
#define HGL_GRAPH_TEXTURE_1D_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
    namespace graph
    {
        struct Texture1DData;

        /**
        * 1D贴图
        */
        class Texture1D:public Texture
        {
        protected:

            uint length;

        protected:
        
            virtual bool    _SetImage   (Texture1DData *)=0;
            virtual int     _GetImage   (void *data_pointer, TSF fmt, int level)=0;
            virtual bool    _ChangeImage(uint s, uint l, void *data, uint bytes, TSF sf)=0;

        public:    //

            uint    GetLength       ()const{return length;}                                            ///<取得纹理长度

        public:

                    Texture1D       (TextureBaseControl *t):Texture(t){length=0;}

            bool    SetImage        (Texture1DData *);                                                ///<创建1D贴图数据
            bool    SetImage        (uint l, TSF fmt)                                                    ///<创建1D贴图数据
                                    {
                                        Texture1DData tex(l,fmt);

                                        return SetImage(&tex);
                                    }

            bool    SetImage        (uint l, void *data, uint size, TSF sf, uint vf)
                                    {
                                        Texture1DData tex(l, data, size, sf, vf);

                                        return SetImage(&tex);
                                    }

            bool    ChangeImage     (uint s,uint l,void *data,uint size,TSF sf);                    ///<更改1D贴图数据

            int     GetImage        (void *data_pointer,TSF fmt,int level=0);                        ///<取得1D贴图数据
        };//class Texture1D

        Texture1D *CreateTexture1D();
        Texture1D *CreateTexture1D(Texture1DData *);
        Texture1D *CreateTexture1D(uint length,TSF format);
        Texture1D *CreateTexture1D(uint length,void *bitmap,uint bitmap_bytes,TSF source_format,uint video_format);
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_1D_INCLUDE
