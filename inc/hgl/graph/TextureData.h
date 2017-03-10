#ifndef HGL_GRAPH_TEXTURE_DATA_INCLUDE
#define HGL_GRAPH_TEXTURE_DATA_INCLUDE

#include<hgl/graph/TextureFormat.h>
namespace hgl
{
    namespace graph
    {
        /**
        * 纹理数据
        */
        struct TextureData
        {
            const TextureFormat   * source_format;                  ///<源始纹理格式

            uint                    video_format;                   ///<显存格式

            bool                    gen_mipmaps;                    ///<是否产生mipmaps

            void *                  bitmap;                         ///<位图数据

            uint                    bitmap_bytes;                   ///<位图数据字节数

        public:

            uint GetVideoFormat()const
            {
                return(video_format?video_format:source_format->video_format);
            }

            uint GetPixelFormat()const
            {
                return(source_format?source_format->pixel_format:0);
            }

            bool SetData(size_t this_size,TSF fmt)
            {
                if(!TextureSourceFormatCheck(fmt))
                {
                    hgl_zero(this,this_size);
                    return(false);
                }

                gen_mipmaps=false;
                bitmap = nullptr;
                bitmap_bytes = 0;

                source_format = TextureFormatInfoList + fmt;       //原始数据格式
                video_format = source_format->video_format;         //显存格式

                return(true);
            }

            bool SetData(size_t this_size,void *data,uint size,TSF sf,uint vf)
            {
                if(data&&!TextureSourceFormatCheck(sf))
                {
                    hgl_zero(this,this_size);
                    return(false);
                }

                gen_mipmaps = false;
                bitmap = data;
                bitmap_bytes = size;

                source_format = TextureFormatInfoList + sf;             //原始数据格式
                video_format = vf ? vf : source_format->video_format;   //显存格式
                return(true);
            }
        };//struct TextureData

        struct Texture1DData:public TextureData
        {
            uint length;

        public:

            Texture1DData()
            {
                hgl_zero(this, sizeof(Texture1DData));
            }

            Texture1DData(uint l, TSF fmt)
            {
                if(!SetData(sizeof(Texture1DData),fmt))return;

                length = l;
            }

            Texture1DData(uint l, void *data, uint size, TSF sf, uint vf)
            {
                if(!SetData(sizeof(Texture1DData),data,size,sf,vf))return;

                length=l;
            }
        };//struct Texture1DData:public TextureData

        struct Texture2DData:public TextureData
        {
            uint width, height;

        public:

            Texture2DData()
            {
                hgl_zero(this, sizeof(Texture2DData));
            }

            Texture2DData(uint w, uint h, TSF fmt)
            {
                if(!SetData(sizeof(Texture2DData),fmt))return;

                width = w;
                height = h;
            }

            Texture2DData(uint w,uint h,void *data,uint size,TSF sf,uint vf)
            {
                if(!SetData(sizeof(Texture2DData),data,size,sf,vf))return;

                width=w;
                height=h;
            }
        };//struct Texture2DData:public TextureData

        using TextureCubeMapData=Texture2DData;     //cube map与2d参数一样，只是数据长6倍

        struct Texture1DArrayData:public TextureData
        {
            uint length, count;

        public:

            Texture1DArrayData()
            {
                hgl_zero(this,sizeof(Texture1DArrayData));
            }

            Texture1DArrayData(uint l, uint c, TSF fmt)
            {
                if(!SetData(sizeof(Texture1DArrayData),fmt))return;

                length=l;
                count=c;
            }

            Texture1DArrayData(uint l,uint c,void *data,uint size,TSF sf,uint vf)
            {
                if(!SetData(sizeof(Texture1DArrayData),data,size,sf,vf))return;

                length=l;
                count=c;
            }
        };//struct Texture1DArrayData:public TextureData
//
//         struct Texture2DArrayData:public TextureData
//         {
//             uint width, height, layer;
//
//         public:
//
//             Texture2DArrayData()
//             {
//                 hgl_zero(this, sizeof(Texture2DArrayData));
//             }
//         };//struct Texture2DArrayData:public TextureData
//
//         struct Texture3DData:public TextureData
//         {
//             uint width, height, depth;
//
//         public:
//
//             Texture3DData()
//             {
//                 hgl_zero(this, sizeof(Texture3DData));
//             }
//         };//struct Texture3DData:public TextureData
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_DATA_INCLUDE
