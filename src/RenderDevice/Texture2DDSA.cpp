#include<hgl/graph/Texture2D.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    namespace graph
    {
        class TextureDSA2D:public Texture2D
        {
        public:

            using Texture2D::Texture2D;
            
            bool _SetImage(Texture2DData *tex)
            {
                glTextureStorage2D(texture_id, 1, tex->video_format, tex->width, tex->height);
				
				if(!tex->bitmap)
					return(true);
				
				if(tex->source_format->compress)      //原本就是压缩格式
                        glCompressedTextureSubImage2D(texture_id, 0, 0, 0, tex->width, tex->height, tex->video_format, tex->bitmap_bytes, tex->bitmap);
                else                    //正常非压缩格式
                        glTextureSubImage2D(texture_id, 0, 0, 0, tex->width, tex->height, tex->source_format->pixel_format, tex->source_format->data_type, tex->bitmap);
				
                if (tex->gen_mipmaps)
                {
                    if (tex->bitmap)
                        glGenerateTextureMipmap(texture_id);

                    //                  glTexEnvf(GL_TEXTURE_FILTER_CONTROL,GL_TEXTURE_LOD_BIAS,-1.5f);     //设置LOD偏向,负是更精细，正是更模糊
                }

                return(true);
            }
            
            int _GetImage(void *data_pointer, TSF fmt, int level,int width,int height)
            {
                int compress;
                int bytes;

                const TextureFormat *tsf = TextureFormatInfoList + fmt;

                glGetTextureLevelParameteriv(texture_id, level, GL_TEXTURE_COMPRESSED, &compress);

                if (compress)
                {
                    glGetTextureLevelParameteriv(texture_id, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &bytes);

                    if (data_pointer)
                        glGetCompressedTextureImage(texture_id, level, bytes, data_pointer);
                }
                else
                {
                    if (tsf->video_bytes == 0)return(-1);

                    bytes = width*height*tsf->video_bytes;

                    if (data_pointer)
                        glGetTextureImage(texture_id, level, tsf->pixel_format, tsf->data_type, bytes, data_pointer);
                }

                return(bytes);
            }

            bool _ChangeImage(uint l, uint t, uint w, uint h, void *data, uint bytes, TSF sf)
            {
                const TextureFormat *sfmt = TextureFormatInfoList + sf;       //原始数据格式

                if (sfmt->compress)
                    glCompressedTextureSubImage2D(texture_id, 0, l, t, w, h, sfmt->video_format, bytes, data);
                else
                    glTextureSubImage2D(texture_id, 0, l, t, w, h, sfmt->pixel_format, sfmt->data_type, data);

                return(true);
            }
        };//class TextureDSA2D

        Texture2D *CreateTexture2DDSA(TextureBaseControl *tbc)
        {
            return(new TextureDSA2D(tbc));
        }
    }//namespace graph
}//namespace hgl
