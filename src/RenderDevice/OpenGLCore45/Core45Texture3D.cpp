#include<glew/include/GL/glew.h>
#include<hgl/graph/Texture3D.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    namespace graph
    {
        Texture3D::Texture3D()
        {
            type=HGL_TEXTURE_3D;
			width=height=depth=wrap_s=wrap_t=wrap_r=0;
            glCreateTextures(GL_TEXTURE_3D,1,&texture_id);
        }

        Texture3D::~Texture3D()
        {
            glDeleteTextures(1,&texture_id);
        }

        bool Texture3D::SetImage(uint w,uint h,uint d,void *data,uint image_size,TSF sf,uint vf,uint ltp)
        {
            if(!w||!h||!d)
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
			depth=d;

            const TextureFormat *sfmt=TextureFormatInfoList+sf;       //原始数据格式

            if(vf==0)
                vf=TextureFormatInfoList[sf].video_format;

            const bool gen_mip=ltp&ltGenMipmaps;            //取得是否创建mipmaps

            if(sfmt->compress)      //原本就是压缩格式
            {
				if(data)
					glCompressedTextureSubImage3D(texture_id,0,0,0,0,w,h,d,vf,image_size,data);
            }
            else                    //正常非压缩格式
            {
                glTextureStorage3D(texture_id, 1, vf, w, h,d);

				if(data)
                glTextureSubImage3D(texture_id, 0,0,0,0, w,h,d, sfmt->color_format, sfmt->data_type, data);
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
				wrap_r=GL_REPEAT;

                //未来使用Sampler Object，则不再需要以下部分
                glTextureParameteri(texture_id,GL_TEXTURE_MIN_FILTER,min_filter);
                glTextureParameteri(texture_id,GL_TEXTURE_MAG_FILTER,mag_filter);
                glTextureParameteri(texture_id,GL_TEXTURE_WRAP_S,wrap_s);
                glTextureParameteri(texture_id,GL_TEXTURE_WRAP_T,wrap_t);
				glTextureParameteri(texture_id,GL_TEXTURE_WRAP_R,wrap_r);
            }

            return(true);
        }

        int Texture3D::GetImage(void *data_pointer,TSF fmt,int level)
        {
            if(!TextureSourceFormatCheck(fmt))
            {
                LOG_ERROR(OS_TEXT("glTexture3D::GetImage,fmt error =")+OSString(fmt));
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

        bool Texture3D::ChangeImage(uint l,uint t,uint s,uint w,uint h,uint d,void *data,uint bytes,TSF sf)
        {
            if(	l>width||t>height||s>depth
				||w>width-l
				||h>height-t
				||d>depth-s
				||!data
				||!TextureSourceFormatCheck(sf))
                RETURN_FALSE;

            const TextureFormat *sfmt=TextureFormatInfoList+sf;       //原始数据格式

            if(sfmt->compress)
                glCompressedTextureSubImage3D(texture_id,0,l,t,s,w,h,d,sfmt->video_format,bytes,data);
            else
                glTextureSubImage3D(texture_id,0,l,t,s,w,h,d,sfmt->color_format,sfmt->data_type,data);

            return(true);
        }

        void Texture3D::SetWrapS(uint wrap)
        {
            if(wrap_s==wrap)return;

            wrap_s=wrap;

            glTextureParameteri(texture_id,GL_TEXTURE_WRAP_S,wrap_s);
        }

        void Texture3D::SetWrapT(uint wrap)
        {
            if(wrap_t==wrap)return;

            wrap_t=wrap;

            glTextureParameteri(texture_id,GL_TEXTURE_WRAP_T,wrap_t);
        }

        void Texture3D::SetWrapR(uint wrap)
        {
            if(wrap_r==wrap)return;

            wrap_r=wrap;

            glTextureParameteri(texture_id,GL_TEXTURE_WRAP_R,wrap_r);
        }
    }//namespace graph
}//namespace hgl
