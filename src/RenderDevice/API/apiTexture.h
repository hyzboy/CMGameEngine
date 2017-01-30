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

			using HGL_API_CREATE_TEXTURE		=uint (*)(uint format);
			using HGL_API_CREATE_MULTI_TEXTURES	=uint (*)(uint *,const uint format, const uint count);
			using HGL_API_DELETE_TEXTURE		=void(*)(const uint);
			using HGL_API_DELETE_MULTI_TEXTURES	=void(*)(const uint *,const uint);

			HGL_API_CREATE_TEXTURE			CreateTexture=nullptr;											///<创建纹理
			HGL_API_CREATE_MULTI_TEXTURES	CreateMultiTextures = nullptr;							///<创建多个纹理
			HGL_API_DELETE_TEXTURE			DeleteTexture = nullptr;
			HGL_API_DELETE_MULTI_TEXTURES	DeleteMultiTextures = nullptr;
		}//namespace api
	}//namespace graph
}//namespace hgl
#endif//HGL_API_TEXTURE_INCLUDE
