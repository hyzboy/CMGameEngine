#include<hgl/graph/Texture1D.h>
#include<hgl/LogInfo.h>
#include"TextureBaseControl.h"

namespace hgl
{
    namespace graph
    {
        bool Texture1D::SetImage(Texture1DData *ptr)
        {
            if (!ptr)
            {
                LOG_ERROR(OS_TEXT("param error,ptr=nullptr"));
                RETURN_FALSE;
            }

            if (!ptr->length)
            {
                LOG_ERROR(OS_TEXT("size error,image length=") + OSString(ptr->length));
                RETURN_FALSE;
            }

            length = ptr->length;

            video_format = ptr->GetVideoFormat();
            pixel_format = ptr->GetPixelFormat();

            if (!_SetImage(ptr))
            {
                length=0;
                RETURN_FALSE;
            }

            return(true);
        }

        int Texture1D::GetImage(void *data_pointer, TSF fmt, int level)
        {
            if (!TextureSourceFormatCheck(fmt))
            {
                LOG_ERROR(OS_TEXT("glTexture1D::GetImage,fmt error =") + OSString(fmt));
                return(-1);
            }

            return _GetImage(data_pointer,fmt,level);
        }

        bool Texture1D::ChangeImage(uint s, uint l, void *data, uint bytes, TSF sf)
        {
            if (s >= length
                || l>length - s
                || !data
                || !TextureSourceFormatCheck(sf))
            RETURN_FALSE;

            return _ChangeImage(s,l,data,bytes,sf);
        }
    }//namespace graph

    namespace graph
    {        
        Texture1D *CreateTexture1DDSA(TextureBaseControl *tbc);
        Texture1D *CreateTexture1DBind(TextureBaseControl *tbc);
        
        Texture1D *(*_CreateTexture1D)(TextureBaseControl *tbc)=nullptr;
        
        Texture1D *CreateTexture1D()
        {
            TextureBaseControl *tbc=TextureBaseControlCreate(HGL_TEXTURE_1D,HGL_TEX_BIND_1D);

            if(!tbc)RETURN_ERROR_NULL;
            
            return(_CreateTexture1D(tbc));
        }

        Texture1D *CreateTexture1D(Texture1DData *ptr)
        {
            Texture1D *tex=CreateTexture1D();

            if(!tex)RETURN_ERROR_NULL;

            tex->SetImage(ptr);
            return(tex);
        }

        Texture1D *CreateTexture1D(uint length, TSF format)
        {
            Texture1D *tex = CreateTexture1D();

            if (!tex)RETURN_ERROR_NULL;

            tex->SetImage(length,format);
            return(tex);
        }

        Texture1D *CreateTexture1D(uint length, void *bitmap, uint bitmap_bytes, TSF source_format, uint video_format)
        {
            Texture1D *tex = CreateTexture1D();

            if (!tex)RETURN_ERROR_NULL;

            tex->SetImage(length, bitmap,bitmap_bytes,source_format, video_format);
            return(tex);
        }

        void InitTexture1DDSA()
        {
            _CreateTexture1D=CreateTexture1DDSA;
        }
    }//namespace graph
}//namespace hgl
