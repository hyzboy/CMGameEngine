#include"apiTexture.h"
#include<glew/include/GL/glew.h>

namespace hgl
{
	namespace graph
	{
		namespace api
		{
			// 1.glBindTexture+glTexImage(OpenGL 1.0)
			// 2.glBindTexture+glTexStorage(4.2)
			// 3.glTextureImage(OpenGL 4.5)
			// 4.glTextureStorage(4.5)

			void InitTextureBind();
			bool InitTextureBindStorage();
			bool InitTextureDSAStorage();

			/**
			* ≥ı ºªØŒ∆¿ÌAPI 
			**/
			bool InitTextureAPI()
			{
				if(GLEW_ARB_direct_state_access)
					if (InitTextureDSAStorage())return(true);

				if (GLEW_ARB_texture_storage)
					if (InitTextureBindStorage())return(true);

				InitTextureBind();
				return(true);
			}
		}//namespace api
	}//namespace graph
}//namespace hgl
