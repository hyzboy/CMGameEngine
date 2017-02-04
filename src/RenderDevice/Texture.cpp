#include<hgl/graph/Texture.h>
#include"API/apiTexture.h"

namespace hgl
{
	namespace graph
	{
        Texture::Texture(uint t, uint bt)													//请使用CreateTexture()来创建贴图,函数在Render.h中定义
        {
            type = t;
            binding_type = bt;

            color_format = video_format = 0;

            texture_id=api::CreateTexture(type);
        }

        Texture::~Texture() 
        {
            api::DeleteTexture(texture_id);
        }

        void Texture::GenMipmaps()
        {
            api::GenMipmaps(type,binding_type,texture_id);
        }

        //void Texture::GetMipmapLevel(int &base_level,int &max_level)
        //{
        //    glGetTextureParameteriv(texture_id,GL_TEXTURE_BASE_LEVEL,&base_level);
        //    glGetTextureParameteriv(texture_id,GL_TEXTURE_MAX_LEVEL,&max_level);
        //}
	}//namespace graph
}//namespace hgl
