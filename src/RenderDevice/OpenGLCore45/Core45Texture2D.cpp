#include<glew/include/GL/glew.h>
#include<hgl/graph/Texture2D.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    namespace graph
    {
        Texture2D::Texture2D()
        {
            type=HGL_TEXTURE_2D;
			width=height=wrap_s=wrap_t=0;
            glCreateTextures(GL_TEXTURE_2D,1,&texture_id);
        }

        Texture2D::~Texture2D()
        {
            glDeleteTextures(1,&texture_id);
        }

        bool Texture2D::SetImage(unsigned int w,unsigned int h,void *data,unsigned int image_size,TSF sf,unsigned int vf,unsigned int ltp)
        {
            if(!w||!h)
            {
                LOG_ERROR(OS_TEXT("size error,width=")+OSString(w)+OS_TEXT(",height=")+OSString(h));
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

            width=w;
            height=h;

            const TextureFormat *sfmt=TextureFormatInfoList+sf;       //原始数据格式

            if(vf==0)
                vf=TextureFormatInfoList[sf].video_format;

            const bool gen_mip=ltp&ltGenMipmaps;            //取得是否创建mipmaps

            if(sfmt->compress)      //原本就是压缩格式
            {
				if(data)
                glCompressedTextureSubImage2D(texture_id,0,0,0,w,h,vf,image_size,data);
            }
            else                    //正常非压缩格式
            {
                glTextureStorage2D(texture_id, 1, vf, w, h);

				if(data)
                glTextureSubImage2D(texture_id, 0, 0, 0, w, h, sfmt->color_format, sfmt->data_type, data);
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

                wrap_s=GL_REPEAT;
                wrap_t=GL_REPEAT;

                //未来使用Sampler Object，则不再需要以下部分
                glTextureParameteri(texture_id,GL_TEXTURE_MIN_FILTER,min_filter);
                glTextureParameteri(texture_id,GL_TEXTURE_MAG_FILTER,mag_filter);
                glTextureParameteri(texture_id,GL_TEXTURE_WRAP_S,wrap_s);
                glTextureParameteri(texture_id,GL_TEXTURE_WRAP_T,wrap_t);
            }

            return(true);
        }

        int Texture2D::GetImage(void *data_pointer,TSF fmt,int level)
        {
            if(!TextureSourceFormatCheck(fmt))
            {
                LOG_ERROR(OS_TEXT("glTexture2D::GetImage,fmt error =")+OSString(fmt));
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

                bytes=width*height*tsf->video_bytes;

                if(data_pointer)
                    glGetTextureImage(texture_id,level,tsf->color_format,tsf->data_type,bytes,data_pointer);
            }

            return(bytes);
        }

        bool Texture2D::ChangeImage(uint l,uint t,uint w,uint h,void *data,uint bytes,TSF sf)
        {
            if(	l>width||t>height
				||w>width-l
				||h>height-t
				||!data
				||!TextureSourceFormatCheck(sf))
                RETURN_FALSE;

            const TextureFormat *sfmt=TextureFormatInfoList+sf;       //原始数据格式

            if(sfmt->compress)
                glCompressedTextureSubImage2D(texture_id,0,l,t,w,h,sfmt->video_format,bytes,data);
            else
                glTextureSubImage2D(texture_id,0,l,t,w,h,sfmt->color_format,sfmt->data_type,data);

            return(true);
        }

        void Texture2D::SetWrapS(uint wrap)
        {
            if(wrap_s==wrap)return;

            wrap_s=wrap;

            glTextureParameteri(texture_id,GL_TEXTURE_WRAP_S,wrap_s);
        }

        void Texture2D::SetWrapT(uint wrap)
        {
            if(wrap_t==wrap)return;

            wrap_t=wrap;

            glTextureParameteri(texture_id,GL_TEXTURE_WRAP_T,wrap_t);
        }
    }//namespace graph
}//namespace hgl
