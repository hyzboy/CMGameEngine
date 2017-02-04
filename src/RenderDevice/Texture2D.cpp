#include<hgl/graph/Texture2D.h>
#include"API/apiTexture.h"
#include<hgl/LogInfo.h>

namespace hgl
{
    namespace graph
    {
        Texture2D::Texture2D():Texture(HGL_TEXTURE_2D, HGL_TEX_BIND_2D)
        {
            width = height = 0;
        }

        Texture2D::Texture2D(Texture2DData *ptr) : Texture(HGL_TEXTURE_2D, HGL_TEX_BIND_2D)
        {
            width = height = 0;
            SetImage(ptr);
        }

        Texture2D::Texture2D(uint w, uint h, uint vf) : Texture(HGL_TEXTURE_2D, HGL_TEX_BIND_2D)
        {
            width = height = 0;
            SetImage(w, h, nullptr,0,HGL_SF_NONE, vf);
        }

        Texture2D::Texture2D(uint w, uint h, void *data, uint size, TSF sf, uint vf) : Texture(HGL_TEXTURE_2D, HGL_TEX_BIND_2D)
        {
            width = height = 0;
            SetImage(w,h,data,size,sf,vf);
        }

        bool Texture2D::SetImage(Texture2DData *ptr)
        {
            if (!ptr)
            {
                LOG_ERROR(OS_TEXT("param error,ptr=nullptr"));
                RETURN_FALSE;
            }

            if (!ptr->width || !ptr->height)
            {
                LOG_ERROR(OS_TEXT("size error,width=") + OSString(ptr->width) + OS_TEXT(",height=") + OSString(ptr->height));
                return(false);
            }

            width = ptr->width;
            height = ptr->height;

            video_format = ptr->GetVideoFormat();
            color_format = ptr->GetColorFormat();

            if (!api::SetTexImage2D(this->texture_id, ptr))
            {
                width=height= 0;
                RETURN_FALSE;
            }

            return(true);
        }

        int Texture2D::GetImage(void *data_pointer, TSF fmt, int level)
        {
            if (!TextureSourceFormatCheck(fmt))
            {
                LOG_ERROR(OS_TEXT("glTexture2D::GetImage,fmt error =") + OSString(fmt));
                return(-1);
            }
            
            return api::GetTexImage2D(texture_id,data_pointer,fmt,level,width,height);
        }

        bool Texture2D::ChangeImage(uint l, uint t, uint w, uint h, void *data, uint bytes, TSF sf)
        {
            if (l>width || t>height
                || w>width - l
                || h>height - t
                || !data
                || !TextureSourceFormatCheck(sf))
                RETURN_FALSE;

            return api::ChangeTexImage2D(texture_id, l,t,w,h, data, bytes, sf);
        }
    }//namespace graph
}//namespace hgl
