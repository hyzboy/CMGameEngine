#include<glew/include/GL/glew.h>
#include<hgl/graph/Texture1D.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    namespace graph
    {
        Texture1D::Texture1D()
        {
			type=HGL_TEXTURE_1D;
			length=wrap=0;
            glCreateTextures(GL_TEXTURE_1D,1,&texture_id);
        }

        Texture1D::~Texture1D()
        {
            glDeleteTextures(1,&texture_id);
        }

        bool Texture1D::SetImage(unsigned int l,void *data,unsigned int image_size,TSF sf,unsigned int vf,unsigned int ltp)
        {
            if(!l)
            {
                LOG_ERROR(OS_TEXT("size error,length=")+OSString(l));
                return(false);
            }

            if(data)        //无DATA数据时不用关心源格式
            {
				if(!TextureSourceFormatCheck(sf))
                {
                    LOG_ERROR(OS_TEXT("sf error =")+OSString(sf));
                    return(false);
                }
            }

            length=l;

            const TextureFormat *sfmt=TextureFormatInfoList+sf;       //原始数据格式

            if(vf==0)
                vf=TextureFormatInfoList[sf].video_format;

            const bool gen_mip=ltp&ltGenMipmaps;            //取得是否创建mipmaps

            if(sfmt->compress)      //原本就是压缩格式
            {
                glCompressedTextureSubImage1D(texture_id,0,0,l,vf,image_size,data);
            }
            else                    //正常非压缩格式
            {
                glTextureStorage1D(texture_id, 1, vf, l);
                glTextureSubImage1D(texture_id, 0, 0, l, sfmt->color_format, sfmt->data_type, data);
            }

            video_format=vf;
            color_format=sfmt->color_format;

            if(gen_mip)
            {
                glGenerateTextureMipmap(texture_id);

//                  glTexEnvf(GL_TEXTURE_FILTER_CONTROL,GL_TEXTURE_LOD_BIAS,-1.5f);     //设置LOD偏向,负是更精细，正是更模糊
            }

            {
                min_filter=gen_mip?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR;
                mag_filter=GL_LINEAR;

                wrap=GL_REPEAT;

                //未来使用Sampler Object，则不再需要以下部分
                glTextureParameteri(texture_id,GL_TEXTURE_MIN_FILTER,min_filter);
                glTextureParameteri(texture_id,GL_TEXTURE_MAG_FILTER,mag_filter);
                glTextureParameteri(texture_id,GL_TEXTURE_WRAP_S,wrap);
            }

            return(true);
        }

        int Texture1D::GetImage(void *data_pointer,TSF fmt,int level)
        {
			if(!TextureSourceFormatCheck(fmt))
            {
                LOG_ERROR(OS_TEXT("glTexture1D::GetImage,fmt error =")+OSString(fmt));
                return(-1);
            }

            int compress;
            int bytes;

            const TextureFormat *tsf=TextureFormatInfoList+fmt;

            glGetTextureLevelParameteriv(texture_id,level,GL_TEXTURE_COMPRESSED,&compress);

            if(compress)
            {
                glGetTextureLevelParameteriv(texture_id,level,GL_TEXTURE_COMPRESSED_IMAGE_SIZE,&bytes);

                if(data_pointer)
                    glGetCompressedTextureImage(texture_id,level,bytes,data_pointer);
            }
            else
            {
                if(tsf->video_bytes==0)return(-1);

                bytes=length*tsf->video_bytes;

                if(data_pointer)
                    glGetTextureImage(texture_id,level,tsf->color_format,tsf->data_type,bytes,data_pointer);
            }

            return(bytes);
        }

        bool Texture1D::ChangeImage(uint s,uint l,void *data,uint bytes,TSF sf)
        {
            if(s>=length
			||l>length-s
			||!data
			||!TextureSourceFormatCheck(sf))
            {
                LOG_ERROR(OS_TEXT("fmt error =")+OSString(sf));
                return(false);
            }

            const TextureFormat *sfmt=TextureFormatInfoList+sf;       //原始数据格式

            if(sfmt->compress)
                glCompressedTextureSubImage1D(texture_id,0,s,l,sfmt->video_format,bytes,data);
            else
                glTextureSubImage1D(texture_id,0,s,l,sfmt->color_format,sfmt->data_type,data);

            return(true);
        }

        void Texture1D::SetWrap(uint w)
        {
            if(wrap==w)return;

            wrap=w;

            glTextureParameteri(texture_id,GL_TEXTURE_WRAP_S,wrap);
        }
    }//namespace graph
}//namespace hgl
