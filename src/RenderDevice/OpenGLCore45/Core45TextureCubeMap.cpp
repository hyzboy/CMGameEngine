#include<glew/include/GL/glew.h>
#include<hgl/graph/TextureCubeMap.h>
#include<hgl/LogInfo.h>

namespace hgl
{
    namespace graph
    {
        TextureCubeMap::TextureCubeMap()
        {
            type=HGL_TEXTURE_CUBE_MAP;
			width=height=0;
            glCreateTextures(GL_TEXTURE_CUBE_MAP,1,&texture_id);
        }

        TextureCubeMap::~TextureCubeMap()
        {
            glDeleteTextures(1,&texture_id);
        }

        bool TextureCubeMap::SetImage(uint w,uint h,void_pointer data[6],uint image_size,TSF sf,uint vf,uint ltp)
        {
            if(!w||!h)
            {
                LOG_ERROR(OS_TEXT("size error,width=")+OSString(w)+OS_TEXT(",height=")+OSString(h));
                return(false);
            }

			if(!TextureSourceFormatCheck(sf))
			{
				LOG_ERROR(OS_TEXT("sf error =")+OSString(sf));
				return(false);
			}

            width=w;
            height=h;

            const TextureFormat *sfmt=TextureFormatInfoList+sf;       //原始数据格式

            if(vf==0)
                vf=TextureFormatInfoList[sf].video_format;

            const bool gen_mip=ltp&ltGenMipmaps;            //取得是否创建mipmaps

            if(sfmt->compress)      //原本就是压缩格式
            {
				for(int i=0;i<6;i++)
					if(data[i])
						glCompressedTextureSubImage2D(texture_id,i,0,0,w,h,vf,image_size,data[i]);
            }
            else                    //正常非压缩格式
            {
				for(int i=0;i<6;i++)
					glTextureStorage2D(texture_id, i, vf, w, h);

				for(int i=0;i<6;i++)
					if(data[i])
						glTextureSubImage2D(texture_id, i, 0, 0, w, h, sfmt->color_format, sfmt->data_type, data[i]);
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

                //未来使用Sampler Object，则不再需要以下部分
                glTextureParameteri(texture_id,GL_TEXTURE_MIN_FILTER,min_filter);
                glTextureParameteri(texture_id,GL_TEXTURE_MAG_FILTER,mag_filter);
                glTextureParameteri(texture_id,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
                glTextureParameteri(texture_id,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
				glTextureParameteri(texture_id,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
            }

            return(true);
        }

        int TextureCubeMap::GetImage(void *data_pointer,TSF fmt,int face)
        {
            if(!TextureSourceFormatCheck(fmt))
            {
                LOG_ERROR(OS_TEXT("glTextureCubeMap::GetImage,fmt error =")+OSString(fmt));
                return(-1);
            }

            int compress;
            int bytes;

            const TextureFormat *tsf=TextureFormatInfoList+fmt;

            glGetTextureLevelParameteriv(texture_id,face,GL_TEXTURE_COMPRESSED,&compress);

            if(compress)
            {
                glGetTextureLevelParameteriv(texture_id,face,GL_TEXTURE_COMPRESSED_IMAGE_SIZE,&bytes);

                if(data_pointer)
                    glGetCompressedTextureImage(texture_id,face,bytes,data_pointer);
            }
            else
            {
                if(tsf->video_bytes==0)return(-1);

                bytes=width*height*tsf->video_bytes;

                if(data_pointer)
                    glGetTextureImage(texture_id,face,tsf->color_format,tsf->data_type,bytes,data_pointer);
            }

            return(bytes);
        }

        bool TextureCubeMap::ChangeImage(uint face,uint l,uint t,uint w,uint h,void *data,uint bytes,TSF sf)
        {
            if(	l>width||t>height
				||w>width-l
				||h>height-t
				||!data
				||!TextureSourceFormatCheck(sf))
                RETURN_FALSE;

            const TextureFormat *sfmt=TextureFormatInfoList+sf;       //原始数据格式

            if(sfmt->compress)
                glCompressedTextureSubImage2D(texture_id,face,l,t,w,h,sfmt->video_format,bytes,data);
            else
                glTextureSubImage2D(texture_id,face,l,t,w,h,sfmt->color_format,sfmt->data_type,data);

            return(true);
        }
    }//namespace graph
}//namespace hgl
