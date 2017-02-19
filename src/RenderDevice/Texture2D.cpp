#include<hgl/graph/Texture2D.h>
#include<hgl/LogInfo.h>
#include"TextureBaseControl.h"

namespace hgl
{
    namespace graph
    {
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

            if (!_SetImage(ptr))
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
            
            return _GetImage(data_pointer,fmt,level,width,height);
        }

        bool Texture2D::ChangeImage(uint l, uint t, uint w, uint h, void *data, uint bytes, TSF sf)
        {
            if (l>width || t>height
                || w>width - l
                || h>height - t
                || !data
                || !TextureSourceFormatCheck(sf))
                RETURN_FALSE;

            return _ChangeImage(l,t,w,h, data, bytes, sf);
        }
    }//namespace graph

    namespace graph
    {
        Texture2D *CreateTexture2DDSA(TextureBaseControl *tbc);
        Texture2D *CreateTexture2DBind(TextureBaseControl *tbc);

        Texture2D *(*_CreateTexture2D)(TextureBaseControl *tbc) = nullptr;

        Texture2D *CreateTexture2D()
        {
            TextureBaseControl *tbc = TextureBaseControlCreate(HGL_TEXTURE_2D, HGL_TEX_BIND_2D);

            if (!tbc)RETURN_ERROR_NULL;

            return(_CreateTexture2D(tbc));
        }

        void InitTexture2DDSA()
        {
            _CreateTexture2D = CreateTexture2DDSA;
        }
    }//namespace graph
}//namespace hgl
