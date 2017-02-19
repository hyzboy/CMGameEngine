#include<hgl/graph/Texture.h>
#include"TextureBaseControl.h"

namespace hgl
{
	namespace graph
	{
        Texture::Texture(TextureBaseControl *c)													//请使用CreateTexture()来创建贴图,函数在Render.h中定义
        {
            type = c->type;
            binding_type = c->binding_type;

            texture_id=c->texture_id;

            color_format = video_format = 0;
        }

        Texture::~Texture() 
        {
            glDeleteTextures(1,&texture_id);
        }

        void Texture::GenMipmaps()
        {
            tbc->GenMipmaps();
        }

        void Texture::GetMipmapLevel(int &base_level,int &max_level)
        {
            tbc->GetMipmapLevel(base_level,max_level);
        }
	}//namespace graph

    namespace graph
    {
        class TextureBaseControl;
        class Texture1D;
        class Texture2D;

        void InitTextureBaseControlDSA();
        void InitTextureBaseControlBind();
		void InitTexture1DDSA();
        void InitTexture2DDSA();

        bool InitTextureAPI()
        {
            if(GLEW_ARB_direct_state_access)
            {
                InitTextureBaseControlDSA();
                InitTexture1DDSA();
                InitTexture2DDSA();
            }

            return(true);
        }
    }//namespace graph
}//namespace hgl
