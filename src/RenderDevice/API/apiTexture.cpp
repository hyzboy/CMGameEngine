#include"apiTexture.h"
#include<glew/include/GL/glew.h>

namespace hgl
{
	namespace graph
	{
		namespace api
		{
            HGL_API_CREATE_TEXTURE		    CreateTexture = nullptr;									///<创建纹理
            HGL_API_CREATE_MULTI_TEXTURES	CreateMultiTextures = nullptr;							///<创建多个纹理
            HGL_API_DELETE_TEXTURE			DeleteTexture = nullptr;
            HGL_API_DELETE_MULTI_TEXTURES   DeleteMultiTextures = nullptr;
            
            HGL_API_GEN_MIPMAPS             GenMipmaps=nullptr;

            HGL_API_SET_TEX_IMAGE_1D        SetTexImage1D = nullptr;
            HGL_API_SET_TEX_IMAGE_2D        SetTexImage2D = nullptr;
            HGL_API_SET_TEX_IMAGE_3D        SetTexImage3D = nullptr;
            HGL_API_SET_TEX_IMAGE_1D_ARRAY  SetTexImage1DArray = nullptr;
            HGL_API_SET_TEX_IMAGE_2D_ARRAY  SetTexImage2DArray = nullptr;

            HGL_API_GET_TEX_IMAGE_1D        GetTexImage1D = nullptr;
            HGL_API_GET_TEX_IMAGE_2D        GetTexImage2D = nullptr;

            HGL_API_CHANGE_TEX_IMAGE_1D     ChangeTexImage1D = nullptr;
            HGL_API_CHANGE_TEX_IMAGE_2D     ChangeTexImage2D = nullptr;

			// 1.glBindTexture+glTexImage(OpenGL 1.0)
			// 2.glBindTexture+glTexStorage(4.2)
			// 3.glTextureImage(OpenGL 4.5)
			// 4.glTextureStorage(4.5)

			void InitTextureBind();
			bool InitTextureBindStorage();
			bool InitTextureDSAStorage();

			/**
			* 初始化纹理API 
			**/
			bool InitTextureAPI()
			{
				if(GLEW_ARB_direct_state_access)
					if (InitTextureDSAStorage())return(true);

				//if (GLEW_ARB_texture_storage)
				//	if (InitTextureBindStorage())return(true);

				//InitTextureBind();
				return(true);
			}
		}//namespace api
	}//namespace graph
}//namespace hgl
