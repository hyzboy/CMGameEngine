#include<hgl/graph/Texture1D.h>
#include<hgl/LogInfo.h>

namespace hgl
{
	namespace graph
	{
        class TextureDSA1D:public Texture1D
        {
        public:

            using Texture1D::Texture1D;

            bool _SetImage(Texture1DData *tex)
            {                
                if (tex->source_format->compress)      //原本就是压缩格式
                {
                    if(tex->bitmap)
                        glCompressedTextureSubImage1D(texture_id, 0, 0, tex->length, tex->video_format, tex->bitmap_bytes, tex->bitmap);
                }
                else                    //正常非压缩格式
                {
                    glTextureStorage1D(texture_id, 1, tex->video_format, tex->length);

                    if(tex->bitmap)
                        glTextureSubImage1D(texture_id, 0, 0, tex->length, tex->source_format->color_format, tex->source_format->data_type, tex->bitmap);
                }

                if (tex->gen_mipmaps)
                {
                    if(tex->bitmap)
                        glGenerateTextureMipmap(texture_id);

                    //                  glTexEnvf(GL_TEXTURE_FILTER_CONTROL,GL_TEXTURE_LOD_BIAS,-1.5f);     //设置LOD偏向,负是更精细，正是更模糊
                }

                return(true);
            }

            int _GetImage(void *data_pointer, TSF fmt, int level)
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

                    bytes = length*tsf->video_bytes;

                    if (data_pointer)
                        glGetTextureImage(texture_id, level, tsf->color_format, tsf->data_type, bytes, data_pointer);
                }

                return(bytes);
            }
            
            bool _ChangeImage(uint s, uint l, void *data, uint bytes, TSF sf)
            {
                const TextureFormat *sfmt = TextureFormatInfoList + sf;       //原始数据格式

                if (sfmt->compress)
                    glCompressedTextureSubImage1D(texture_id, 0, s, l, sfmt->video_format, bytes, data);
                else
                    glTextureSubImage1D(texture_id, 0, s, l, sfmt->color_format, sfmt->data_type, data);

                return(true);
            }
        };//class TextureDSA1D:public Texture1D

        Texture1D *CreateTexture1DDSA(TextureBaseControl *tbc)
        {
            return(new TextureDSA1D(tbc));
        }
    }//namespace graph
}//namespace hgl
