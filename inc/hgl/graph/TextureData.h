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

            uint GetColorFormat()const
            {
                return(source_format?source_format->color_format:0);
            }
        };//struct TextureData

        struct Texture1DData :public TextureData
        {
            uint length;

        public:

            Texture1DData()
            {
                hgl_zero(this, sizeof(Texture1DData));
            }

            Texture1DData(uint l, void *data, uint size, TSF sf, uint vf)
            {
                if (!TextureSourceFormatCheck(sf))
                {
                    hgl_zero(this, sizeof(Texture1DData));
                    return;
                }

                length=l;

                bitmap = data;
                bitmap_bytes = size;

                source_format = TextureFormatInfoList + sf;             //原始数据格式
                video_format = vf ? vf : source_format->video_format;   //显存格式
            }
        };//struct Texture1DData:public TextureData

        struct Texture2DData :public TextureData
        {
            uint width, height;

        public:

            Texture2DData()
            {
                hgl_zero(this, sizeof(Texture2DData));
            }

            Texture2DData(uint w,uint h,void *data,uint size,TSF sf,uint vf)
            {
                if (!TextureSourceFormatCheck(sf))
                {
                    hgl_zero(this, sizeof(Texture2DData));
                    return;                    
                }

                width=w;
                height=h;

                bitmap=data;
                bitmap_bytes=size;

                source_format=TextureFormatInfoList + sf;       //原始数据格式
                video_format=vf?vf:source_format->video_format; //显存格式
            }
        };//struct Texture2DData :public TextureData

        struct Texture1DArrayData :public TextureData
        {
            uint length, count;

        public:

            Texture1DArrayData()
            {
                hgl_zero(this, sizeof(Texture1DArrayData));
            }
        };

        struct Texture2DArrayData :public TextureData
        {
            uint width, height, layer;

        public:

            Texture2DArrayData()
            {
                hgl_zero(this, sizeof(Texture2DArrayData));
            }
        };

        struct Texture3DData :public TextureData
        {
            uint width, height, depth;

        public:

            Texture3DData()
            {
                hgl_zero(this, sizeof(Texture3DData));
            }
        };
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_DATA_INCLUDE
