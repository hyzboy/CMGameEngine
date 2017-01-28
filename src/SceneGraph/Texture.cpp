#include<hgl/graph/Texture.h>

namespace hgl
{
	namespace graph
	{
		Texture::Texture(uint t,uint bt):type(t),binding_type(bt)
		{
			texture_id=type=color_format=video_format=0;

			bitmap_data=0;

			min_filter=mag_filter=HGL_FILTER_LINEAR;
		}

        void Texture::SetMinFilter(uint mf)
        {
            if(min_filter==mf)return;

            min_filter=mf;

            glTextureParameteri(texture_id,GL_TEXTURE_MIN_FILTER,min_filter);
        }

        void Texture::SetMagFilter(uint mf)
        {
            if(mag_filter==mf)return;

            mag_filter=mf;

            glTextureParameteri(texture_id,GL_TEXTURE_MAG_FILTER,mag_filter);
        }

        void Texture::GetMipmapLevel(int &base_level,int &max_level)
        {
            glGetTextureParameteriv(texture_id,GL_TEXTURE_BASE_LEVEL,&base_level);
            glGetTextureParameteriv(texture_id,GL_TEXTURE_MAX_LEVEL,&max_level);
        }
	}//namespace graph
}//namespace hgl
