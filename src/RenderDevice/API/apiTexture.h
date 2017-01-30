#ifndef HGL_API_TEXTURE_INCLUDE
#define HGL_API_TEXTURE_INCLUDE

#include<hgl/graph/Texture.h>
namespace hgl
{
	namespace graph
	{
		namespace api
		{
			bool InitTextureAPI();

			using HGL_API_CREATE_TEXTURE		=Texture *(*)(uint format);
			using HGL_API_CREATE_MULTI_TEXTURE	=Texture **(*)(uint format, uint count);

			HGL_API_CREATE_TEXTURE			CreateTexture=nullptr;											///<创建纹理
			HGL_API_CREATE_MULTI_TEXTURE	CreateMultiTexture = nullptr;							///<创建多个纹理

		}//namespace api
	}//namespace graph
}//namespace hgl
#endif//HGL_API_TEXTURE_INCLUDE
