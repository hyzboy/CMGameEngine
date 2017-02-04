#include<hgl/graph/Texture1D.h>
#include<hgl/graph/TextureData.h>
#include"API/apiTexture.h"
#include<hgl/LogInfo.h>

namespace hgl
{
	namespace graph
	{
		Texture1D::Texture1D():Texture(HGL_TEXTURE_1D,HGL_TEX_BIND_1D)
		{
            length=0;
		}

		bool Texture1D::SetImage(Texture1DData *ptr)
		{
            if (!ptr)
            {
                LOG_ERROR(OS_TEXT("param error,ptr=nullptr"));
                RETURN_FALSE;
            }

			if (!ptr->length)
			{
				LOG_ERROR(OS_TEXT("size error,length=") + OSString(ptr->length));
                RETURN_FALSE;
			}

			length = ptr->length;

            video_format = ptr->GetVideoFormat();
			color_format = ptr->source_format->color_format;

            if (!api::SetTexImage1D(this->texture_id,ptr))
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

            return api::GetTexImage1D(texture_id,data_pointer,fmt,level,length);
		}

		bool Texture1D::ChangeImage(uint s, uint l, void *data, uint bytes, TSF sf)
		{
			if (s >= length
				|| l>length - s
				|| !data
				|| !TextureSourceFormatCheck(sf))
			{
				LOG_ERROR(OS_TEXT("fmt error =") + OSString(sf));
				return(false);
			}

            return api::ChangeTexImage1D(texture_id,s,l,data,bytes,sf);
		}
	}//namespace graph
}//namespace hgl
